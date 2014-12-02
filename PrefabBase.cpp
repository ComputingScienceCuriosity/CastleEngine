#include "PrefabBase.hpp"
#include <QFileInfo>
#include <QDir>

QMap<QString, PrefabBase*> PrefabBase::PrefabMap = QMap<QString, PrefabBase*>();

PrefabBase::PrefabBase(const QString& path)
{
    // Read the scene
    SCENE* tempScene = ReadOBJFile(path);

    // Gather the vertices
    for (uint i = 0; i < tempScene->VerticesCount; ++i)
    {
        Vertices << tempScene->Vertices[i];
    }

    // Gather the normals
    for (uint i = 0; i < tempScene->NormalsCount; ++i)
    {
        Normals << tempScene->Normals[i];
    }

    // Gather the UV
    for (uint i = 0; i < tempScene->UVCount; ++i)
    {
        UVs << tempScene->UV[i];
    }

    // Gather the faces
    for (uint i = 0; i < tempScene->FacesCount; ++i)
    {
        Faces << tempScene->Faces[i];
    }

    // Gather the material
    Mat = tempScene->Materials[0];

    // Destroys the scene
    ReleaseScene(tempScene);
}

void PrefabBase::initializeList()
{
    // Let's explore the "prefabs" folder
    QDir prefabs("./prefabs");

    foreach (QFileInfo const& finfo, prefabs.entryInfoList())
    {
        // Concentrate on OBJ files
        if (finfo.isFile() && finfo.suffix().toLower() == "obj")
        {
            // Creates and adds a new prefab based on that file
            PrefabBase* newone = new PrefabBase(finfo.absoluteFilePath());
            newone->Name = finfo.baseName();
            PrefabMap.insert(finfo.baseName(), newone);
        }
    }
}
