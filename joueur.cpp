#include "joueur.h"

Joueur::Joueur()
{
    m_placementGrille.resize(TAILLE_GRILLE, vector<bool>(TAILLE_GRILLE, false));

    for(unsigned int i=0; i<NB_CUIRASSE; i++)
    {
        m_flotte.push_back(new Cuirasse(m_placementGrille));
        m_indicesCuirasse.push_back(i);
    }
    for(unsigned int i=0; i<NB_CROISEUR; i++)
        m_flotte.push_back(new Croiseur(m_placementGrille));
    for(unsigned int i=0; i<NB_DESTROYER; i++)
        m_flotte.push_back(new Destroyer(m_placementGrille));
    for(unsigned int i=0; i<NB_SOUS_MARIN; i++)
        m_flotte.push_back(new SousMarin(m_placementGrille));
}

Joueur::~Joueur()
{

}


vector<vector<bool> > Joueur::getPlacementGrille() const
{
    return m_placementGrille;
}
vector<vector<bool> >* Joueur::getpPlacementGrille()
{
    return &m_placementGrille;
}

vector<Bateau*> Joueur::getFlotte() const
{
    return m_flotte;
}


vector<usint> Joueur::getIndicesCuirasse() const
{
    return m_indicesCuirasse;
}


vector<Coordonnees> Joueur::getCoordDegatsAdversaire() const
{
    return m_coord_degatsAdversaire;
}

vector<Coordonnees> Joueur::getCoordCasesEclairees() const
{
    return m_coord_casesEclairees;
}


void Joueur::afficherAction(string s) const
{
    Console* console = Console::getInstance();
    console->setColor(BLANC, NOIR);
    console->gotoLigCol(POS_Y_ACTIONS, (LARGEUR_CONSOLE-s.size())/2);
    console->write(s);
}



bool Joueur::verifierCoordonnees(string coordonnes) const
{
    if(coordonnes.size() == 2)
    {
        if(tolower(coordonnes[0]) >= 'a' && tolower(coordonnes[0]) < 'a'+TAILLE_GRILLE)
        {
            if(coordonnes[1] >= '0' && coordonnes[1] <= '9')
                return true;
        }
    }
    else if(coordonnes.size() == 3)
    {
        if(tolower(coordonnes[0]) >= 'a' && tolower(coordonnes[0]) < 'a'+TAILLE_GRILLE)
        {
            if(coordonnes[1] == '1')
            {
                if(coordonnes[2] >= '0' && coordonnes[2] <= '4')
                    return true;
            }
        }
    }

    return false;
}


Coordonnees Joueur::choisirCoordonnees(string s) const
{
    Console* console = Console::getInstance();
    string coordonnes;
    short unsigned int ligne = POS_Y_ACTIONS+1;
    bool ok = false;

    do
    {
        console->setColor(GRIS, NOIR);
        console->gotoLigCol(ligne, LARGEUR_CONSOLE/2-s.size());
        console->write(s);

        console->gotoLigCol(ligne, LARGEUR_CONSOLE/2);
        console->setColor(GRIS_CLAIR, NOIR);
        cin >> coordonnes;
        ok = this->verifierCoordonnees(coordonnes);

        //Effacement de ce que l'utilisateur a entré précédement
        if(!ok)
            console->effacerZoneActions();
    }while(!ok);

    //Calcul des indices du bateau dans la grille en fonction des coordonnées
    unsigned short  i = tolower(coordonnes[0])-'a', j;
    if(coordonnes.size() == 2)
        j = coordonnes[1]-'0';
    else
        j = 10 + coordonnes[2]-'0';
    Coordonnees c = {i, j};
    return c;
}

void Joueur::eteindreFuseeEclairante()
{
    m_coord_casesEclairees.clear(); //Vider les cases éclairées précédement, pour qu'elles ne s'affichent plus
}



usint Joueur::verifierBateauExisteAuxCoordonnees(Coordonnees coords_bateau) const
{
    //Sélection du bon bateau dans la flotte en fonction des coordonnées récupérées (un bateau peut être sélectionné par n'importe laquelle de ses cases)
    for(usint n=0; n<m_flotte.size(); n++)
    {
        /*if(m_flotte[n]->getCoule())
            return NB_BATEAUX_FLOTTE;
        else
        {*/
            Orientation o = m_flotte[n]->getOrientation();
            usint bateau_i = m_flotte[n]->getI()  ,  bateau_j = m_flotte[n]->getJ();
            Taille t = m_flotte[n]->getTaille();

            if(o == NORD  ||  o == SUD)
            {
                if(coords_bateau.j == bateau_j  &&  coords_bateau.i >= bateau_i-t/2  &&  coords_bateau.i <= bateau_i+t/2)
                    return n;
            }
            else if(o == EST  ||  o == OUEST)
            {
                if(coords_bateau.i == bateau_i  &&  coords_bateau.j >= bateau_j-t/2  &&  coords_bateau.j <= bateau_j+t/2)
                    return n;
            }
        //}
    }
    return NB_BATEAUX_FLOTTE;
}



bool Joueur::avancer()
{
    Console* console = Console::getInstance();
    usint indice_bateauChoisi = NB_BATEAUX_FLOTTE;

    this->afficherAction("AVANCER");
    while(indice_bateauChoisi == NB_BATEAUX_FLOTTE)
    {
        Coordonnees coords_bateau = this->choisirCoordonnees("Coordonnees du bateau : ");
        indice_bateauChoisi = verifierBateauExisteAuxCoordonnees(coords_bateau);
        console->effacerZoneActions();
    }

    return m_flotte[indice_bateauChoisi]->avancer(m_placementGrille);
}


void Joueur::avancerCuirasse()
{
    for(usint i=0; i<m_indicesCuirasse.size(); i++)
        m_flotte[m_indicesCuirasse[i]]->tournerCarTourSuivant(m_placementGrille);
}



bool Joueur::tourner()
{
    Console* console = Console::getInstance();
    usint indice_bateauChoisi = NB_BATEAUX_FLOTTE;

    this->afficherAction("TOURNER");
    while(indice_bateauChoisi == NB_BATEAUX_FLOTTE)
    {
        Coordonnees coords_bateau = this->choisirCoordonnees("Coordonnees du bateau : ");
        indice_bateauChoisi = verifierBateauExisteAuxCoordonnees(coords_bateau);
        console->effacerZoneActions();
    }

    return m_flotte[indice_bateauChoisi]->tourner(m_placementGrille);
}


void Joueur::recevoirTir(vector<Coordonnees> casesTouchees)
{
    usint indiceBateau;
    for(usint n=0; n<casesTouchees.size(); n++)
    {
        indiceBateau = verifierBateauExisteAuxCoordonnees(casesTouchees[n]);
        if(indiceBateau != NB_BATEAUX_FLOTTE)
            m_flotte[indiceBateau]->recevoirTir(casesTouchees[n]);
    }
}


bool Joueur::tirer(Joueur adversaire)
{
    Console* console = Console::getInstance();
    usint indice_bateauChoisi = NB_BATEAUX_FLOTTE;

    this->afficherAction("TIRER");
    while(indice_bateauChoisi == NB_BATEAUX_FLOTTE)
    {
        Coordonnees coords_bateau = this->choisirCoordonnees("Coordonnees du bateau : ");
        indice_bateauChoisi = verifierBateauExisteAuxCoordonnees(coords_bateau);
        console->effacerZoneActions();
    }

    Coordonnees coords_tir = this->choisirCoordonnees("Coordonnees du tir : ");

    //On récupère les cases touchées par le tir
    TypeRetourdeTir_CoordonneesRetour retour = m_flotte[indice_bateauChoisi]->tirer(coords_tir, adversaire.getPlacementGrille());
    if(retour.retourDeTir == DEGATS) //Si le tir était un tir normal, on ajoute les coordonnées de cases touchées dans le vecteur correspondant
    {
        vector<Coordonnees> nouveauxDegatsAdversaire = retour.coord_cases;
        if(!nouveauxDegatsAdversaire.empty())
        {
            for(usint i=0; i<nouveauxDegatsAdversaire.size(); i++)
                m_coord_degatsAdversaire.push_back(nouveauxDegatsAdversaire[i]); //On ajoute les nouveaux dégats à ceux déjà existants

            adversaire.recevoirTir(nouveauxDegatsAdversaire);
        }
    }
    else if(retour.retourDeTir == CASES_ECLAIREES) //Si le tir était une fusée éclairante de Destroyer, on récupère les cases éclairées par le tir
        m_coord_casesEclairees = retour.coord_cases;

    return true;
}


bool Joueur::aPerdu() const
{
    for(usint i=0; i<m_flotte.size(); i++)
        if(!m_flotte[i]->getCoule())
            return false;
    return true;
}
