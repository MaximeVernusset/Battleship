#include "jeu.h"

using namespace std;


int main()
{
    //Initialisation
    srand(time(NULL));
    allegro_init();
    set_uformat(U_ASCII); //Pour les accents en Allegro
    (install_sound(DIGI_AUTODETECT, MIDI_NONE, "") != 0);

    Console::getInstance()->resize(HAUTEUR_CONSOLE, LARGEUR_CONSOLE); //Redimensionnement de la console, pour qu'elle ait la même taille sur tous les PC,
                                                                      //et ainsi pouvoir afficher le jeu entier sans avoir à scroller
    Jeu battleship = Jeu(); //Instanciation du jeu
    battleship.menu();      //Lancement du menu du jeu

    return 0;

}END_OF_MAIN();
