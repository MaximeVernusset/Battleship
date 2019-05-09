#ifndef GRILLE_H_INCLUDED
#define GRILLE_H_INCLUDED

#include <vector>
#include <string>
#include <ostream>
#include <sstream>
#include "bateaux.h"
#include "console.h"

using namespace std;


class Grille
{
    private:
        /**Attributs*/
        vector<vector<string> > m_grilleBateaux;
        vector<vector<string> > m_grilleDegatsAdversaire;

        /**Méthodes internes à la classe*/
        string toUpper(string s); ///Retourne la string entrée en paramètre en majuscules

    public:
        Grille();
        ~Grille();

        /**Méthodes*/
        void genererGrilleBateaux(vector<Bateau*> flotte); ///Génère la grille du joueur en fonction de sa flotte
        void afficherGrilleBateaux() const;
        void genererDegatsAdversaire(vector<Coordonnees> coord_degatsAdversaire, vector<Coordonnees> coord_casesEclairees); ///Génère la grille des dégats de l'adversaire (cases touchées et éclairées par une fusée)
        void afficherDegatsAdversaire() const;
};


#endif // GRILLE_H_INCLUDED
