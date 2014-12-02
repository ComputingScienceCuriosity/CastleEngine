#ifndef PREFAB_HPP
#define PREFAB_HPP

#include <QVector>
#include <QVector3D>

#include "Voxel.hpp"

#define mP Prefab::makePrefab

class Prefab
{
    public:
        // Constructeurs, destructeur et affectation
        Prefab(Prefab const& other) = default;
        ~Prefab() = default;
        Prefab& operator=(Prefab const& other) = default;
        // Les opérateurs
        Voxel const& operator() (quint32 x, quint32 y, quint32 z) const;
        Voxel& operator() (quint32 x, quint32 y, quint32 z);
        Voxel const& operator() (quint32 index) const;
        Voxel& operator() (quint32 index);
        quint32 x() const;
        quint32 y() const;
        quint32 z() const;

        static Prefab makePrefab(QString const& name = "",
                                 qint32 arg1 = -1, qint32 arg2 = -1, qint32 arg3 = -1, qint32 arg4 = -1);
    private:
        QString Name;
        QVector<Voxel> Voxels;
        quint32 X, Y, Z;
        // Le constructeur de base reste privé
        Prefab(QString const& Name, quint32 X, quint32 Y, quint32 Z);
        // Les "statiques privées" des PREFABS
        // Les basiques
        static Prefab makeEmpty();

        static Prefab makeWall(quint32 Width, quint32 Height, quint32 Thickness = 1);
        static Prefab makeCrazyWall(quint32 Width, quint32 Height, quint32 Thickness = 1, quint32 Distance = 4);
        static Prefab makeCrazyWall_rotated(quint32 Width, quint32 Height, quint32 Thickness = 1, quint32 Distance = 4);
        static Prefab makeWall_corner(quint32 Width, quint32 Height, quint32 Thickness = 1, quint32 Distance = 4);
        static Prefab makeWall_corner_Rotated90(quint32 Width, quint32 Height, quint32 Thickness = 1, quint32 Distance = 4);
        static Prefab makeTopWall(quint32 Width, quint32 Height, quint32 Thickness = 1);
        static Prefab makeWallRotated90(quint32 Width, quint32 Height, quint32 Thickness = 1);


        static Prefab makeTubeTour(quint32 Height);
        static Prefab makeTubeTour_2(quint32 Height);
        static Prefab makeTour(quint32 Width, quint32 Height, quint32 Thickness = 1);
        static Prefab makeTopTour(quint32 Width);
        // Les composés
};

#endif // PREFAB_HPP
