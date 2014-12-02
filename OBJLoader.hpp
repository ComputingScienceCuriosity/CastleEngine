#ifndef OBJLOADER_HPP
#define OBJLOADER_HPP


#include <GL/glu.h>
#include <QString>
#include <QVector3D>
#include <QVector2D>
#include <QVector>
#include <QImage>
#include <QFile>


#define RENDERFLAGS_LIGHTING	1L
#define RENDERFLAGS_FOG			1L<<1


struct IMAGE_DATA
{
    QImage *image = nullptr;                            // Texture diffuse
    GLuint *id = nullptr;                               // ID de la texture

    IMAGE_DATA(QString const& file)
    {
        QFile fin(file);
        if (!fin.exists())
        {
            qDebug(QString("IMAGE_DATA::IMAGE_DATA(QString const&) : Le fichier n'existe pas : %1").arg(file).toStdString().c_str());
        }
        else
        {
            image = new QImage(file);
            if (image->isNull())
            {
                qDebug("IMAGE_DATA::IMAGE_DATA(QString const&) : L'image ne se charge pas.");
                delete image;
                image = nullptr;
            }
        }
    }
    ~IMAGE_DATA()
    {
        if (image) delete image;
        if (id) delete id;
    }
};

struct MATERIAL
{
    QString Name = "";                                  // Material's name
    float Ambient[4] = {0.f, 0.f, 0.f, 0.f};            // Ambient coefficients
    float Diffuse[4] = {0.f, 0.f, 0.f, 0.f};            // Diffuse coefficients
    float Specular[4] = {0.f, 0.f, 0.f, 0.f};           // Specular coefficients
    float Emission[4] = {0.f, 0.f, 0.f, 0.f};           // Emission coefficients
    float Shininess = 0.f;                              // Specular shininess exponent
    QString TextureFile = "";                           // Diffuse texture
    IMAGE_DATA *pDiffuse = nullptr;                     // Diffuse texture data
    quint32 RenderFlags = 0u;                           // Render mode
};

typedef QVector3D VERTEX;                               // Vertex position

typedef QVector2D TEXTURE_COORDS;                       // Vertex'texture coordinates

struct FACE
{
    quint32 Vertices[3];                                // Vertex'indices
    quint32 UV[3];                                      // Texture coordinates indices
    quint32 Normals[3];                                 // Normals indices
};

struct OBJECT
{
    quint32 Material = 0xFFFFFFFF;                                          // Material index in global materials table
    quint32 FirstFace = 0u;                                                 // First face index
    quint32 FacesCount = 0u;                                                // Total number of faces for object
    float Matrix[16] = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};    // Transformation matrix
    VERTEX Center = VERTEX(0, 0, 0);                                        // Center's coordinates
};

//struct SCENE
//{
//    QVector<OBJECT> Objects = QVector<OBJECT>();                // Objects list
//    quint32 ObjectsCount = 0;                                   // Total objects count

//    QVector<VERTEX> Vertices = QVector<VERTEX>();               // Global vertices table
//    quint32 VerticesCount = 0;                                  // Total number of vertices
		
//    QVector<VERTEX> Normals = QVector<VERTEX>();                // Global normals table
//    quint32 NormalsCount = 0;                                   // Total number of normals

//    QVector<TEXTURE_COORDS> UV = QVector<TEXTURE_COORDS>();     // Global UV table
//    quint32 UVCount = 0;                                        // Total number of UV pairs

//    QVector<FACE> Faces = QVector<FACE>();                      // Faces'indices table
//    quint32 FacesCount = 0;                                     // Total number of faces

//    QVector<MATERIAL> Materials = QVector<MATERIAL>();          // Global materials table
//    quint32 MaterialsCount = 0;                                 // Total number of materials
//};

struct SCENE
{
    OBJECT *Objects = nullptr;                             // Objects list
    unsigned int ObjectsCount = 0;                          // Total objects count

    VERTEX *Vertices = nullptr;                            // Global vertices table
    unsigned int VerticesCount = 0;                         // Total number of vertices

    VERTEX *Normals = nullptr;                             // Global normals table
    unsigned int NormalsCount = 0;                          // Total number of normals

    TEXTURE_COORDS *UV = nullptr;                          // Global UV table
    unsigned int UVCount = 0;                               // Total number of UV pairs

    FACE *Faces = nullptr;                                 // Faces'indices table
    unsigned int FacesCount = 0;							// Total number of faces

    MATERIAL *Materials = nullptr;                         // Global materials table
    unsigned int MaterialsCount = 0;						// Total number of materials
};

//*********************************************************************************

SCENE *ReadOBJFile(QString const& FileName, bool GenerateMissingNormals = false);
void ReleaseScene(SCENE *pScene);

#endif // OBJLOADER_H
