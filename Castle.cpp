#include "Castle.hpp"
#include "Prefab.hpp"
#include "CastleRessources.hpp"


Castle::Castle(quint32 X, quint32 Y, quint32 Z) :
    X(X), Y(Y), Z(Z),
    Voxels(X*Y*Z)
{
    Voxels.squeeze();
}

const Voxel& Castle::operator()(quint32 x, quint32 y, quint32 z) const
{
    return Voxels[y * Z * X + z * X + x];
}
Voxel* Castle::get(quint32 x, quint32 y, quint32 z)
{
    return &Voxels[y * Z * X + z * X + x];
}

Voxel& Castle::operator()(quint32 x, quint32 y, quint32 z)
{
    return Voxels[y * Z * X + z * X + x];
}

const Voxel&Castle::operator()(quint32 index) const
{
    return Voxels[index];
}

Voxel&Castle::operator()(quint32 index)
{
    return Voxels[index];
}

quint32 Castle::x() const
{
    return X;
}

quint32 Castle::y() const
{
    return Y;
}

quint32 Castle::z() const
{
    return Z;
}

void Castle::invokePrefab(const Prefab& prefab, QVector3D const& position)
{
    invokePrefab(prefab, position.x(), position.y(), position.z());
}

void Castle::invokePrefab(const Prefab& prefab, quint32 X, quint32 Y, quint32 Z)
{
    for (uint j = 0; j < prefab.y(); ++j)
    {
        for (uint k = 0; k < prefab.z(); ++k)
        {
            for (uint i = 0; i < prefab.x(); ++i)
            {
                (*this)(i+X, j+Y, k+Z) = prefab(i, j, k);
            }
        }
     }
}

void Castle::CreateCastle(Castle *castle,quint8 ObjectsRatio_X,quint8 ObjectsRatio_Z)
{
    Prefab wallCornerRotated = mP("wall_corner_rotated",ui_tour_X-2,4,ui_wall_Z,4);

    Prefab wallCorner   = mP("wall_corner",ui_tour_X-2,4,ui_wall_Z,4);

    Prefab wallCrazy    = mP("wallCrazy",ui_wall_X,ui_wall_Y,ui_wall_Z,4);

    Prefab wallCrazy_rotated = mP("wallCrazy_rotated",ui_wall_X+4,ui_wall_Y,ui_wall_Z,4);

    Prefab wallWithTop  = mP("wallWithTop",ui_wall_X,ui_wall_Y,ui_wall_Z);
   // Invoquation du mur rotation = 90deg : Z = Largeur
    Prefab wallRotated  = mP("wallRotated",1,ui_wallRotated_Y,ui_wallRotated_Z);
    // Invoquation de la tour
    Prefab tour         = mP("tour",ui_tour_X,ui_tour_Y);
    // Invoquation de la tour tube
    Prefab tubeTour     = mP("tubeTour",ui_tubeTour_Y);

    Prefab tubeTour_2   = mP("tubeTour_2",ui_tubeTour_2_Y);

    Prefab topTour      = mP("topTour",ui_topTour_X);
    // ------------------------------------------------DESSIN DU CASTLE ------------------------------------------------------
    // Partie Post Interne :
    //---------------------------------------------------------------------------------------------------------------
    //arriere gauche
    inv(tubeTour_2,2+(ui_tour_X)+8,ui_CastlePrefabs_Y,2+(ui_tour_X)+8);
    inv(wallCorner,2+4, 8,2+(ui_tour_X)+5);
    inv(wallCornerRotated,2+ui_tour_X+6, 8,2+4);
    //arriere droite
    inv(tubeTour_2,2+((ui_T_W)*ObjectsRatio_X)- 8,ui_CastlePrefabs_Y,2+(ui_tour_X)+8);
    inv(wallCorner,2+((ui_T_W)*ObjectsRatio_X)-1, 8,2+(ui_tour_X)+5);
    inv(wallCornerRotated,2+((ui_T_W)*ObjectsRatio_X)-(ui_tour_X)+3, 8,2+4);
    //avant gauche

    inv(tubeTour_2,2+(ui_tour_X)+8,ui_CastlePrefabs_Y,2+((ui_T_W)*ObjectsRatio_Z)-8);
    inv(wallCorner,2+4, 8,2+((ui_T_W)*ObjectsRatio_Z)-((ui_tour_X) -4));
    inv(wallCornerRotated,2+ui_tour_X+6, 8,2+((ui_T_W)*ObjectsRatio_Z)-1);

    //avant droite
    inv(tubeTour_2,2+((ui_T_W)*ObjectsRatio_X)- 8,ui_CastlePrefabs_Y,2+((ui_T_W)*ObjectsRatio_Z)-8);
    inv(wallCorner,2+((ui_T_W)*ObjectsRatio_X)-1, 8,2+((ui_T_W)*ObjectsRatio_Z)-10);
    inv(wallCornerRotated,2+((ui_T_W)*ObjectsRatio_X)-(ui_tour_X)+3, 8,2+((ui_T_W)*ObjectsRatio_Z)-1);
   //---------------------------------------------------------------------------------------------------------------
    //***********************************************
    for(quint8 r = 0 ; r < ObjectsRatio_X; r++)
    {
        // - > ARCHITECTURE EXTERNE
        //Partie Arrière
        inv(tour,2+((ui_T_W)*r+1),ui_CastlePrefabs_Y,2);
        inv(topTour,2-1+((ui_T_W)*r+1),(ui_tour_Y),2-1);

        inv(wallWithTop,4+ui_tour_X+(ui_T_W)*r-1,ui_CastlePrefabs_Y,5);
        if(r == ObjectsRatio_X -1)
        {
            inv(tour,2+((ui_T_W)*ObjectsRatio_X),ui_CastlePrefabs_Y,2);
            inv(topTour,2-1+((ui_T_W)*ObjectsRatio_X),(ui_tour_Y),2-1);
        }
       //Partie Avant
        inv(tour,2+((ui_T_W)*r+1),ui_CastlePrefabs_Y,2+((ui_T_W)*ObjectsRatio_Z));
        inv(topTour,2-1+((ui_T_W)*r+1),(ui_tour_Y),2-1+((ui_T_W)*ObjectsRatio_Z));

        inv(wallWithTop,4+ui_tour_X+(ui_T_W)*r-1,ui_CastlePrefabs_Y,13+((ui_T_W)*ObjectsRatio_Z));
        if(r == ObjectsRatio_X -1)
        {
            inv(tour,2+((ui_T_W)*ObjectsRatio_X),ui_CastlePrefabs_Y,2+((ui_T_W)*ObjectsRatio_Z));
            inv(topTour,2-1+((ui_T_W)*ObjectsRatio_X),(ui_tour_Y),2-1+((ui_T_W)*ObjectsRatio_Z));
        }
        // F/-> ARCHITECTURE EXTERNE
        //----------------------------------------------------------------------------------------------------------------
        //  - > ARCHITECTURE INTERNE AVANT - ARRIERE

        // Partie INTERNE niveau 1 du chateau (Arrière):
        if((ObjectsRatio_X == 1) && (ObjectsRatio_Z == 1))
        {
            // Si on à qu'une seul tour a déssiné au milieu
            inv(tubeTour,2+(ui_tour_X+(ui_wall_X/2)),ui_CastlePrefabs_Y,2+(ui_tour_X+(ui_wall_X/2)));
            break;
        }
        else
        {
            // On dessine sur les X
            if(ObjectsRatio_X == 2 && ObjectsRatio_Z ==1)
            {
                // On dessine la partie Commune dans le centre
                inv(tubeTour,2+(ui_tour_X+(ui_wall_X/2))+((ui_T_W-3)*r-1),ui_CastlePrefabs_Y,2+(ui_tour_X+(ui_wall_X/2)));
                if(r == 0)
                    inv(wallCrazy,2+(ui_tour_X+(ui_wall_X/2)+6),ui_CastlePrefabs_Y,(ui_tour_X+(ui_wall_X/2)));
            }
            if(ObjectsRatio_X > 1 && ObjectsRatio_Z == 1)
            {
                // On dessine la partie Commune dans le centre
                inv(tubeTour,2+(ui_tour_X+(ui_wall_X/2))+((ui_T_W-3)*r-1),ui_CastlePrefabs_Y,2+(ui_tour_X+(ui_wall_X/2)));
                if(r < ObjectsRatio_X -1)
                    inv(wallCrazy,2+(ui_tour_X+(ui_wall_X/2)+6)+((ui_wall_X-1+12)*r-1),ui_CastlePrefabs_Y,(ui_tour_X+(ui_wall_X/2)));
            }
            // Plusieurs Tours et Plusieurs Murs ...
            else
            {
                // On dessine la partie d'arrière
                if(ObjectsRatio_X > 1 && ObjectsRatio_Z > 1){
                    inv(tubeTour,2+(ui_tour_X+(ui_wall_X/2))+((ui_T_W-3)*r-1),ui_CastlePrefabs_Y,2+(ui_tour_X+(ui_wall_X/2)));
                    if(r < ObjectsRatio_X -1)
                       inv(wallCrazy,2+(ui_tour_X+(ui_wall_X/2)+6)+((ui_wall_X-1+12)*r-1),ui_CastlePrefabs_Y,(ui_tour_X+(ui_wall_X/2)));
                // On dessine la partie d'avant
                    inv(tubeTour,2+(ui_tour_X+(ui_wall_X/2))+((ui_T_W-3)*r-1),ui_CastlePrefabs_Y,2+(((ui_T_W)*ObjectsRatio_Z)-(ui_wall_X/2)));
                    if(r < ObjectsRatio_X -1)
                        inv(wallCrazy,2+(ui_tour_X+(ui_wall_X/2)+6)+((ui_wall_X-1+12)*r-1),ui_CastlePrefabs_Y,((ui_T_W)*ObjectsRatio_Z)-((ui_wall_X/2)));
                }

            }
            // F- > ARCHITECTURE INTERNE
            //----------------------------------------------------------------------------------------------------------------
        }
    }


    for(quint8 v = 0 ; v < ObjectsRatio_Z; v++)
    {
        //Partie Gauche
        inv(wallRotated,5,ui_CastlePrefabs_Y,ui_tour_X+3+((ui_T_WR)*v));
        if(ObjectsRatio_Z>1 && v < ObjectsRatio_Z - 1)
        {
            inv(tour,2,ui_CastlePrefabs_Y,2+((ui_T_WR)+(ui_T_WR)*v));
            inv(topTour,2-1,(ui_tour_Y),2-1+((ui_T_WR)+(ui_T_WR)*v));
        }

        //Partie Droite
        inv(wallRotated,10+2+((ui_T_WR)*ObjectsRatio_X),ui_CastlePrefabs_Y,ui_tour_X+3+((ui_T_WR)*v));
        if(ObjectsRatio_Z>1 && v < ObjectsRatio_Z - 1)
        {
            inv(tour,2+((ui_T_WR)*ObjectsRatio_X),ui_CastlePrefabs_Y,2+((ui_T_WR)+(ui_T_WR)*v));
            inv(topTour,2-1+(((ui_T_WR)*ObjectsRatio_X)),(ui_tour_Y),2-1+((ui_T_WR)+(ui_T_WR)*v));
        }

        if(ObjectsRatio_Z == 2 && ObjectsRatio_X ==1)
        {
            // On dessine la partie Commune dans le centre
            inv(tubeTour,2+(ui_tour_X+(ui_wall_X/2)),ui_CastlePrefabs_Y,2+(ui_tour_X+(ui_wall_X/2))+((ui_T_W-3)*v-1));
            if(v == 0)
                inv(wallRotated,2+(ui_tour_X+(ui_wall_X/2)),ui_CastlePrefabs_Y,(ui_tour_X+(ui_wall_X/2)+6));
        }
        else
        if(ObjectsRatio_Z > 1 && ObjectsRatio_X == 1)
        {
            // On dessine la partie Commune dans le centre
            inv(tubeTour,2+(ui_tour_X+(ui_wall_X/2)),ui_CastlePrefabs_Y,2+(ui_tour_X+(ui_wall_X/2))+((ui_T_W-3)*v-1));
            if(v < ObjectsRatio_Z -1)
                inv(wallRotated,2+(ui_tour_X+(ui_wall_X/2)),ui_CastlePrefabs_Y,(ui_tour_X+(ui_wall_X/2)+6+2)+((ui_wall_X-1+12)*v-1));

        }
        // Plusieurs Tours et Plusieurs Murs ...
        else
        {
            // On dessine la partie d'arrière
            if(ObjectsRatio_Z > 1 && ObjectsRatio_X > 1){
                if(v < ObjectsRatio_Z -1){
                    inv(wallCrazy_rotated,2+(ui_tour_X+(ui_wall_X/2)-4),ui_CastlePrefabs_Y,2+(ui_tour_X+(ui_wall_X/2)+6)+((ui_wall_X-1+14)*v-1));
                    inv(wallCrazy_rotated,((ui_T_W)*ObjectsRatio_X)-((ui_wall_X/2)+4),ui_CastlePrefabs_Y,2+(ui_tour_X+(ui_wall_X/2)+6)+((ui_wall_X-1+14)*v-1));
                }
                if(ObjectsRatio_Z > 2){
                    if((v < ObjectsRatio_Z -1) && (v > 0)){
                        inv(tubeTour,2+(ui_tour_X+(ui_wall_X/2)),ui_CastlePrefabs_Y,2+(ui_tour_X+(ui_wall_X/2))+((ui_T_W-1)*v));
                        inv(tubeTour,((ui_T_W)*ObjectsRatio_X)-((ui_wall_X/2)+2),ui_CastlePrefabs_Y,2+(ui_tour_X+(ui_wall_X/2))+((ui_T_W-1)*v));
                    }
                }
            }
         }
    }
}

