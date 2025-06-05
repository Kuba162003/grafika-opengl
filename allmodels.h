/*
Niniejszy program jest wolnym oprogramowaniem; możesz go
rozprowadzać dalej i / lub modyfikować na warunkach Powszechnej
Licencji Publicznej GNU, wydanej przez Fundację Wolnego
Oprogramowania - według wersji 2 tej Licencji lub(według twojego
wyboru) którejś z późniejszych wersji.

Niniejszy program rozpowszechniany jest z nadzieją, iż będzie on
użyteczny - jednak BEZ JAKIEJKOLWIEK GWARANCJI, nawet domyślnej
gwarancji PRZYDATNOŚCI HANDLOWEJ albo PRZYDATNOŚCI DO OKREŚLONYCH
ZASTOSOWAŃ.W celu uzyskania bliższych informacji sięgnij do
Powszechnej Licencji Publicznej GNU.

Z pewnością wraz z niniejszym programem otrzymałeś też egzemplarz
Powszechnej Licencji Publicznej GNU(GNU General Public License);
jeśli nie - napisz do Free Software Foundation, Inc., 59 Temple
Place, Fifth Floor, Boston, MA  02110 - 1301  USA
*/

#ifndef ALL_MODELS_H
#define ALL_MODELS_H

#include "cube.h"
#include "sphere.h"
#include "teapot.h"
#include "torus.h"
//#include "texturedmodel.h"
#include "objmodel.h"

class ModelResources {
public:
    //static TexturedModel tile; // Dodane: nowy model kafelka
    static Models::ObjModel motyl;
    static Models::ObjModel lodyga;
    static Models::ObjModel zalaznia;
    static Models::ObjModel platki;
    static Models::ObjModel lewe_skrzydlo;
    static Models::ObjModel prawe_skrzydlo;
    static Models::ObjModel lewe_male;
    static Models::ObjModel prawe_male;
};

void loadModels();   // Funkcja inicjalizująca wszystkie modele
void cleanupModels();   // Czyszczenie


#endif