#ifndef CASTLE_HPP
#define CASTLE_HPP

#include <QVector>
#include "Prefab.hpp"

#define inv castle->invokePrefab

class Castle
{
    public:
        Castle* Castel = nullptr;


        Castle(quint32 X, quint32 Y, quint32 Z);
        Voxel const& operator() (quint32 x, quint32 y, quint32 z) const;
        Voxel* get (quint32 x, quint32 y, quint32 z);
        Voxel& operator() (quint32 x, quint32 y, quint32 z);
        Voxel const& operator() (quint32 index) const;
        Voxel& operator() (quint32 index);
        quint32 x() const;
        quint32 y() const;
        quint32 z() const;

        void invokePrefab(Prefab const& prefab, QVector3D const& position);
        void invokePrefab(Prefab const& prefab, quint32 X = 0, quint32 Y = 0, quint32 Z = 0);
        // ObjectsRatio représente le nombre de tours dans l'axe des 'x'et 'z'
        void CreateCastle(Castle *castle, quint8 ObjectsRatio_X, quint8 ObjectsRatio_Z);
    private:
        quint32 X, Y, Z;
        QVector<Voxel> Voxels;
};



#endif // CASTLE_HPP
