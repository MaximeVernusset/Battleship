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

        /**M�thodes internes � la classe*/
        string toUpper(string s); ///Retourne la string entr�e en param�tre en majuscules

    public:
        Grille();
        ~Grille();

        /**M�thodes*/
        void genererGrilleBateaux(vector<Bateau*> flotte); ///G�n�re la grille du joueur en fonction de sa flotte
        void afficherGrilleBateaux() const;
        void genererDegatsAdversaire(vector<Coordonnees> coord_degatsAdversaire, vector<Coordonnees> coord_casesEclairees); ///G�n�re la grille des d�gats de l'adversaire (cases touch�es et �clair�es par une fus�e)
        void afficherDegatsAdversaire() const;
};


#endif // GRILLE_H_INCLUDED
