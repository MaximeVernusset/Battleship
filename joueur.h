#ifndef JOUEUR_H_INCLUDED
#define JOUEUR_H_INCLUDED

#include <vector>
#include <string>
#include "bateaux.h"
#include "grille.h"

using namespace std;


enum ///Nombre de bateaux de chaque type dans la flotte
{
    NB_CUIRASSE = 1,
    NB_CROISEUR = 2,
    NB_DESTROYER = 3,
    NB_SOUS_MARIN = 4
};


#define NB_BATEAUX_FLOTTE NB_CUIRASSE+NB_CROISEUR+NB_DESTROYER+NB_SOUS_MARIN



class Joueur
{
    private:
        /**Attributs*/
        vector<vector<bool> > m_placementGrille; ///Matrice de bool, pour vérifier qu'il n'y a pas de collisions entre bateaux
        vector<Bateau*> m_flotte; ///Vecteur de pointeurs sur Bateau pour pouvoir stocker des classes filles et accéder à leur méthodes
        vector<usint> m_indicesCuirasse; ///Vecteur pour connaitre l'indice des cuirasses (s'il y en a plusieurs dans le jeu), pour pouvoir les retrouver quand il faut les tourner au tour suivant
        vector<Coordonnees> m_coord_degatsAdversaire; ///Liste des cases touchées chez l'adversaire, pour pouvoir les afficher
        vector<Coordonnees> m_coord_casesEclairees; ///Liste des cases contenant des parties de bateaus de l'adversaire pour les afficher dans la grille


    public:
        Joueur();///Constructeur : instancie les différents bateaux dans le vecteur flotte
        ~Joueur();

        /**Getters*/
        vector<vector<bool> > getPlacementGrille() const;
        vector<vector<bool> >* getpPlacementGrille();
        vector<Bateau*> getFlotte() const;
        vector<usint> getIndicesCuirasse() const;
        vector<Coordonnees> getCoordDegatsAdversaire() const;
        vector<Coordonnees> getCoordCasesEclairees() const;

        /**Méthodes*/
        void afficherAction(string s) const; ///Ecrit le titre de l'action après qu'elle ait été choisie
        bool verifierCoordonnees(string coordonnes) const; ///Vérifie que la string est bien une coordonnée de la grille et pas un texte random
        usint verifierBateauExisteAuxCoordonnees(Coordonnees coords_bateau) const; ///Renvoie l'indice du bateau dans la flotte en fonction des coordonnées entrées par le joueur
        Coordonnees choisirCoordonnees(string s) const; ///s : message à afficher
        void eteindreFuseeEclairante(); ///Vide le vecteur m_coord_casesEclairees
        bool avancer(); ///Demande le bateau à tourner puis le fait avancer suivant sa direction
        void avancerCuirasse(); ///Avance le cuirasse au tour d'après
        bool tourner(); ///Demande le bateau à tourner puis le fait tourner dans la bonne direction (propose gauche/droite si le joueur a le choix, ou tourne directement dans la bonne direction
        void recevoirTir(vector<Coordonnees> casesTouchees); ///Pour chaue case touchée, cherche à quel bateau elle appartient et lui attribue les dégats
        bool tirer(Joueur adversaire); ///Tire sur l'adversaire
        bool aPerdu() const; ///Vérifie si tous les bateaux du joueur sont coulés ou non
};

#endif // JOUEUR_H_INCLUDED
