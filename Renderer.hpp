#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <QVector>
#include <QVector2D>
#include <QVector3D>
#include <QVector4D>
#include <QList>
#include <QMap>
#include <QSet>
#include <QOpenGLFunctions>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include "Castle.hpp"
#include "OBJLoader.hpp"

#define rdi Renderer::instance()

class Renderer : protected QOpenGLFunctions
{
    public:
        // Singleton
        ~Renderer();
        static Renderer* instance();

        // Data
        QVector<QVector3D> Vertices;
        QVector<QVector3D> Normals;
        QVector<QVector2D> UVs;
        QVector<FACE> Faces;

        Castle* Castel = nullptr;

        // Camera
        QMatrix4x4 ModelView;
        QMatrix4x4 Projection;
        float Heading = 0.0f;
        float LookUpDown = 0.0f;
        QVector3D Position;

        // BO
        QOpenGLBuffer VBO = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
        QOpenGLBuffer IBO = QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);

        // Shaders
        QList<QOpenGLShaderProgram*> Programs;
        int shaderProgramIndex = -1;
        QOpenGLShaderProgram* program() const;

        // Methods
        void load();
        void aspectRatio(int w, int h);
        void render();

        void nextShaderProgram();       
        void nextCastle(quint8 xRatio, quint8 yRatio);

    private:
        // Singleton
        Renderer();
        static Renderer* Instance;

        // Internal methods
        void initShaders();

        void cubeScaling_Optimisation_X();
        void cubeScaling_Optimisation_Y();
        void cubeScaling_Optimisation_Z();


        void updateCastleStructure();
        void initBO();

        inline QVector3D cameraLookDirection();
        inline bool isVoxelHidden(quint32 x, quint32 y, quint32 z, const QVector3D& direction);


};

#endif // RENDERER_HPP
