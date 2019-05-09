#ifndef BATEAUX_H_INCLUDED
#define BATEAUX_H_INCLUDED

#include <iostream>
#include <vector>
#include <time.h>
#include <math.h>
#include "console.h"
#include "son.h"

using namespace std;


#define MESSAGE_TIME_MS 2250
#define MESSAGE__TOUCHE_TIME_MS MESSAGE_TIME_MS/6


enum Taille ///Tailles possibles des bateaux
{
    TAILLE_CUIRASSE = 7,
    TAILLE_CROISEUR = 5,
    TAILLE_DESTROYER = 3,
    TAILLE_SOUSMARIN = 1
};

enum Puissance ///Nombre de cases touchées par tir du bateau
{
    PUISSANCE_CUIRASSE = 9,
    PUISSANCE_CROISEUR = 4,
    PUISSANCE_DESTROYER = 1,
    PUISSANCE_DESTROYER_FUSEE_ECLAIRANTE = 16,
    PUISSANCE_SOUSMARIN = 1
};

enum Orientation
{
    NORD = 0,
    SUD = 1,
    EST = 2,
    OUEST = 3,
    NB_ORIENTATIONS = 4
};

enum Direction ///Utilisé pour tourner le bateau
{
    GAUCHE = '1',
    DROITE = '2'
};

enum Ascii ///Codes ascii pour représenter les bateaux dans la grille
{
    ASCII_CUIRASSE = 'c',
    ASCII_CROISEUR = 'x',
    ASCII_DESTROYER = 'd',
    ASCII_SOUSMARIN = 's',
    ASCII_CUIRASSE_TETE = 'C',
    ASCII_CROISEUR_TETE = 'X',
    ASCII_DESTROYER_TETE = 'D',
    ASCII_SOUSMARIN_TETE = 'S',
    ASCII_ECLAIRE = (char)219,
    ASCII_TOUCHE = (char)177, ///Code ASCII caractère bloc en pointillés
    ASCII_VIDE = ' '
};

enum TypeRetourDeTir ///Valeur retournée par un tir pour savoir si les coordonnées de cases récupérées correspondents à des dégâts ou une zone éclairée
{
    DEGATS,
    CASES_ECLAIREES
};

typedef struct ///Coordonnées pour représenter une case
{
    usint i, j;
}Coordonnees;

typedef struct ///Structure retournée par un tir : contient le type de tyr effectué et les cases concernées pr le tir
{
    TypeRetourDeTir retourDeTir;
    vector<Coordonnees> coord_cases;
}TypeRetourdeTir_CoordonneesRetour;



class Bateau
{
    protected:
        /**Attributs*/
        usint m_i, m_j;  ///Indices de position dans la grille
        Taille m_taille;
        Puissance m_puissance;
        Orientation m_orientation;
        Ascii m_ascii; ///Caractère de représentation du bateau dans la grille
        vector<Coordonnees> m_casesTouchees;
        bool m_coule;


        /**Méthode internes à la classe*/
        void messageInformation(string message, int sleep=MESSAGE_TIME_MS) const; ///Affiche le message d'erreur passé en paramètre en bas de la console
        bool peutTournerGauche(vector<vector<bool> >& placementGrille) const; ///Vérifie si le bateau peut tourner à gauche en fonction de sa position dans la grille et par rapport aux autres bateaux
        bool peutTournerDroite(vector<vector<bool> >& placementGrille) const; ///Vérifie si le bateau peut tourner à droite en fonction de sa position dans la grille et par rapport aux autres bateaux
        void majPlacementGrille(bool peutTourner, vector<vector<bool> >& placementGrille) const; ///Met à jour les booléens de placementGrille pour connaitre où sont positionnés les bateaux
        Direction demanderDirectionDeRotation() const; ///Demande au joueur Gauche ou Droite pour tourner le bateau
        bool estTouche() const; ///Vérifie si le bateau a subit des dégats (m_casesTouchees non vide), pour si oui l'empêcher d'avancer ou tourner


    public:
        ///Constructeur du bateau, génère sa position "aléatoire" en fonction de la disposition des autres bateaux (placementGrille : matrice de bool indiquant les cases déjà occupées)
        Bateau(Taille t, Puissance p, Ascii a, vector< vector<bool> >& placementGrille);
        virtual ~Bateau();
        /**Getters*/
        int getI() const;
        int getJ() const;
        Taille getTaille() const;
        Puissance getPuissance() const;
        Orientation getOrientation() const;
        Ascii getAscii() const;
        vector<Coordonnees> getCaseTouchees() const;
        bool getCoule() const;

        /**Méthodes polymorphes  --  placementGrille : matrice 2D de bool définissant si une case est vide (false) ou prise (true)**/
        virtual bool avancer(vector<vector<bool> >& placementGrille); ///Met à jour les indices m_i et m_j en fonction de la direction de déplacement
        virtual bool tourner(vector<vector<bool> >& placementGrille); ///Met à jour les indices m_i et m_j en fonction du sens de rotation
        virtual void tournerCarTourSuivant(vector<vector<bool> >& placementGrille); ///Fait tourner le cuirassé 1 tour après la demande du joueur
        virtual TypeRetourdeTir_CoordonneesRetour tirer(Coordonnees coord_tir, vector<vector<bool> > placementGrilleAdversaire); ///Regarde si des bateaux sont présents dans la zone du tir et retourne leurs positions

        void recevoirTir(Coordonnees coord_tir); ///Ajoute les coordonnées de la case touchée au vecteur les comptabilisant (m_casesTouchees)
};


class Cuirasse : public Bateau
{
    private:
        bool m_tournerTourSuivant;
        bool m_peutTournerDroite;
        bool m_peutTournerGauche;
        Orientation m_orientationFuture;

    public:
        ///Constructeur qui appelle le constructeur de la classe mère, et lui trnsmet en paramètre la matrice de bool des cases occupées
        Cuirasse(vector< vector<bool> >& placementGrille);
        ~Cuirasse();

        bool avancer(vector<vector<bool> >& placementGrille);
        bool tourner(vector<vector<bool> >& placementGrille);
        void tournerCarTourSuivant(vector<vector<bool> >& placementGrille); ///Fait tourner le cuirassé 1 tour après la demande du joueur
        TypeRetourdeTir_CoordonneesRetour tirer(Coordonnees coord_tir, vector<vector<bool> > placementGrilleAdversaire);
};


/**Classe filles de Bateau*/

class Croiseur : public Bateau
{
    public:
        ///Constructeur qui appelle le constructeur de la classe mère, et lui trnsmet en paramètre la matrice de bool des cases occupées
        Croiseur(vector< vector<bool> >& placementGrille);
        ~Croiseur();

        bool avancer(vector<vector<bool> >& placementGrille);
        bool tourner(vector<vector<bool> >& placementGrille);
        TypeRetourdeTir_CoordonneesRetour tirer(Coordonnees coord_tir, vector<vector<bool> > placementGrilleAdversaire);
};


class Destroyer : public Bateau
{
    private:
        bool m_premierTirEffectue; ///Pour savoir si le Destroyer doit tirer une fusée ou un tir normal
        usint m_puissanceFuseeEclairante;

        /**Méthode interne à la classe*/
        TypeRetourdeTir_CoordonneesRetour tirerFuseeEclairante(Coordonnees coord_tir, vector<vector<bool> > placementGrilleAdversaire);

    public:
        ///Constructeur qui appelle le constructeur de la classe mère, et lui trnsmet en paramètre la matrice de bool des cases occupées
        Destroyer(vector< vector<bool> >& placementGrille);
        ~Destroyer();

        bool avancer(vector<vector<bool> >& placementGrille);
        bool tourner(vector<vector<bool> >& placementGrille);
        TypeRetourdeTir_CoordonneesRetour tirer(Coordonnees coord_tir, vector<vector<bool> > placementGrilleAdversaire);
};


class SousMarin : public Bateau
{
    public:
        ///Constructeur qui appelle le constructeur de la classe mère, et lui trnsmet en paramètre la matrice de bool des cases occupées
        SousMarin(vector< vector<bool> >& placementGrille);
        ~SousMarin();

        bool avancer(vector<vector<bool> >& placementGrille);
        bool tourner(vector<vector<bool> >& placementGrille);
        TypeRetourdeTir_CoordonneesRetour tirer(Coordonnees coord_tir, vector<vector<bool> > placementGrilleAdversaire);
};

#endif // BATEAUX_H_INCLUDED
