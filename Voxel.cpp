#include "Voxel.hpp"

Voxel::Voxel(QString const& PrefabID, quint8 Rotation, QVector3D Size) :
    PrefabID(PrefabID),
    Rotation(Rotation),
    Size(Size),
    __Size(Size),
    isVisible(true)
{

}

const QString& Voxel::prefabID() const
{
    return PrefabID;
}

quint8 Voxel::rotation() const
{
    return Rotation;
}

bool Voxel::isValid() const
{
    return (PrefabID != "");
}
