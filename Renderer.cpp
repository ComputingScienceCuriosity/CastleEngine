#include "Renderer.hpp"
#include <QDir>
#include <QFileInfo>
#include <QMessageBox>
#include <QApplication>
#include <qmath.h>
#include "Window.hpp"
#include "PrefabBase.hpp"
#include "Prefab.hpp"

#define PI 3.14159265f
#define PI_OVER_180 PI / 180.f
#define V_FORWARD   QVector3D(0.f, 0.f, -1.f);
#define V_BACK      QVector3D(0.f, 0.f, +1.f);
#define V_LEFT      QVector3D(-1.f, 0.f, 0.f);
#define V_RIGHT     QVector3D(+1.f, 0.f, 0.f);
#define V_UP        QVector3D(0.f, +1.f, 0.f);
#define V_DOWN      QVector3D(0.f, -1.f, 0.f);

GLfloat LightDiffuse[4] = {1.f, 1.f, 1.f, 1.f};
GLfloat LightAmbient[4] = {0.2f, 0.2f, 0.2f, 1.f};
GLfloat	LightSpecular[4] = {0.0f,0.0f,0.0f,1.0f};
GLfloat	LightEmission[4] = {0.0f,0.0f,0.0f,0.1f};
GLfloat	LightShininess(0.1f);

static bool operator< (QVector3D const& A, QVector3D const& B);
static bool operator< (FACE const& A, FACE const& B);
static FACE operator- (FACE const& f);

Renderer* Renderer::Instance = nullptr;

Renderer::Renderer()
{
}

Renderer::~Renderer()
{
    delete Instance;
}

Renderer*Renderer::instance()
{
    if (Instance == nullptr)
        Instance = new Renderer;
    return Instance;
}

QOpenGLShaderProgram*Renderer::program() const
{
    if (shaderProgramIndex != -1)
        return Programs[shaderProgramIndex];
    else
        return nullptr;
}

void Renderer::load()
{
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    // Initialisations Castle3D
    PrefabBase::initializeList();
    //initShaders();
    //initBO();
}

void Renderer::aspectRatio(int w, int h)
{
       // Initialisations OpenGL
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    Projection.setToIdentity();
    Projection.perspective(60, float(w)/float(h), 0.1, 10000);
    glLoadMatrixf(Projection.constData());

    glMatrixMode(GL_MODELVIEW);
}

void Renderer::render()
{
    glClearColor(0.f, 0.f, 1.f, 1.0f);
    glClearDepth(1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glEnable (GL_ALPHA_TEST);

    // Vue caméra
    ModelView.setToIdentity();
    glLoadMatrixf(ModelView.constData());
    //glLightfv(GL_LIGHT0, GL_POSITION, LightPosition);
    ModelView.rotate(-LookUpDown, 1.0f, 0.0f, 0.0f);
    ModelView.rotate(-Heading, 0.0f, 1.0f, 0.0f);
    ModelView.translate(Position * -1.f);
    glLoadMatrixf(ModelView.constData());

    // Éclairage
    glLightfv(GL_LIGHT0, GL_AMBIENT,  LightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  LightDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, LightSpecular);
    glLightfv(GL_LIGHT0, GL_EMISSION, LightEmission);
    glLightf (GL_LIGHT0, GL_SHININESS,LightShininess);



//    glPushMatrix();

    //glTranslatef(i, j, k);

//    VBO.bind();
//    glEnableClientState(GL_VERTEX_ARRAY);
//    glVertexPointer(3, GL_FLOAT, sizeof(float) * 3, 0);
//    glEnableClientState(GL_NORMAL_ARRAY);
//    glNormalPointer(GL_FLOAT, sizeof(float) * 3, (void*) 96);

//    IBO.bind();
//    glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);

//    glPopMatrix();


    for (FACE const& f : Faces)
    {
        glBegin(GL_TRIANGLES);
        for (uint i = 0; i < 3; ++i)
        {
            QVector3D const& V = Vertices[f.Vertices[i]];
            QVector3D const& N = Normals[f.Normals[i]];
            glNormal3f(N.x(), N.y(), N.z());
            glVertex3f(V.x(), V.y(), V.z());

        }
        glEnd();
    }

/*
    if (!Castel) return;
    Voxel* voxel;
          Castle const& c = *Castel;

    Voxel* lastVoxel;
    QVector3D lastSize;



  for (quint32 j = 0; j < c.z(); ++j)
  {
      for (quint32 k = 0; k < c.y(); ++k)
      {
          for (quint32 i = 0; i < c.x(); i += voxel->__Size.x() - voxel->Size.x() + 1)
          {


              voxel = Castel->get(i, j, k);
              if ( !voxel->isVisible || !voxel->isValid())
                  continue;

              if(voxel->prefabID()!="cube")
                  continue;

              if((i > 0 && i < c.x()) &&
                                 (j > 0 && j < c.y()) &&
                                 (k > 0 && k < c.z()) &&
                                 (c(i+1,j,k).isVisible && c(i-1,j,k).isVisible) &&
                                 (c(i,j+1,k).isVisible && c(i,j-1,k).isVisible) &&
                                 (c(i,j,k+1).isVisible && c(i,j,k-1).isVisible))
                                  continue;

              QVector3D scale = QVector3D(voxel->__Size.x(), voxel->__Size.y(), voxel->__Size.z());
              QVector3D position = QVector3D();
              position.setX(  i - c.x() / 2.0f+ (voxel->__Size.x() - voxel->Size.x())/ 2.0f );
              position.setY(  j - c.y() / 2.0f+ (voxel->__Size.y() - voxel->Size.y())/ 2.0f );
              position.setZ(  k - c.z() / 2.0f+ (voxel->__Size.z() - voxel->Size.z())/ 2.0f );

              glPushMatrix();
              glTranslatef( position.x(),position.y(),position.z());
              glScalef( scale.x(),scale.y(),scale.z());
              glBegin(GL_QUADS);

              // top
              glColor3f(1.0f, 0.0f, 0.0f);
              glNormal3f(0.0f, 1.0f, 0.0f);
              glVertex3f(-0.5f, 0.5f, 0.5f);
              glVertex3f(0.5f, 0.5f, 0.5f);
              glVertex3f(0.5f, 0.5f, -0.5f);
              glVertex3f(-0.5f, 0.5f, -0.5f);

              // front
              glColor3f(0.0f, 1.0f, 0.0f);
              glNormal3f(0.0f, 0.0f, 1.0f);
              glVertex3f(0.5f, -0.5f, 0.5f);
              glVertex3f(0.5f, 0.5f, 0.5f);
              glVertex3f(-0.5f, 0.5f, 0.5f);
              glVertex3f(-0.5f, -0.5f, 0.5f);

              // right
              glColor3f(0.0f, 0.0f, 1.0f);
              glNormal3f(1.0f, 0.0f, 0.0f);
              glVertex3f(0.5f, 0.5f, -0.5f);
              glVertex3f(0.5f, 0.5f, 0.5f);
              glVertex3f(0.5f, -0.5f, 0.5f);
              glVertex3f(0.5f, -0.5f, -0.5f);


              // bottom
              glColor3f(1.0f, 0.0f, 0.0f);
              glNormal3f(0.0f, 1.0f, 0.0f);
              glVertex3f(-0.5f,- 0.5f, 0.5f);
              glVertex3f(0.5f, -0.5f, 0.5f);
              glVertex3f(0.5f, -0.5f, -0.5f);
              glVertex3f(-0.5f,- 0.5f, -0.5f);

              // Back
              glColor3f(0.0f, 1.0f, 0.0f);
              glNormal3f(0.0f, 0.0f, 1.0f);
              glVertex3f(0.5f, -0.5f, -0.5f);
              glVertex3f(0.5f, 0.5f, -0.5f);
              glVertex3f(-0.5f, 0.5f, -0.5f);
              glVertex3f(-0.5f, -0.5f, -0.5f);


              // Left
              glColor3f(0.0f, 0.0f, 1.0f);
              glNormal3f(1.0f, 0.0f, 0.0f);
              glVertex3f(-0.5f, 0.5f, -0.5f);
              glVertex3f(-0.5f, 0.5f, 0.5f);
              glVertex3f(-0.5f, -0.5f, 0.5f);
              glVertex3f(-0.5f, -0.5f, -0.5f);
              glEnd();

              glPopMatrix();
          }

       }
   }


*/
/*
       if (!Castel) return;

        Castle const& c = *Castel;
        Voxel* voxel;

        Voxel* lastVoxel;
        QVector3D lastSize;
        QVector3D camDir = cameraLookDirection();
        for (quint32 k = 0; k < c.z(); ++k)
        {

            for (quint32 j = 0; j < c.y(); ++j)
            {

                for (quint32 i = 0; i < c.x();  i += voxel->__Size.x() - voxel->Size.x() + 1)
                {
                    voxel = Castel->get(i, j, k);
                    if ( !voxel->isVisible || !voxel->isValid())
                        continue;

                    if((i > 0 && i < c.x()) &&
                       (j > 0 && j < c.y()) &&
                       (k > 0 && k < c.z()) &&
                       (c(i+1,j,k).isValid() && c(i-1,j,k).isValid()) &&
                       (c(i,j+1,k).isValid() && c(i,j-1,k).isValid()) &&
                       (c(i,j,k+1).isValid() && c(i,j,k-1).isValid()))
                                        continue;

                   // Rendu d'un voxel
                    if (isVoxelHidden(i, j, k, camDir))
                         continue;

                 // Rendu effectif
                   // On crée un vecteur pour la translation

                 // On récupère le PrefabBase associé au voxel
                   PrefabBase* prefab_base = PrefabBase::PrefabMap[c(i,j,k).prefabID()];
                   QVector<QVector3D> const& _vertices = prefab_base->Vertices;
                   QVector<QVector3D> const& _normals = prefab_base->Normals;
                   QVector<FACE> const& _faces = prefab_base->Faces;

                   QVector3D scale = QVector3D(voxel->__Size.x(), voxel->__Size.y(), voxel->__Size.z());
                   QVector3D position = QVector3D();
                   position.setX(  i - c.x() / 2.0f+ (voxel->__Size. x() - voxel->Size.x())/ 2.0f );
                   position.setY(  j - c.y() / 2.0f+ (voxel->__Size.y() - voxel->Size.y())/ 2.0f );
                   position.setZ(  k - c.z() / 2.0f+ (voxel->__Size.z() - voxel->Size.z()) / 2.0f);
                   glPushMatrix();
                   glScalef( scale.x(),scale.y(),scale.z());
                   glBegin(GL_QUADS);
                   for (quint32 f = 0; f < _faces.size(); ++f)
                   {


                       for (uint v = 0; v < 3; ++v)
                       {
                           QVector3D const& V = _vertices[_faces[f].Vertices[v]];
                           QVector3D const& N = _normals[_faces[f].Normals[v]];

                           glNormal3f(N.x(), N.y(), N.z());

                           glVertex3f(V.x() + i, V.y() + j, V.z()  +k);

                       }

                    }
                    glEnd();
                   glPopMatrix();


                }
            }
        }
*/




}

void Renderer::nextShaderProgram()
{
    if (shaderProgramIndex == -1)
        return;

    ++shaderProgramIndex;

    if (shaderProgramIndex == Programs.size())
        shaderProgramIndex = 0;

    wi->update();
}

void Renderer::nextCastle(quint8 xRatio, quint8 yRatio)
{
    if (Castel != nullptr)
        delete Castel;
    Castel = new Castle(xRatio*(80+3), 100,yRatio*(80+3));
    Castel->CreateCastle(Castel,xRatio,yRatio);
    updateCastleStructure();

    //cubeScaling_Optimisation_X();
    //cubeScaling_Optimisation_Y();
    //cubeScaling_Optimisation_Z();
}

void Renderer::initShaders()
{
    // Let's explore the "programs" folder
    QDir programs("./programs");
    if (!programs.exists())
    {
        return;
    }
    foreach (QFileInfo const& finfo, programs.entryInfoList())
    {
        // Only deals with subdirectories
        if (finfo.isDir() && finfo.filePath() != "./programs/." && finfo.filePath() != "./programs/..")
        {
            // This is the processing for ONE program

            // Checking for vsh and fsh files to be present
            QFileInfo finfo_vsh(finfo.absoluteFilePath() + "/vertex.vsh");
            QFileInfo finfo_fsh(finfo.absoluteFilePath() + "/fragment.fsh");
            if (!finfo_vsh.exists() || !finfo_fsh.exists())
                break;

            // Creating the program
            QOpenGLShaderProgram *newprog = new QOpenGLShaderProgram;

            // Creating and compiling the Vertex Shader
            QOpenGLShader *vsh = new QOpenGLShader(QOpenGLShader::Vertex);
            if (!vsh->compileSourceFile(finfo_vsh.absoluteFilePath()))
            {
                QMessageBox::critical(0, QString("Erreur de compilation GLSL"),
                                      QString(u8"Le Vertex Shader du programme \"%1\" ne s'est pas compilé correctement.\nVoici le log :\n%2")
                                      .arg(finfo.fileName()).arg(vsh->log()));
                qApp->exit(1);
            }
            // Adding the Vertex Shader
            if (!newprog->addShader(vsh))
            {
                QMessageBox::critical(0, QString("Erreur d'ajout de shader"),
                                      QString(u8"Le Vertex Shader du programme \"%1\" n'a pas pu être ajouté.\nVoici le log :\n%2")
                                      .arg(finfo.fileName()).arg(vsh->log()));
                qApp->exit(1);
            }

            // Creating and compiling the Fragment Shader
            QOpenGLShader *fsh = new QOpenGLShader(QOpenGLShader::Fragment);
            if (!fsh->compileSourceFile(finfo_fsh.absoluteFilePath()))
            {
                QMessageBox::critical(0, QString("Erreur de compilation GLSL"),
                                      QString(u8"Le Fragment Shader du programme \"%1\" ne s'est pas compilé correctement.\nVoici le log :\n%2")
                                      .arg(finfo.fileName()).arg(fsh->log()));
                qApp->exit(1);
            }
            // Adding the Fragment Shader
            if (!newprog->addShader(fsh))
            {
                QMessageBox::critical(0, QString("Erreur d'ajout de shader"),
                                      QString(u8"Le Fragment Shader du programme \"%1\" n'a pas pu être ajouté.\nVoici le log :\n%2")
                                      .arg(finfo.fileName()).arg(fsh->log()));
                qApp->exit(1);
            }

            // Linking the program
            if (!newprog->link())
            {
                QMessageBox::critical(0, QString(u8"Erreur d'édition de liens des shaders"),
                                      QString(u8"Les shaders du programme \"%1\" n'ont pu être linkés. Voici le log :\n%2")
                                      .arg(finfo.fileName()).arg(newprog->log()));
                qApp->exit(1);
            }

            // Adding the program to the list
            Programs << newprog;
        }
    }

    // If there's at least one shader program, select the first
    if (Programs.size() != 0)
        shaderProgramIndex = 0;
}

void Renderer::updateCastleStructure()
{
    // Efface tous les tampons et les recrée
    Vertices.clear();
    Normals.clear();
    UVs.clear();
    Faces.clear();


    // Référence sur le château en cours
    Q_ASSERT(Castel != nullptr);
    Castle const& c = const_cast<Castle const&>(*Castel);



    // Prépare les optimisations
    QMap<QVector3D, int>* optVertices = new QMap<QVector3D, int>();
    QMap<QVector3D, int>* optNormals = new QMap<QVector3D, int>();
    QMap<FACE, int>* optFaces = new QMap<FACE, int>();
    QList<int>* optFacesToDel = new QList<int>();





    // On itère sur les voxels du château
    for (uint j = 0; j < c.y(); ++j)
    {
        for (uint k = 0; k < c.z(); ++k)
        {
            for (uint i = 0; i < c.x(); ++i)
            {
                // Si le voxel est invalide, on ne le dessine pas
                if (!c(i,j,k).isValid()) continue;

                //if(c(i,j,k).prefabID()=="cube") continue;

                // On crée un vecteur pour la translation
                QVector3D translation(i, j, k);

                // Optimisation : on enleve les voxels caché par les autres voxels
                if((i > 0 && i < c.x()) &&
                   (j > 0 && j < c.y()) &&
                   (k > 0 && k < c.z()) &&
                   (c(i+1,j,k).isValid() && c(i-1,j,k).isValid()) &&
                   (c(i,j+1,k).isValid() && c(i,j-1,k).isValid()) &&
                   (c(i,j,k+1).isValid() && c(i,j,k-1).isValid()))
                    continue;
                // On récupère le PrefabBase associé au voxel

                PrefabBase* prefab_base = PrefabBase::PrefabMap[c(i,j,k).prefabID()];
                // Enregistre l'offset de base pour les Vertex/Normals/UVs
                int vertOffset = Vertices.size();
                int normOffset = Normals.size();
                int textOffset = UVs.size();
                int faceOffset = Faces.size();
                // On fout tout du PrefabBase dans nos tableaux
                foreach (QVector3D const& v, prefab_base->Vertices)
                {
                    Vertices << v + translation;
                }
                foreach (QVector3D const& n, prefab_base->Normals)
                {
                    Normals << n;
                }
                foreach (QVector2D const& t, prefab_base->UVs)
                {
                    UVs << t;
                }
                foreach (FACE const& f, prefab_base->Faces)
                {

                    FACE tmpface = f;
                    for (uint i = 0; i < 3; ++i)
                        tmpface.Vertices[i] += vertOffset;
                    for (uint i = 0; i < 3; ++i)
                        tmpface.Normals[i] += normOffset;
                    for (uint i = 0; i < 3; ++i)
                        tmpface.UV[i] += textOffset;

                    Faces << tmpface;
                }


            }
        }
    }


    // On libère la mémoire des optimisations
    delete optVertices;
    delete optNormals;
    delete optFaces;
    delete optFacesToDel;

    int counter = 0;
    /*for (quint32 i = 0; i < Castel->x()*Castel->y()*Castel->z(); ++i)
    {
        if ((*Castel)(i).prefabID() == "cube")
            ++counter;
    }*/
    qDebug(u8"Génération terminée.");
}

void Renderer::initBO()
{
    VBO.create();
    VBO.allocate(16 * 3 * sizeof(float));
    VBO.write(0, PrefabBase::PrefabMap["cube"]->Vertices.constData(),8 * 3 * sizeof(float));
    VBO.write(8 * 3 * sizeof(float), PrefabBase::PrefabMap["cube"]->Normals.constData(),8 * 3 * sizeof(float));

    IBO.create();
    IBO.allocate(12 * 3 * sizeof(quint32));
    IBO.write(0, PrefabBase::PrefabMap["cube"]->Faces.constData(), 12 * 3 * sizeof(quint32));
}


inline QVector3D Renderer::cameraLookDirection()
{
    return
        (float)qSin(Heading*PI_OVER_180) * V_LEFT +
        (float)qSin(LookUpDown*PI_OVER_180) * V_UP +
        (float)qCos(Heading*PI_OVER_180) * V_FORWARD;
}

inline bool Renderer::isVoxelHidden(quint32 x, quint32 y, quint32 z, QVector3D const& direction)
{
    Castle const& c = *Castel;
    return
            (direction.x() < 0 || (x > 0 && c(x-1,y,z).isValid())) &&       // Si la cam regarde en +X
            (direction.y() < 0 || (y > 0 && c(x,y-1,z).isValid())) &&
            (direction.z() < 0 || (z > 0 && c(x,y,z-1).isValid())) &&
            (direction.x() > 0 || (x < c.x()-1 && c(x+1,y,z).isValid())) &&
            (direction.y() > 0 || (y < c.y()-1 && c(x,y+1,z).isValid())) &&
            (direction.z() > 0 || (z < c.z()-1 && c(x,y,z+1).isValid()));
}
void Renderer::cubeScaling_Optimisation_X()
{
    Voxel* lastVoxel;
    QVector3D lastSize;

    for (int z = 0; z < Castel->z(); z++)
            for (int y = 0; y < Castel->y(); y++)
            {
                lastVoxel = 0;

                for (int x = 0; x < Castel->x(); x++)
                {
                    Voxel* voxel = Castel->get(x, y, z);

                    if (lastVoxel && voxel->prefabID() == lastVoxel->prefabID() && voxel->Size == lastSize )
                    {
                        voxel->isVisible = false;
                        lastVoxel->__Size.setX( lastVoxel->__Size.x()+1);
                    }
                    else
                    {
                        voxel->isVisible = true;
                        lastVoxel = voxel;
                        lastSize = lastVoxel->__Size = lastVoxel->Size;
                    }
                }
            }
}

void Renderer::cubeScaling_Optimisation_Y()
{
    Voxel* lastVoxel;
    QVector3D lastSize;

    for (int x = 0; x < Castel->x(); x++)
            for (int z = 0; z < Castel->z(); z++)
            {
                lastVoxel = 0;

                for (int y = 0; y < Castel->y(); y++)
                {
                    Voxel* voxel = Castel->get(x, y, z);

                    if (lastVoxel && voxel->prefabID() == lastVoxel->prefabID() && voxel->Size == lastSize )
                    {
                        voxel->isVisible = false;
                        lastVoxel->__Size.setY( lastVoxel->__Size.y()+1);
                    }
                    else
                    {
                        voxel->isVisible = true;
                        lastVoxel = voxel;
                        lastSize = lastVoxel->__Size = lastVoxel->Size;
                    }
                }
            }
}
void Renderer::cubeScaling_Optimisation_Z()
{
    Voxel* lastVoxel;
    QVector3D lastSize;

    for (int x = 0; x < Castel->x(); x++)
            for (int y = 0; y < Castel->y(); y++)
            {
                lastVoxel = 0;

                for (int z = 0; z < Castel->z(); z++)
                {
                    Voxel* voxel = Castel->get(x, y, z);

                    if (lastVoxel && voxel->prefabID() == lastVoxel->prefabID() && voxel->Size == lastSize )
                    {
                        voxel->isVisible = false;
                        lastVoxel->__Size.setZ( lastVoxel->__Size.z()+1);
                    }
                    else
                    {
                        voxel->isVisible = true;
                        lastVoxel = voxel;
                        lastSize = lastVoxel->__Size = lastVoxel->Size;
                    }
                }
            }
}
