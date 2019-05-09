/** Classe encapsulant la console fournie par Florent Diedler **/
/*  Ajout de la méthode pour effacer le contenu de la console  */
/*  Ajout des méthodes pour écrire dans la console             */
/* Modification méthode changement de couleurs dans la console */

#ifndef CONSOLE_H_INCLUDED
#define CONSOLE_H_INCLUDED

#include <allegro.h>
#include <winalleg.h>
#include <conio.h>
#include <iostream>
#include <ostream>
#include <sstream>

using namespace std;

#define LARGEUR_CONSOLE 120
#define HAUTEUR_CONSOLE 30

#define POS_GRILLE_BATEAUX_X 1
#define POS_GRILLE_BATEAUX_Y 2
#define POS_GRILLE_DEGATS_X LARGEUR_CONSOLE-49
#define POS_GRILLE_DEGATS_Y POS_GRILLE_BATEAUX_Y

#define TAILLE_GRILLE 15

#define POS_Y_ACTIONS POS_GRILLE_BATEAUX_Y+TAILLE_GRILLE+4 //Ligne dans la console à partir de laquelle afficher les options du joueur

typedef unsigned short int usint; //Définition d'un nouveau type pour ne pas avoir à écrire "unsigned short int" à chaque fois

enum Color
{
    NOIR = 0,
    BLEU_FONCE = 1,
    VERT = 2,
    BLEU_GRIS = 3,
    MARRON = 4,
    POURPRE = 5,
    KAKI = 6,
    GRIS_CLAIR = 7,
    GRIS = 8,
    BLEU = 9,
    VERT_FLUO = 10,
    TURQUOISE = 11,
    ROUGE = 12,
    ROSE_FLUO = 13,
    JAUNE_FLUO = 14,
    BLANC = 15
};

class Console
{
    private:
        // Empecher la création
        Console();
        ~Console();

        // Empecher la copie d'objet...
        Console& operator= (const Console&){ return *this;}
        Console (const Console&){}

        // Attributs
        static Console* m_instance;



    public:
        // Méthodes statiques (publiques)
        static Console* getInstance();
        static void deleteInstance();

        // Méthodes publiques
        void write(string s);
        void writeLine(string s);
        void clear();
        void effacerZoneActions();
        void gotoLigCol(int lig, int col);
        bool isKeyboardPressed();
        int getInputKey();
        void setColor(Color front, Color back);
        void wait(int t);
        void resize(int lig, int col);
};

#endif // CONSOLE_H_INCLUDED
