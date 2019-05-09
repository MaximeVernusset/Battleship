#ifndef JEU_H_INCLUDED
#define JEU_H_INCLUDED

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include "console.h"
#include "joueur.h"
#include "grille.h"
#include "son.h"

using namespace std;

#define ECHAP 27 ///code ASCII touche 'esc'
#define NB_JOUEURS 2
#define SLEEP 800


class Jeu
{
    private:
        /**Attributs*/
        unsigned char m_tourDuJoueur;
        vector<Joueur> m_joueur; ///Vecteur de NB_JOUEURS (ici 2) joueurs


        /**Méthodes internes à la classe*/
        unsigned int sizeMax(vector<string> v) const; ///Retourne l'indice de la string la plus longue contenue dans le vecteur
        void afficherMenu() const;
        void gererGrille() const;
        char choixAction() const;

    public:
        Jeu();///Constructeur : initialise le tour du joueur au 1er joueur, et instancie NB_JOUEURS (ici 2) joueurs
        ~Jeu();

        /**Méthodes*/
        void menu();
        void creerPartie();
        void boucleDeJeu();
        void chargerPartie();
        void aide();
        void sauvegarder() const;
};

#endif // JEU_H_INCLUDED
