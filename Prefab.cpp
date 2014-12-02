#include "Prefab.hpp"
#include "Voxel.hpp"
#include <QMatrix4x4>


Prefab::Prefab(const QString& Name, quint32 X, quint32 Y, quint32 Z) :
    Name(Name),
    Voxels(X*Y*Z),
    X(X), Y(Y), Z(Z)
{
    Voxels.squeeze();
}

const Voxel& Prefab::operator()(quint32 x, quint32 y, quint32 z) const
{
    return Voxels[y * Z * X + z * X + x];
}

Voxel& Prefab::operator()(quint32 x, quint32 y, quint32 z)
{
    return Voxels[y * Z * X + z * X + x];
}

const Voxel& Prefab::operator()(quint32 index) const
{
    return Voxels[index];
}

Voxel& Prefab::operator()(quint32 index)
{
    return Voxels[index];
}

quint32 Prefab::x() const
{
    return X;
}

quint32 Prefab::y() const
{
    return Y;
}

quint32 Prefab::z() const
{
    return Z;
}

Prefab Prefab::makePrefab(const QString& name, qint32 arg1, qint32 arg2, qint32 arg3, qint32 arg4)
{
    if (name == "")
    {
        return Prefab::makeEmpty();
    }
    // Mur basic
    else if (name == "wall")
    {
        if (arg3 == -1)
            return Prefab::makeWall(arg1, arg2);
        else
            return Prefab::makeWall(arg1, arg2, arg3);
    }
    // Mur rotationné à 90 deg; A faire après : appliquer les transfo sur les prefabs directement
    else if(name == "wallRotated")
    {
        if(arg3 == -1)
            return Prefab::makeWallRotated90(arg1, arg2);
        else
            return Prefab::makeWallRotated90(arg1, arg2, arg3);
    }
    else if(name == "wall_corner_rotated")
    {
        if(arg3 == -1)
            return Prefab::makeWall_corner_Rotated90(arg1, arg2);
        else
            return Prefab::makeWall_corner_Rotated90(arg1, arg2, arg3);
    }
    // Mur avec les formes en haut
    else if(name == "wallWithTop")
    {
        if(arg3 == -1)
            return Prefab::makeTopWall(arg1, arg2);
        else
            return Prefab::makeTopWall(arg1, arg2, arg3);
    }
    else if(name == "wallCrazy")
    {
        if(arg3 == -1 || arg4 == -1)
            return Prefab::makeCrazyWall(arg1, arg2);
        else
            return Prefab::makeCrazyWall(arg1, arg2, arg3, arg4);
    }
    else if(name == "wallCrazy_rotated")
    {
        if(arg3 == -1 || arg4 == -1)
            return Prefab::makeCrazyWall_rotated(arg1, arg2);
        else
            return Prefab::makeCrazyWall_rotated(arg1, arg2, arg3, arg4);
    }
    else if(name == "wall_corner")
    {
        if(arg3 == -1 || arg4 == -1)
            return Prefab::makeWall_corner(arg1, arg2);
        else
            return Prefab::makeWall_corner(arg1, arg2, arg3, arg4);
    }
    // Tour sans toit
    else if(name == "tour")
    {
        if(arg3 == -1)
            return Prefab::makeTour(arg1, arg2);
        else
            return Prefab::makeTour(arg1, arg2, arg3);
    }
    // Tour avec toit
    else if(name == "topTour")
    {      
        return Prefab::makeTopTour(arg1);

    }
    // Tour de type tube
    else if(name == "tubeTour")
    {
        return makeTubeTour(arg1);
    }
    else if(name == "tubeTour_2")
    {
        return makeTubeTour_2(arg1);
    }
}

Prefab Prefab::makeEmpty()
{
    return Prefab("", 0, 0, 0);
}

Prefab Prefab::makeWall(quint32 Width, quint32 Height, quint32 Thickness)
{
    // On crée notre instance à renvoyer
    Prefab toret("wall", Width, Height, Thickness);

    // On boucle pour donner une position
    for (uint j = 0; j < Height; ++j)
    {
        for (uint k = 0; k < Thickness; ++k)
        {
            for (uint i = 0; i < Width; ++i)
            {
                toret(i, j, k) = Voxel("cube");
            }
        }
    }

    return toret;
}

Prefab Prefab::makeCrazyWall(quint32 Width, quint32 Height, quint32 Thickness, quint32 Distance)
{

    // On crée notre instance à renvoyer
    Prefab toret("wallCrazy", Width*2 , (Height+2)*2, (Thickness+1 )*Distance);

    // On boucle pour donner une position
    for (uint j = 0; j < Height; ++j)
    {
        for (uint k = 0; k < Thickness; ++k )
        {
            for (uint i = 0; i < Width; ++i)
            {
                if((j == Height -1) && ((i % 2*Voxel::VoxelSize == 0) || (i % 6 == 0)) ){
                    toret(i, j, k) = Voxel("cube");


                }else
                    if(j < Height -1)
                    toret(i, j, k) = Voxel("cube");

            }
        }
    }
    // On boucle pour déssiner le haut du mur
    for (uint i = 0; i < Width; i+=6 )
    {
        toret(i,Height, (Thickness-1)) = Voxel("cube");
    }

    // On boucle pour donner une position
    for (uint j = 0; j < Height; ++j)
    {
        for (uint k = 0; k < Thickness; ++k )
        {
            for (uint i = 0; i < Width; ++i)
            {
                if((j == Height -1) && ((i % 2*Voxel::VoxelSize == 0) || (i % 6 == 0)) ){
                    toret(i, j, k+Distance) = Voxel("cube");


                }else
                    if(j < Height -1)
                    toret(i, j, k+Distance) = Voxel("cube");

            }
        }
    }
    // On boucle pour déssiner le haut du mur
    for (uint i = 0; i < Width; i+=6)
    {
        toret(i,Height, (Thickness-1)+Distance) = Voxel("cube");
    }
    // On boucle pour dessiner la structure qui est entre les murs

        for (uint k = 0; k < (Distance-(Thickness*Voxel::VoxelSize)); ++k)
        {
            for (uint i = 0; i < Width; ++i)
            {
                toret(i, Height-5, k+Thickness) = Voxel("cube");
            }
        }

    return toret;



}

Prefab Prefab::makeCrazyWall_rotated(quint32 Width, quint32 Height, quint32 Thickness, quint32 Distance)
{

    // On crée notre instance à renvoyer
    Prefab toret("wallCrazy_rotated",(Thickness+1 )+Distance, (Height+2)*2, Width );

    // On boucle pour donner une position
    for (uint j = 0; j < Height; ++j)
    {
        for (uint k = 0; k < Thickness; ++k )
        {
            for (uint i = 0; i < Width; ++i)
            {
                if((j == Height -1) && ((i % 2*Voxel::VoxelSize == 0) || (i % 6 == 0)) ){
                    toret(k, j, i) = Voxel("cube");


                }else
                    if(j < Height -1)
                    toret(k, j,i) = Voxel("cube");

            }
        }
    }
    // On boucle pour déssiner le haut du mur
    for (uint i = 0; i < Width; i+=6 )
    {
        toret((Thickness-1),Height,i ) = Voxel("cube");
    }

    // On boucle pour donner une position
    for (uint j = 0; j < Height; ++j)
    {
        for (uint k = 0; k < Thickness; ++k )
        {
            for (uint i = 0; i < Width; ++i)
            {
                if((j == Height -1) && ((i % 2*Voxel::VoxelSize == 0) || (i % 6 == 0)) ){
                    toret(k+Distance, j, i) = Voxel("cube");


                }else
                    if(j < Height -1)
                    toret(k+Distance, j,i ) = Voxel("cube");

            }
        }
    }
    // On boucle pour déssiner le haut du mur
    for (uint i = 0; i < Width; i+=6)
    {
        toret((Thickness-1)+Distance,Height,i) = Voxel("cube");
    }
    // On boucle pour dessiner la structure qui est entre les murs

        for (uint k = 0; k < (Distance-(Thickness*Voxel::VoxelSize)); ++k)
        {
            for (uint i = 0; i < Width; ++i)
            {
                toret(k+Thickness, Height-5,i ) = Voxel("cube");
            }
        }

    return toret;



}

Prefab Prefab::makeWall_corner(quint32 Width, quint32 Height, quint32 Thickness, quint32 Distance)
{

    // On crée notre instance à renvoyer
    Prefab toret("wall_corner", Width +2 , (Height+Voxel::VoxelSize)*2, Thickness+Voxel::VoxelSize + Distance);

    // On boucle pour donner une position
    for (uint k = 0; k < Thickness; ++k )
    {
        for (uint i = 0; i < Width; ++i)
        {
            toret(i, Height -1, k) = Voxel("cube");
            toret(i, Height -1, k+Distance) = Voxel("cube");
        }
    }

    // On boucle pour déssiner le haut du mur
    for (uint i = 0; i < Width; i+= Voxel::VoxelSize * 2)
    {
        toret(i,Height, Thickness-Voxel::VoxelSize) = Voxel("cube");
        toret(i,Height, (Thickness-Voxel::VoxelSize)+Distance) = Voxel("cube");
    }


    for (uint k = 0; k < (Distance-(Thickness*Voxel::VoxelSize)); ++k)
    {
        for (uint i = 0; i < Width; ++i)
        {
            toret(i, Height-1, k+Thickness) = Voxel("cube");
        }
    }
    return toret;


}

Prefab Prefab::makeWall_corner_Rotated90(quint32 Width, quint32 Height, quint32 Thickness, quint32 Distance)
{

    // On crée notre instance à renvoyer
    Prefab toret("wall_corner_rotated", Thickness+Voxel::VoxelSize + Distance , (Height+Voxel::VoxelSize)*2, Width +2);
    // On boucle pour donner une position
    for (uint k = 0; k < Thickness; ++k )
    {
        for (uint i = 0; i < Width; ++i)
        {
            toret(k, Height -1, i) = Voxel("cube");
            toret(k+Distance, Height -1,i ) = Voxel("cube");
        }
    }

    // On boucle pour déssiner le haut du mur
    for (uint i = 0; i < Width; i+= Voxel::VoxelSize * 2)
    {
        toret( Thickness-Voxel::VoxelSize,Height,i) = Voxel("cube");
        toret((Thickness-Voxel::VoxelSize)+Distance,Height,i ) = Voxel("cube");
    }


    for (uint k = 0; k < (Distance-(Thickness*Voxel::VoxelSize)); ++k)
    {
        for (uint i = 0; i < Width; ++i)
        {
            toret(k+Thickness, Height-1, i ) = Voxel("cube");
        }
    }
    return toret;





}

Prefab Prefab::makeWallRotated90(quint32 Width, quint32 Height, quint32 Thickness)
{
    // On crée notre instance à renvoyer
    Prefab toret("wallRotated", Width, Height+1, Thickness+1);

    // On boucle pour donner une position
    for (uint j = 0; j < Height; ++j)
    {
        for (uint k = 0; k < Thickness; ++k )
        {
            for (uint i = 0; i < Width; ++i)
            {
                toret(0 , j, k) = Voxel("cube");
            }
        }
    }
    // On boucle pour déssiner le haut du mur
    for (uint i = 0; i < Thickness; i+= Voxel::VoxelSize*2)
    {
        toret(0 ,Height,i+1 ) = Voxel("cube");
    }
    return toret;
}


Prefab Prefab::makeTopWall(quint32 Width, quint32 Height, quint32 Thickness)
{
    // On crée notre instance à renvoyer
    Prefab toret("wallWithTop", Width , Height+Voxel::VoxelSize, Thickness );

    // On boucle pour donner une position
    for (uint j = 0; j < Height; ++j)
    {
        for (uint k = 0; k < Thickness; ++k )
        {
            for (uint i = 0; i < Width; ++i)
            {
                toret(i, j, k) = Voxel("cube");
            }
        }
    }
    // On boucle pour déssiner le haut du mur
    for (uint i = 0; i < Width; i+= Voxel::VoxelSize * 2)
    {
        toret(i+Voxel::VoxelSize,Height, Thickness-Voxel::VoxelSize) = Voxel("cube");
    }

    return toret;
}

/*Prefab Prefab::makeDoor(quint32 Width, quint32 Height, quint32 Thickness)
{




}*/

Prefab Prefab::makeTour(quint32 Width, quint32 Height, quint32 Thickness)
{
    // On crée notre instance à renvoyer
    Prefab toret("tour", Width  , Height + Height, Thickness + Width );
    // On boucle pour dessiner le mur d'avant
    for (uint j = 0; j < Height; ++j)
    {
        for (uint i = 0; i < Width; ++i)
        {

            if(
             (i < ((Width/2)- ((Width/4)/2)) || (i > ((Width/2)+ ((Width/4)/2)))) || ((j < ((Height/2)-((Height/5)/2))) || (j > ((Height/2)+((Height/5)/2)))))

                toret(i, j,0) = Voxel("cube");
        }
    }

    // On boucle pour dessiner le mur droite
    for(uint j = 0; j < Height;++j)
    {
        for (uint i = 0; i < Width; ++i)
        {
            if((i < ((Width/2)- ((Width/4)/2)) || (i > ((Width/2)+ ((Width/4)/2)))) || ((j < ((Height/2)+((Height/2)/2))) || (j > ((Height/2)+((Height/4)/2)*3)))   )

                toret(Width-1, j, i) = Voxel("cube");
        }
    }

    // On boucle pour dessiner le mur d'avant
    for (uint j = 0; j < Height; ++j)
    {
        for (uint i = 0; i < Width; ++i)
        {
            if((i < ((Width/2)- ((Width/4)/2)) || (i > ((Width/2)+ ((Width/4)/2)))) || ((j < ((Height/2)-((Height/5)/2))) || (j > ((Height/2)+((Height/5)/2))))   )

                toret(i, j, Width) = Voxel("cube");
        }
    }

    // On boucle pour dessiner le mur gauche
    for(uint j = 0; j < Height;++j)
    {
        for (uint i = 0; i < Width; ++i)
        {
            if((i < ((Width/2)- ((Width/4)/2)) || (i > ((Width/2)+ ((Width/4)/2)))) || ((j < ((Height/2)+((Height/2)/2))) || (j > ((Height/2)+((Height/4)/2)*3)))   )

                toret(0, j, i) = Voxel("cube");
        }
    }

   return toret;
}

Prefab Prefab::makeTubeTour(quint32 Height)
{
    // On crée notre instance à renvoyer
    Prefab toret("tubeTour", Voxel::VoxelSize  , Height+20 , Voxel::VoxelSize );
    // On boucle pour dessiner la tour en tube
    for (uint j = 0; j < Height; ++j)
    {
            toret(0, j,0) = Voxel("tube");
    }
    // On boucle pour dessiner le tube du toit
    for (uint j = 0; j < 3; ++j)
    {
            toret(0, Height+j,0) = Voxel("tubeTop");
    }
    // On dessine le toit
            toret(0, Height+4,0) = Voxel("cone");
            toret(0, Height+5,0) = Voxel("som");


    return toret;
}

Prefab Prefab::makeTubeTour_2(quint32 Height)
{
    // On crée notre instance à renvoyer
    Prefab toret("tubeTour_2", Voxel::VoxelSize  , Height+11 , Voxel::VoxelSize );
    // On boucle pour dessiner la tour en tube
    for (uint j = 0; j < Height; ++j)
    {
        if(j < (Height *1/4))
            if(j % 4 == 0)
                toret(0,j,0) = Voxel("tubeTop");
            else
                toret(0, j,0) = Voxel("tube");
        else
            if(j == Height -2)
                toret(0, j,0) = Voxel("tubeTop");

            else
                 toret(0, j,0) = Voxel("tube");
    }
    // On boucle pour dessiner le tube du toit
    toret(0, Height-1,0) = Voxel("cone_big_t");
    toret(0, Height+10,0) = Voxel("som");





    return toret;
}


Prefab Prefab::makeTopTour(quint32 Width)
{
    // On crée notre instance à renvoyer
    Prefab toret("topTour", Width + 2 , 2 , Width +2 ) ;
    //----------------------------------------
    // On boucle pour dessiner le mur d'arrière
    //----------------------------------------
    for (uint i = 0; i < Width; ++i)
    {
        toret(i, 0,0) = Voxel("cube");
    }

    for (uint i = 0; i < Width; i+=3)
    {
        toret(i, 1, 0) = Voxel("cube");
    }

    //----------------------------------------
    // On boucle pour dessiner le mur droite
    //----------------------------------------
    for (uint i = 0; i < Width; ++i)
    {
        toret(Width-1, 0, i) = Voxel("cube");
    }

    for (uint i = 0; i < Width; i+=3)
    {
        toret( Width-1 ,1, i) = Voxel("cube");
    }

    //----------------------------------------
    // On boucle pour dessiner le mur d'avant
    //----------------------------------------
    for (uint i = 0; i < Width; ++i)
    {
        toret(i, 0, Width) = Voxel("cube");
    }

    for (uint i = 0; i < Width; i+=3)
    {
        toret( i ,1, Width ) = Voxel("cube");
    }
    //----------------------------------------
    // On boucle pour dessiner le mur gauche
    //----------------------------------------
    for (uint k = 0; k < Width; ++k)
    {
        toret(0, 0, k) = Voxel("cube");

    }
    for (uint i = 0; i < Width; i+=3)
    {
        toret( 0 ,1, i) = Voxel("cube");
    }

     //----------------------------------------

    //------------------------------------------
    return toret;
}


