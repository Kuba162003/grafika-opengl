#include "allmodels.h"
#include <iostream>

Models::ObjModel ModelResources::motyl;
Models::ObjModel ModelResources::lodyga;
Models::ObjModel ModelResources::zalaznia;
Models::ObjModel ModelResources::platki;
Models::ObjModel ModelResources::lewe_skrzydlo;
Models::ObjModel ModelResources::prawe_skrzydlo;
Models::ObjModel ModelResources::lewe_male;
Models::ObjModel ModelResources::prawe_male;



//Models::ObjModel ModelResources::apple("C:/Users/Tomasz/Downloads/g3d_st_04_win/g3d_st_04_win/g3d_st_04_win/x64/Debug/apple.obj");


//TexturedModel ModelResources::tile; // Dodajemy obiekt


void loadModels() {
    // Tu mo¿esz dodaæ inne modele jeœli chcesz...
    //ModelResources::tile.readFromOBJ("C:/Users/Tomasz/Downloads/g3d_st_04_win/g3d_st_04_win/g3d_st_04_win/x64/Debug/models/tile.obj");         // Prosty model kafelka
    //ModelResources::tile.loadTexture("C:/Users/Tomasz/Downloads/g3d_st_04_win/g3d_st_04_win/g3d_st_04_win/x64/Debug/textures/tile_texture.jpg"); // Tekstura pod³ogi
    ModelResources::motyl.load("Motyl.obj");
    ModelResources::lodyga.load("lodyga.obj");
    ModelResources::zalaznia.load("zalaznia.obj");
    ModelResources::platki.load("platki.obj");
    ModelResources::lewe_skrzydlo.load("lewe_skrzydlo.obj");
    ModelResources::prawe_skrzydlo.load("prawe_skrzydlo.obj");
    ModelResources::lewe_male.load("lewe_male.obj");
    ModelResources::prawe_male.load("prawe_male.obj");
    //std::cout << "APPLE vertexCount = " << ModelResources::apple.vertexCount << std::endl;

}

void cleanupModels() {
    //ModelResources::tile.cleanup();
}

