#include "OBJLoader.hpp"
#include <QtGlobal>
#include <QStringList>
#include <QTextStream>
#include <QRegularExpression>
#include <QFileInfo>
#include <QFile>
#include <QDir>
#include <cstring>

//*********************************************************************************

typedef enum
{
	TAGNAME_COMMENT = 0,
	TAGNAME_MATLIB,
	TAGNAME_VERTEX,
	TAGNAME_FACE3,
	TAGNAME_FACE4,
	TAGNAME_NORMAL,
	TAGNAME_TEXTURECOORDS,
	TAGNAME_OBJECT,
	TAGNAME_MATERIAL,
	TAGNAME_AMBIENT,
	TAGNAME_DIFFUSE,
	TAGNAME_SPECULAR,
	TAGNAME_EMISSION,
	TAGNAME_TEXTURE,
	TAGNAME_OPACITY,
	TAGNAME_SHININESS,
	TAGNAME_ILLUMINATION,
	TAGNAME_UNKNOWN,

	TAGNAME_MAXCOUNT
} TAG_NAME;

//*********************************************************************************

static QString TokenizeFileLine(QString const& Line, TAG_NAME *Tag);

static bool ExtractParameters(TAG_NAME Tag, QString const& Args, QString& String);
static bool ExtractParameters(TAG_NAME Tag, QString const& Args, QVector3D& Vector3D);
static bool ExtractParameters(TAG_NAME Tag, QString const& Args, QVector2D& Vector2D);
static bool ExtractParameters(TAG_NAME Tag, QString const& Args, float& Float);
static bool ExtractParameters(TAG_NAME Tag, QString const& Args, float* FloatArray4);
static bool ExtractParameters(TAG_NAME Tag, QString const& Args, quint32& UInt32);
static bool ExtractParameters(TAG_NAME Tag, QString const& Args, FACE& Face);
static bool ExtractParameters(TAG_NAME Tag, QString const& Args, FACE& Face1, FACE& Face2);

static bool ParseMaterialLibrary(SCENE *S, QString const& SceneFile, QString const& MaterialLib, bool ParseOnly);



SCENE *ReadOBJFile(QString const& FileName, bool GenerateMissingNormals)
{
    bool ok;

    // On cible le fichier. Si le fichier est introuvable, on quitte
    QFile Scene(FileName);
    if (!Scene.exists())
        return nullptr;

    // On ouvre le fichier. Si son ouverture foire, on quitte
    if (!Scene.open(QIODevice::ReadOnly))
        return nullptr;

    // On crée un nouvel objet SCENE (+ gestion de l'erreur d'allocation)
    SCENE* S = new SCENE;
    if (S == nullptr)
        goto Exit;

    // PARSING : 1ère passe
    while (!Scene.atEnd())
    {
        // Lecture de la ligne courante
        QString line = Scene.readLine(512);
        line = line.trimmed();
        // Parsing de cette ligne
        TAG_NAME Tag;
        QString Args = TokenizeFileLine(line, &Tag);
        // Traitement en fonction du tag
        switch (Tag)
        {
            case TAGNAME_VERTEX :
                ++S->VerticesCount;
                break;
            case TAGNAME_NORMAL :
                ++S->NormalsCount;
                break;
            case TAGNAME_TEXTURECOORDS :
                ++S->UVCount;
                break;
            case TAGNAME_FACE3 :
                ++S->FacesCount;
                break;
            case TAGNAME_FACE4 :
                S->FacesCount += 2;
                break;
            case TAGNAME_OBJECT :
                ++S->ObjectsCount;
                break;
            case TAGNAME_MATLIB :
            {
                QString matlib;
                ok = ExtractParameters(Tag, Args, matlib);
                Q_ASSERT(ok);
                ok = ParseMaterialLibrary(S, FileName, matlib, true);
                Q_ASSERT(ok);
                break;
            }
        }
    }

    // Si à l'issue de cette première passe, il manque les vertices ou les faces, on abandonne
    if (S->VerticesCount == 0 || S->FacesCount == 0)
    {
        ReleaseScene(S);
        S = nullptr;
        goto Exit;
    }

    // Préparation des "scene buffers"

    if (GenerateMissingNormals && (S->NormalsCount == 0))
        S->NormalsCount = S->VerticesCount;
    else
        GenerateMissingNormals = false;

    S->Vertices = new VERTEX[S->VerticesCount + S->NormalsCount];
    if (S->Vertices == nullptr)
    {
        ReleaseScene(S);
        S = nullptr;
        goto Exit;
    }

    if (S->NormalsCount != 0)
        S->Normals = S->Vertices + S->VerticesCount;
    else
        S->Normals = nullptr;

    if (S->UVCount != 0)
    {
        S->UV = new TEXTURE_COORDS[S->UVCount];
        if (S->UV == nullptr)
        {
            ReleaseScene(S);
            S = nullptr;
            goto Exit;
        }
    }

    S->Faces = new FACE[S->FacesCount];
    if (S->Faces == nullptr)
    {
        ReleaseScene(S);
        S = nullptr;
        goto Exit;
    }

    S->Materials = new MATERIAL[S->MaterialsCount];
    if (S->Materials == nullptr)
    {
        ReleaseScene(S);
        S = nullptr;
        goto Exit;
    }

    //--- Init objects table

    S->Objects = new OBJECT[S->ObjectsCount+1];
    if (S->Objects == nullptr)
    {
        ReleaseScene(S);
        S = nullptr;
        goto Exit;
    }


    // PARSING : 2ème passe
    S->VerticesCount = S->NormalsCount = S->UVCount = S->FacesCount = S->ObjectsCount = S->MaterialsCount = 0;
    Scene.seek(0);
    while (!Scene.atEnd())
    {
        // Lecture de la ligne courante
        QString line = Scene.readLine(512);
        line = line.trimmed();
        // Parsing de cette ligne
        TAG_NAME Tag;
        QString Args = TokenizeFileLine(line, &Tag);
        // Traitement en fonction du tag
        switch (Tag)
        {
            case TAGNAME_VERTEX :
                ok = ExtractParameters(Tag, Args, S->Vertices[S->VerticesCount++]);
                Q_ASSERT(ok);
                break;
            case TAGNAME_NORMAL :
                if (S->Normals)
                {
                    ok = ExtractParameters(Tag, Args, S->Normals[S->NormalsCount++]);
                    Q_ASSERT(ok);
                }
                break;
            case TAGNAME_TEXTURECOORDS :
                ok = ExtractParameters(Tag, Args, S->UV[S->UVCount++]);
                Q_ASSERT(ok);
                break;
            case TAGNAME_FACE3 :
                ok = ExtractParameters(Tag, Args, S->Faces[S->FacesCount++]);
                Q_ASSERT(ok);
                S->Objects[S->ObjectsCount].FacesCount += 1;
                for (uint i = 0; i < 3; ++i)
                {
                    Q_ASSERT(S->NormalsCount == 0 ||
                             S->Faces[S->FacesCount - 1].Normals[i] < S->NormalsCount);
                    Q_ASSERT(S->UVCount == 0 ||
                             S->Faces[S->FacesCount - 1].UV[i] < S->UVCount);
                }
                break;
            case TAGNAME_FACE4 :
                ok = ExtractParameters(Tag, Args, S->Faces[S->FacesCount], S->Faces[S->FacesCount + 1]);
                Q_ASSERT(ok);
                S->FacesCount += 2;
                S->Objects[S->ObjectsCount].FacesCount += 2;
                for (uint i = 0; i < 3; ++i)
                {
                    for (uint j = 0; j < 2; ++j)
                    {
                        Q_ASSERT(S->NormalsCount == 0 ||
                                 S->Faces[S->FacesCount - 1 - j].Normals[i] < S->NormalsCount);
                        Q_ASSERT(S->UVCount == 0 ||
                                 S->Faces[S->FacesCount - 1 - j].UV[i] < S->UVCount);
                    }
                }
                break;
            case TAGNAME_OBJECT :
            {
                // Switch to next object
                ++S->ObjectsCount;
                S->Objects[S->ObjectsCount].FirstFace = S->FacesCount;
                S->Objects[S->ObjectsCount].FacesCount = 0;
                // Extrait et recherche le material
                QString material;
                ok = ExtractParameters(Tag, Args, material);
                Q_ASSERT(ok);
                S->Objects[S->ObjectsCount].Material = 0u;
                for (; S->Objects[S->ObjectsCount].Material < S->MaterialsCount; S->Objects[S->ObjectsCount].Material += 1)
                {
                    if (material == S->Materials[S->Objects[S->ObjectsCount].Material].Name)
                        break;
                }
                if (S->Objects[S->ObjectsCount].Material >= S->MaterialsCount)
                    S->Objects[S->ObjectsCount].Material = 0xFFFFFFFF;
                break;
            }
            case TAGNAME_MATLIB :
            {
                QString matlib;
                ok = ExtractParameters(Tag, Args, matlib);
                Q_ASSERT(ok);
                ok = ParseMaterialLibrary(S, FileName, matlib, false);
                Q_ASSERT(ok);
                break;
            }
        }
    }

    S->ObjectsCount += 1;

    // Régénération des normales manquantes
    if (GenerateMissingNormals)
    {
        // TODO : s'en occuper (c'est pas fondamental selon moi)
    }

    // Calcul du centre de chaque objet
    for (uint i = 0; i < S->ObjectsCount; ++i)
    {
        for (uint j = 0; j < S->Objects[i].FacesCount; ++j)
        {
            VERTEX FaceCenter = VERTEX(0, 0, 0);
            for (uint k = 0 ; k < 3; ++k)
            {
                FaceCenter += S->Vertices[S->Faces[S->Objects[i].FirstFace + j].Vertices[k]];
            }
            S->Objects[i].Center += FaceCenter / 3;
        }
        S->Objects[i].Center /= S->Objects[i].FacesCount;
    }

    // Fin de la fonction : on ferme le fichier scène et on renvoie
Exit:
    Scene.close();
    return S;
}

//*********************************************************************************
// Parse file line and extract parameters
//
static QString TokenizeFileLine(const QString& Line, TAG_NAME *Tag)
{
    QString ParsedTag, _line;
    QStringList Tags = {"#", "v", "vt", "vn", "f", "f", "mtllib", "usemtl", "newmtl", "Ns", "d", "Ka", "Kd", "Ks", "Ke", "map_Kd", "illum", ""};
    QList<TAG_NAME> TagNames = {TAGNAME_COMMENT, TAGNAME_VERTEX, TAGNAME_TEXTURECOORDS, TAGNAME_NORMAL, TAGNAME_FACE3, TAGNAME_FACE4, TAGNAME_MATLIB, TAGNAME_OBJECT, TAGNAME_MATERIAL, TAGNAME_SHININESS, TAGNAME_OPACITY, TAGNAME_AMBIENT, TAGNAME_DIFFUSE, TAGNAME_SPECULAR, TAGNAME_EMISSION, TAGNAME_TEXTURE, TAGNAME_ILLUMINATION, TAGNAME_UNKNOWN};

    //--- Skip leading and ending spaces
    _line = Line.trimmed();

	//--- Copy tag
    ParsedTag = _line.split(QRegularExpression("\\s+"))[0];

	//--- Identify tag
    int index = 0;
    while (index < Tags.size()-1 && Tags[index] != ParsedTag)
        ++index;
    *Tag = TagNames[index];

    //--- Get the rest
    _line = _line.right(_line.size() - ParsedTag.size());
    _line = _line.trimmed();

    //--- Deals with the case of faces (triangles or quads ?)
    if (*Tag == TAGNAME_FACE3 || *Tag == TAGNAME_FACE4)
    {
        if (_line.split(QRegularExpression("\\s+")).size() >= 4)
            *Tag = TAGNAME_FACE4;
		else
            *Tag = TAGNAME_FACE3;
	}

    return _line;
}



//*********************************************************************************
// Release scene contents
//
void ReleaseScene(SCENE *Scene)
{
	unsigned int i;

    if (!Scene)
		return;

    if (Scene->Objects)
        delete [] Scene->Objects;

    if (Scene->Vertices)
        delete [] Scene->Vertices;

    if (Scene->UV)
        delete [] Scene->UV;

    if (Scene->Faces)
        delete [] Scene->Faces;

    if (Scene->Materials)
	{
        for (i=0; i<Scene->MaterialsCount; ++i)
            delete Scene->Materials[i].pDiffuse;

        delete [] Scene->Materials;
	}
}

static bool ExtractParameters(TAG_NAME Tag, QString const& Args, QString& String)
{
    if (Tag == TAGNAME_MATLIB ||
        Tag == TAGNAME_OBJECT ||
        Tag == TAGNAME_MATERIAL ||
        Tag == TAGNAME_TEXTURE)
    {
        String = Args;
        return true;
    }
    else
        return false;
}

static bool ExtractParameters(TAG_NAME Tag, QString const& Args, QVector3D& Vector3D)
{
    if (Tag == TAGNAME_VERTEX ||
        Tag == TAGNAME_NORMAL)
    {
        QTextStream ts(const_cast<QString*>(&Args), QIODevice::ReadOnly);
        float fX, fY, fZ;
        ts >> fX >> fY >> fZ;
        Vector3D.setX(fX);
        Vector3D.setY(fY);
        Vector3D.setZ(fZ);
        return true;
    }
    else
        return false;
}

static bool ExtractParameters(TAG_NAME Tag, QString const& Args, QVector2D& Vector2D)
{
    if (Tag == TAGNAME_TEXTURECOORDS)
    {
        QTextStream ts(const_cast<QString*>(&Args), QIODevice::ReadOnly);
        float fU, fV;
        ts >> fU >> fV;
        Vector2D.setX(fU);
        Vector2D.setY(fV);
        return true;
    }
    else
        return false;
}

static bool ExtractParameters(TAG_NAME Tag, QString const& Args, float& Float)
{
    if (Tag == TAGNAME_OPACITY || Tag == TAGNAME_SHININESS)
    {
        bool ok;
        Float = Args.toFloat(&ok);
        if (ok)
            return true;
        else
            return false;
    }
    else
        return false;
}

static bool ExtractParameters(TAG_NAME Tag, QString const& Args, float* FloatArray4)
{
    if (Tag == TAGNAME_AMBIENT ||
        Tag == TAGNAME_DIFFUSE ||
        Tag == TAGNAME_SPECULAR ||
        Tag == TAGNAME_EMISSION)
    {
        QTextStream ts(const_cast<QString*>(&Args), QIODevice::ReadOnly);
        ts >> FloatArray4[0] >> FloatArray4[1] >> FloatArray4[2];
        FloatArray4[3] = 1;
        return true;
    }
    else
    {
        return false;
    }
}

static bool ExtractParameters(TAG_NAME Tag, QString const& Args, quint32& UInt32)
{
    if (Tag == TAGNAME_ILLUMINATION)
    {
        bool ok;
        int val = Args.toInt(&ok);
        if (!ok)
            return false;
        if (val == 0)
            UInt32 = 0;
        else if (val == 1)
            UInt32 = RENDERFLAGS_LIGHTING;
        else
            UInt32 = RENDERFLAGS_LIGHTING | RENDERFLAGS_FOG;
        return true;
    }
    else
        return false;
}

static bool ExtractParameters(TAG_NAME Tag, QString const& Args, FACE& Face)
{
    bool ok;
    if (Tag == TAGNAME_FACE3)
    {
        QStringList members = Args.split(QRegularExpression("\\s+"), QString::SkipEmptyParts);
        if (members.size() != 3)
            return false;
        for (uint i = 0; i < 3; ++i)
        {
            QStringList inner = members[i].split('/');
            // Indice du vertex
            Face.Vertices[i] = inner[0].toUInt(&ok);
            if (!ok) return false;
            if (Face.Vertices[i] != 0) --Face.Vertices[i];
            // Coordonnée de texture
            if (inner.size() >= 2 && inner[1] != "")
            {
                Face.UV[i] = inner[1].toUInt(&ok);
                if (!ok) return false;
                if (Face.UV[i] != 0) --Face.UV[i];
            }
            else
                Face.UV[i] = 0;
            // Indice du vertex pour la normale
            if (inner.size() >= 3)
            {
                Face.Normals[i] = inner[2].toUInt(&ok);
                if (!ok) return false;
                if (Face.Normals[i] != 0) --Face.Normals[i];
            }
            else
                Face.Normals[i] = 0;
        }
        return true;
    }
    else
        return false;
}

static bool ExtractParameters(TAG_NAME Tag, QString const& Args, FACE& Face1, FACE& Face2)
{
    bool ok;
    if (Tag == TAGNAME_FACE4)
    {
        QStringList members = Args.split(QRegularExpression("\\s+"), QString::SkipEmptyParts);
        if (members.size() != 4)
            return false;
        static FACE* faces[2] = {&Face1, &Face2};
        static int usage[4][2] = {{0, 0}, {1, -1}, {2, 1}, {-1, 2}};
        for (uint i = 0; i < 4; ++i)
        {
            QStringList inner = members[i].split('/');
            for (uint j = 0; j < 2; ++j)
            {
                // Skippe cette itération si le vertex n'est pas utilisé dans la face actuelle
                if (usage[i][j] == -1)
                    continue;
                // Indice du vertex
                faces[j]->Vertices[usage[i][j]] = inner[0].toUInt(&ok);
                if (!ok) return false;
                if (faces[j]->Vertices[usage[i][j]] != 0)
                    --faces[j]->Vertices[usage[i][j]];
                // Coordonnée de texture
                if (inner.size() >= 2 && inner[1] != "")
                {
                    faces[j]->UV[usage[i][j]] = inner[1].toUInt(&ok);
                    if (!ok) return false;
                    if (faces[j]->UV[usage[i][j]] != 0)
                        --faces[j]->UV[usage[i][j]];
                }
                else
                    faces[j]->UV[usage[i][j]] = 0;
                // Indice du vertex pour la normale
                if (inner.size() >= 3)
                {
                    faces[j]->Normals[usage[i][j]] = inner[2].toUInt(&ok);
                    if (!ok) return false;
                    if (faces[j]->Normals[usage[i][j]] != 0)
                        --faces[j]->Normals[usage[i][j]];
                }
                else
                    faces[j]->Normals[usage[i][j]] = 0;
            }
        }
        return true;
    }
    else
        return false;
}

static bool ParseMaterialLibrary(SCENE *S, QString const& SceneFile, QString const& MaterialLib, bool ParseOnly)
{
    bool ok;

    // Si le chemin relatif du fichier matériel est vide, on quitte
    if (MaterialLib.isEmpty())
        return false;

    // On ouvre le fichier (ou comment écrire en 2 lignes ce qui en prend 15 en C bien crade...)
    QFile MatFile(QDir::cleanPath(QFileInfo(SceneFile).dir().filePath(MaterialLib)));
    if (!MatFile.open(QIODevice::ReadOnly))
        return false;

    // Boucle de lecture
    while (!MatFile.atEnd())
    {
        // Lecture de la ligne courante
        QString line = MatFile.readLine(512);
        line = line.trimmed();
        // Parsing de cette ligne
        TAG_NAME Tag;
        QString Args = TokenizeFileLine(line, &Tag);
        // Traitement en fonction du tag
        switch (Tag)
        {
            case TAGNAME_MATERIAL :
            {
                // Prends connaissance d'un nouveau matériel
                if (!ParseOnly)
                {
                    ok = ExtractParameters(Tag, Args, S->Materials[S->MaterialsCount].Name);
                    if (!ok) return false;
                    for (uint i = 0; i < 4; ++i)
                    {
                        S->Materials[S->MaterialsCount].Ambient[i] = 0.f;
                        S->Materials[S->MaterialsCount].Diffuse[i] = 0.f;
                        S->Materials[S->MaterialsCount].Specular[i] = 0.f;
                        S->Materials[S->MaterialsCount].Emission[i] = 0.f;
                    }
                    S->Materials[S->MaterialsCount].Diffuse[3] = 1.f;
                    S->Materials[S->MaterialsCount].Shininess = 128.f;
                    S->Materials[S->MaterialsCount].TextureFile = "";
                    S->Materials[S->MaterialsCount].pDiffuse = nullptr;
                    S->Materials[S->MaterialsCount].RenderFlags = 0u;
                }
                ++S->MaterialsCount;
                break;
            }
            case TAGNAME_OPACITY :
            {
                // Récupère les propriétés d'opacité
                if (ParseOnly)
                    break;
                ok = ExtractParameters(Tag, Args, S->Materials[S->MaterialsCount-1].Diffuse[3]);
                if (!ok) return false;
                break;
            }
            case TAGNAME_SHININESS :
            {
                // Récupère les propriétés de brillance
                if (ParseOnly)
                    break;
                ok = ExtractParameters(Tag, Args, S->Materials[S->MaterialsCount-1].Shininess);
                if (!ok) return false;
                if (S->Materials[S->MaterialsCount-1].Shininess < 1.f)
                    S->Materials[S->MaterialsCount-1].Shininess = 1.f;
                else if (S->Materials[S->MaterialsCount-1].Shininess > 128.f)
                    S->Materials[S->MaterialsCount-1].Shininess = 128.f;
                break;
            }
            case TAGNAME_ILLUMINATION :
            {
                // Récupère les propriétés d'éclairage
                if (ParseOnly)
                    break;
                ok = ExtractParameters(Tag, Args, S->Materials[S->MaterialsCount-1].RenderFlags);
                if (!ok) return false;
                break;
            }
            case TAGNAME_AMBIENT :
            {
                if (ParseOnly)
                    break;
                ok = ExtractParameters(Tag, Args, S->Materials[S->MaterialsCount-1].Ambient);
                if (!ok) return false;
                break;
            }
            case TAGNAME_DIFFUSE :
            {
                if (ParseOnly)
                    break;
                ok = ExtractParameters(Tag, Args, S->Materials[S->MaterialsCount-1].Diffuse);
                if (!ok) return false;
                break;
            }
            case TAGNAME_SPECULAR :
            {
                if (ParseOnly)
                    break;
                ok = ExtractParameters(Tag, Args, S->Materials[S->MaterialsCount-1].Specular);
                if (!ok) return false;
                break;
            }
            case TAGNAME_EMISSION :
            {
                if (ParseOnly)
                    break;
                ok = ExtractParameters(Tag, Args, S->Materials[S->MaterialsCount-1].Emission);
                if (!ok) return false;
                break;
            }
            case TAGNAME_TEXTURE :
            {
                if (ParseOnly)
                    break;
                ok = ExtractParameters(Tag, Args, S->Materials[S->MaterialsCount-1].TextureFile);
                if (!ok) return false;
                S->Materials[S->MaterialsCount-1].pDiffuse = new IMAGE_DATA(QDir::cleanPath(QFileInfo(SceneFile).dir().filePath(S->Materials[S->MaterialsCount-1].TextureFile)));
                break;
            }
        }
    }

    // Fermeture du fichier
    MatFile.close();

    return true;
}
