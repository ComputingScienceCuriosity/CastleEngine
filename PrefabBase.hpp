#ifndef PREFAB_BASE_HPP
#define PREFAB_BASE_HPP

#include <QVector>
#include <QVector2D>
#include <QVector3D>
#include <QMap>
#include "OBJLoader.hpp"

class PrefabBase
{
    public:
        // METHODS
        static void initializeList();

    public:
        // ATTRIBUTES
        QString Name;
        QVector<QVector3D> Vertices;
        QVector<QVector3D> Normals;
        QVector<QVector2D> UVs;
        QVector<FACE> Faces;
        MATERIAL Mat;

        static QMap<QString, PrefabBase*> PrefabMap;

    private:
        explicit PrefabBase(QString const& path);
};

#endif // PREFAB_BASE_HPP
