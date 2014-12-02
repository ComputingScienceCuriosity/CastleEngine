#ifndef VOXEL_HPP
#define VOXEL_HPP

#include <QMatrix4x4>
#include <QVector3D>


class Voxel
{
    public:

        enum RotationFlags
        {
            ROTATION_0 = 0,
            ROTATION_90 = 1,
            ROTATION_180 = 2,
            ROTATION_270 = 3,
            ROTATION_X = 4,
            ROTATION_Y = 8,
            ROTATION_Z = 12
        };

        Voxel(QString const& PrefabID = "", quint8 Rotation = 0, QVector3D Size = {1,1,1});
        QString const& prefabID() const;
        quint8 rotation() const;
        bool isValid() const;
        bool isVisible;

        QVector3D  Size;
        QVector3D __Size;

        static constexpr quint8 VoxelSize = 1.f;


    private:
        QString PrefabID;
        quint8  Rotation;

};

#endif // VOXEL_HPP
