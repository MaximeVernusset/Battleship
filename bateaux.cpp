#include "bateaux.h"


/** BATEAU (classe mère) **/

Bateau::Bateau(Taille t, Puissance p, Ascii a, vector< vector<bool> >& placementGrille)
    : m_taille(t), m_puissance(p), m_ascii(a), m_coule(false)
{
    bool placementOk;
    do
    {
        placementOk = true;

        m_orientation = Orientation(rand()%NB_ORIENTATIONS);

        switch(m_orientation)
        {
            //On définit le milieu du bateau comme référence de placement dans la grille
            //Donc, il faut vérifier qu'il n'y aura pas de dépacement de tableau
            //Ensuite, il faut vérifier que toutes les cases occupées par le bateau sont bien vides
            case NORD:
            case SUD:
                m_i = rand() % (TAILLE_GRILLE-2*(m_taille-1)/2) + (m_taille-1)/2; //Si la direction est verticale, on génère un nombre compris entre la longueur de la moitié du bateau,
                m_j = rand() % TAILLE_GRILLE;                                     //et la taille du tableau moins la moitié du bateau

                for(int i=m_i-(m_taille-1)/2; i<m_i+(m_taille-1)/2+1; i++) //Ensuite on regarde pour toutes les case que va occuper le bateau, si elles ne sont pas déjà prises
                    if(placementGrille[i][m_j] == true)
                        placementOk = false;
                break;

            case EST:
            case OUEST:
                m_i = rand() % TAILLE_GRILLE;
                m_j = rand() % (TAILLE_GRILLE-2*(m_taille-1)/2) + (m_taille-1)/2; //Procédure inversée pour un bateau orienté horizontalement
                for(int j=m_j-(m_taille-1)/2; j<m_j+(m_taille-1)/2+1; j++)
                    if(placementGrille[m_i][j] == true)
                        placementOk = false;
                break;

            default:
                break;
        }
    }while(!placementOk);

    //Une fois les vérifications de placement effectuées, on met à jour la grille de bool pour pouvoir faire les vérifications avec les autes bateaux
    switch(m_orientation)
    {
        case NORD:
        case SUD:
            for(int i=m_i-(m_taille-1)/2; i<m_i+(m_taille-1)/2+1; i++)
                placementGrille[i][m_j] = true;

            break;

        case EST:
        case OUEST:
            for(int j=m_j-(m_taille-1)/2; j<m_j+(m_taille-1)/2+1; j++)
                placementGrille[m_i][j] = true;
            break;

        default:
            break;
    }
}

Bateau::~Bateau()
{

}


int Bateau::getI() const
{
    return m_i;
}

int Bateau::getJ() const
{
    return m_j;
}

Taille Bateau::getTaille() const
{
    return m_taille;
}

Puissance Bateau::getPuissance() const
{
    return m_puissance;
}

Orientation Bateau::getOrientation() const
{
    return m_orientation;
}

Ascii Bateau::getAscii() const
{
    return m_ascii;
}

vector<Coordonnees> Bateau::getCaseTouchees() const
{
    return m_casesTouchees;
}


bool Bateau::getCoule() const
{
    return m_coule;
}


void Bateau::messageInformation(string message, int sleep) const
{
    Console* console = Console::getInstance();
    console->gotoLigCol(POS_Y_ACTIONS+3, LARGEUR_CONSOLE/2-message.size()/2);
    console->setColor(JAUNE_FLUO, NOIR);
    console->write(message);
    console->wait(sleep);
    console->effacerZoneActions();
}

bool Bateau::peutTournerGauche(vector<vector<bool> >& placementGrille) const
{
   //Vérification s'il peut tourner sur la gauche, en fonction de son orientation
    if(m_orientation == NORD  ||  m_orientation == SUD)
    {
        for(usint i=m_i-m_taille/2; i<m_i+1; i++)
        {
            for(usint j=m_j-m_taille/2; j<m_j; j++)
            {
                if(placementGrille[i][j] == true  ||  placementGrille[i+m_taille/2][j+m_taille/2+1] == true)
                    return false;
            }
        }
    }
    else if(m_orientation == EST  ||  m_orientation == OUEST)
    {
        for(usint i=m_i-m_taille/2; i<m_i; i++)
        {
            for(usint j=m_j+m_taille/2; j>m_j-1; j--)
            {
                if(placementGrille[i][j] == true  ||  placementGrille[i+m_taille/2+1][j-m_taille/2] == true)
                    return false;
            }
        }
    }

    return true;
}

bool Bateau::peutTournerDroite(vector<vector<bool> >& placementGrille) const
{
    //Vérification s'il peut tourner sur la droite
    if(m_orientation == NORD  || m_orientation == SUD)
    {
        for(usint i=m_i-m_taille/2; i<m_i+1; i++)
        {
            for(usint j=m_j+m_taille/2; j>m_j; j--)
            {
                if(placementGrille[i][j] == true  ||  placementGrille[i+m_taille/2][j-m_taille/2-1] == true)
                    return false;
            }
        }
    }
    else if(m_orientation == EST  || m_orientation == OUEST)
    {
        for(usint i=m_i-m_taille/2; i<m_i; i++)
        {
            for(usint j=m_j-m_taille/2; j<m_j+1; j++)
            {
                if(placementGrille[i][j] == true  ||  placementGrille[i+m_taille/2+1][j+m_taille/2] == true)
                    return false;
            }
        }
    }

    return true;
}


void Bateau::majPlacementGrille(bool peutTourner, vector<vector<bool> >& placementGrille) const
{
    if(peutTourner)
    {
        if(m_orientation == NORD  || m_orientation == SUD)
        {
            //"Effacement" des cases qui ne sont plus utilisées
            for(usint i=m_i-m_taille/2; i<m_i+m_taille/2+1; i++)
                placementGrille[i][m_j] = false;
            //"Remplissage" des nouvelles cases utilisées
            for(usint j=m_j-m_taille/2; j<m_j+m_taille/2+1; j++)
                placementGrille[m_i][j] = true;
        }
        else if(m_orientation == EST  || m_orientation == OUEST)
        {
            //"Effacement" des cases qui ne sont plus utilisées
            for(usint j=m_j-m_taille/2; j<m_j+m_taille/2+1; j++)
                placementGrille[m_i][j] = false;
            //"Remplissage" des nouvelles cases utilisées
            for(usint i=m_i-m_taille/2; i<m_i+m_taille/2+1; i++)
                placementGrille[i][m_j] = true;
        }
    }
}


Direction Bateau::demanderDirectionDeRotation() const
{
    Console* console = Console::getInstance();
    string s1 = "Direction de rotation : ";
    string s2 = "1) Gauche   2)Droite";
    short unsigned int ligne = POS_Y_ACTIONS+2;
    char c;

    console->setColor(GRIS, NOIR);
    console->gotoLigCol(ligne, LARGEUR_CONSOLE/2-s1.size());
    console->write(s1);
    console->gotoLigCol(ligne, LARGEUR_CONSOLE/2);
    console->setColor(GRIS_CLAIR, NOIR);
    console->write(s2);
    console->gotoLigCol(ligne+1, LARGEUR_CONSOLE/2);

    do
        c = console->getInputKey();
    while(!(c == '1'  ||  c == '2'));

    return (Direction)c;
}


bool Bateau::estTouche() const
{
    if(!m_casesTouchees.empty())
        return true;
    return false;
}




bool Bateau::avancer(vector<vector<bool> >& placementGrille)
{
    switch(m_orientation)
    {
        case NORD: if(m_i-m_taille/2 - 1 >= 0  &&  placementGrille[m_i-m_taille/2-1][m_j] == false)
                   {
                        placementGrille[m_i-m_taille/2-1][m_j] = true;
                        placementGrille[m_i+m_taille/2][m_j] = false;
                        m_i--;
                        Son::bruitageKlaxon();
                        return true;
                   }
                   break;
        case SUD: if(m_i+m_taille/2 + 1 < TAILLE_GRILLE  &&  placementGrille[m_i+m_taille/2+1][m_j] == false)
                   {
                        placementGrille[m_i+m_taille/2+1][m_j] = true;
                        placementGrille[m_i-m_taille/2][m_j] = false;
                        m_i++;
                        Son::bruitageKlaxon();
                        return true;
                   }
                   break;
        case EST: if(m_j+m_taille/2 + 1 < TAILLE_GRILLE  &&  placementGrille[m_i][m_j+m_taille/2+1] == false)
                   {
                        placementGrille[m_i][m_j+m_taille/2+1] = true;
                        placementGrille[m_i][m_j-m_taille/2] = false;
                        m_j++;
                        Son::bruitageKlaxon();
                        return true;
                   }
                   break;
        case OUEST: if(m_j-m_taille/2 - 1 >= 0  &&  placementGrille[m_i][m_j-m_taille/2-1] == false)
                   {
                        placementGrille[m_i][m_j-m_taille/2-1] = true;
                        placementGrille[m_i][m_j+m_taille/2] = false;
                        m_j--;
                        Son::bruitageKlaxon();
                        return true;
                   }
                   break;
        default: break;
    }

    return false;
}

bool Bateau::tourner(vector<vector<bool> >& placementGrille)
{
    bool peutTourner = true;
    bool peutTournerGauche = false, peutTournerDroite = false;

    //Si le bateau en tournant ne dépasse pas de la grille
    if(m_i-m_taille/2 >= 0  && m_i+m_taille/2 < TAILLE_GRILLE  &&  m_j-m_taille/2 >= 0  && m_j+m_taille/2 < TAILLE_GRILLE)
    {
        peutTournerDroite = this->peutTournerDroite(placementGrille);
        peutTournerGauche = this->peutTournerGauche(placementGrille);
        this->majPlacementGrille(peutTournerGauche|peutTournerDroite, placementGrille);

        switch(m_orientation)
        {
           case NORD:
            {
                //Si le bateau peut tourner dans les 2 sens, la direction de rotation est aléatoire, sinon le bateau tourne dans l'unique sens possible
                if(peutTournerGauche && peutTournerDroite)
                {
                    if(this->demanderDirectionDeRotation() == GAUCHE)
                        m_orientation = OUEST;
                    else
                        m_orientation = EST;
                }
                else if(peutTournerGauche)
                    m_orientation = OUEST;
                else if(peutTournerDroite)
                    m_orientation = EST;
                else
                    peutTourner = false;
            }
            break;

            case SUD:
            {
                //Si le bateau peut tourner dans les 2 sens, la direction de rotation est aléatoire, sinon le bateau tourne dans l'unique sens possible
                if(peutTournerGauche && peutTournerDroite)
                {
                    if(this->demanderDirectionDeRotation() == GAUCHE)
                        m_orientation = EST;
                    else
                        m_orientation = OUEST;
                }
                else if(peutTournerGauche)
                    m_orientation = EST;
                else if(peutTournerDroite)
                    m_orientation = OUEST;
                else
                    peutTourner = false;
            }
            break;

            case EST:
            {
                //Si le bateau peut tourner dans les 2 sens, la direction de rotation est aléatoire, sinon le bateau tourne dans l'unique sens possible
                if(peutTournerGauche && peutTournerDroite)
                {
                    if(this->demanderDirectionDeRotation() == GAUCHE)
                        m_orientation = NORD;
                    else
                        m_orientation = SUD;
                }
                else if(peutTournerGauche)
                        m_orientation = NORD;
                else if(peutTournerDroite)
                        m_orientation = SUD;
                else
                    peutTourner = false;
            }
            break;

            case OUEST:
            {
                //Si le bateau peut tourner dans les 2 sens, la direction de rotation est aléatoire, sinon le bateau tourne dans l'unique sens possible
                if(peutTournerGauche && peutTournerDroite)
                {
                    if(this->demanderDirectionDeRotation() == GAUCHE)
                        m_orientation = SUD;
                    else
                        m_orientation = NORD;
                }
                else if(peutTournerGauche)
                    m_orientation = SUD;
                else if(peutTournerDroite)
                    m_orientation = NORD;
                else
                    peutTourner = false;
            }
            break;

            default:
            break;
        }
    }
    else
        peutTourner = false;

    if(peutTourner)
        Son::bruitageKlaxon();

    return peutTourner;
}

void Bateau::tournerCarTourSuivant(vector<vector<bool> >& placementGrille)
{
                 /** NE RIEN ECRIRE ICI **/
    /*Fonction virtuelle utilisée juste pour le cuirassé*/
}


TypeRetourdeTir_CoordonneesRetour Bateau::tirer(Coordonnees coord_tir, vector<vector<bool> > placementGrilleAdversaire)
{
    short int iBorneInf = coord_tir.i - ((short int)sqrt(m_puissance)-1)/2;
    if(iBorneInf < 0) //Si valeur négative, on remet à 0 pour ne pas faire de dépassement de tableau
        iBorneInf = 0;
    short int iBorneSup = coord_tir.i + ((short int)sqrt(m_puissance))/2 + 1;
    if(iBorneSup > TAILLE_GRILLE) //Si valeur > TAILLE_GRILLE, on remet à TAILLE_GRILLE pour ne pas faire de dépassement de tableau
        iBorneSup = TAILLE_GRILLE;
    short int jBorneInf = coord_tir.j - ((short int)sqrt(m_puissance)-1)/2;
    if(jBorneInf < 0) //Si valeur négative, on remet à 0 pour ne pas faire de dépassement de tableau
        jBorneInf = 0;
    short int jBorneSup = coord_tir.j + ((short int)sqrt(m_puissance))/2 + 1;
    if(jBorneSup > TAILLE_GRILLE) //Si valeur négative, on  à 0 pour ne pas faire de dépassement de tableau
        jBorneSup = TAILLE_GRILLE;

    vector<Coordonnees> degatsAdversaireOccasionnes;
    for(usint i=(unsigned)iBorneInf; i<iBorneSup; i++)
    {
        for(usint j=(unsigned)jBorneInf; j<jBorneSup; j++)
        {
            if(placementGrilleAdversaire[i][j] == true) //Si une case est occupée dans la zone du tir, elle est touchée
                degatsAdversaireOccasionnes.push_back({i,j}); //On enregistre les coordonnées de la case touchée, pour récupérer l'indice du bateau concerné plus tard
        }
    }
    if(!degatsAdversaireOccasionnes.empty())
    {
        Son::bruitageExplosion();
        this->messageInformation("Touche", MESSAGE__TOUCHE_TIME_MS);
    }

    return {DEGATS, degatsAdversaireOccasionnes};
}


void Bateau::recevoirTir(Coordonnees coord_tir)
{
    m_casesTouchees.push_back(coord_tir);

    //Si la taille du vecteur de cases touchées = taille du bateau -> le bateau est coulé
    if(m_casesTouchees.size() == m_taille)
    {
        m_coule = true;
        this->messageInformation("Coule", MESSAGE__TOUCHE_TIME_MS);
    }
}



/** CUIRASSE (classe fille) **/

Cuirasse::Cuirasse(vector< vector<bool> >& placementGrille)
    : Bateau(TAILLE_CUIRASSE, PUISSANCE_CUIRASSE, ASCII_CUIRASSE, placementGrille), m_tournerTourSuivant(false), m_peutTournerDroite(false), m_peutTournerGauche(false)
{

}

Cuirasse::~Cuirasse()
{

}


bool Cuirasse::avancer(vector<vector<bool> >& placementGrille)
{
    if(!m_coule)
    {
        if(!this->estTouche())
        {
            if(Bateau::avancer(placementGrille) == false)
            {
                this->messageInformation("Deplacement du CUIRASSE impossible, veuillez essayer autre chose.");
                return false;
            }
            return true;
        }
        this->messageInformation("Le CUIRASSE est endommage, vous ne pouvez pas le deplacer.");
        return false;
    }

    this->messageInformation("Le CUIRASSE a ete coule, vous ne pouvez plus l'utiliser.");
    return false;
}

bool Cuirasse::tourner(vector<vector<bool> >& placementGrille)
{
    if(!m_coule)
    {
        if(!this->estTouche())
        {
            m_peutTournerDroite = false;
            m_peutTournerGauche = false;
            m_tournerTourSuivant = false;

            //Si le bateau en tournant ne dépasse pas de la grille
            if(m_i-m_taille/2 >= 0  && m_i+m_taille/2 < TAILLE_GRILLE  &&  m_j-m_taille/2 >= 0  && m_j+m_taille/2 < TAILLE_GRILLE)
            {
                m_peutTournerDroite = this->peutTournerDroite(placementGrille);
                m_peutTournerGauche = this->peutTournerGauche(placementGrille);
                this->majPlacementGrille(m_peutTournerGauche|m_peutTournerDroite, placementGrille);

                if(m_peutTournerDroite || m_peutTournerGauche)
                {
                    m_tournerTourSuivant = true;

                    switch(m_orientation)
                    {
                        case NORD:
                        {
                            //Si le bateau peut tourner dans les 2 sens, la direction de rotation est demandée
                            if(m_peutTournerGauche && m_peutTournerDroite)
                            {
                                if(this->demanderDirectionDeRotation() == GAUCHE)
                                    m_orientationFuture = OUEST;
                                else
                                    m_orientationFuture = EST;
                            }
                            else if(m_peutTournerGauche)
                                m_orientationFuture = OUEST;
                            else if(m_peutTournerDroite)
                                m_orientationFuture = EST;
                        }
                        break;

                        case SUD:
                        {
                            //Si le bateau peut tourner dans les 2 sens, la direction de rotation est demandée
                            if(m_peutTournerGauche && m_peutTournerDroite)
                            {
                                if(this->demanderDirectionDeRotation() == GAUCHE)
                                    m_orientationFuture = EST;
                                else
                                    m_orientationFuture = OUEST;
                            }
                            else if(m_peutTournerGauche)
                                m_orientationFuture = EST;
                            else if(m_peutTournerDroite)
                                m_orientationFuture = OUEST;
                        }
                        break;

                        case EST:
                        {
                            //Si le bateau peut tourner dans les 2 sens, la direction de rotation est demandée
                            if(m_peutTournerGauche && m_peutTournerDroite)
                            {
                                if(this->demanderDirectionDeRotation() == GAUCHE)
                                    m_orientationFuture = NORD;
                                else
                                    m_orientationFuture = SUD;
                            }
                            else if(m_peutTournerGauche)
                                    m_orientationFuture = NORD;
                            else if(m_peutTournerDroite)
                                    m_orientationFuture = SUD;
                        }
                        break;

                        case OUEST:
                        {
                            //Si le bateau peut tourner dans les 2 sens, la direction de rotation est demandée
                            if(m_peutTournerGauche && m_peutTournerDroite)
                            {
                                if(this->demanderDirectionDeRotation() == GAUCHE)
                                    m_orientationFuture = SUD;
                                else
                                    m_orientationFuture = NORD;
                            }
                            else if(m_peutTournerGauche)
                                m_orientationFuture = SUD;
                            else if(m_peutTournerDroite)
                                m_orientationFuture = NORD;
                        }
                        break;

                        default:
                        break;
                    }

                    Son::bruitageKlaxon();
                    this->messageInformation("Le CUIRASSE entamme son changement de cap, il aura fini de tourner au tour suivant.");

                    return true;
                }
            }

            this->messageInformation("Rotation du CUIRASSE impossible, veuillez essayer autre chose.");
            return false;
        }
        this->messageInformation("Le CUIRASSE est touche, vous ne pouvez pas le tourner.");
        return false;
    }

    this->messageInformation("Le CUIRASSE a ete coule, vous ne pouvez plus l'utiliser.");
    return false;
}



void Cuirasse::tournerCarTourSuivant(vector<vector<bool> >& placementGrille)
{
    if(m_tournerTourSuivant)
        m_orientation = m_orientationFuture;

    m_tournerTourSuivant = false;
}



TypeRetourdeTir_CoordonneesRetour Cuirasse::tirer(Coordonnees coord_tir, vector<vector<bool> > placementGrilleAdversaire)
{
    return Bateau::tirer(coord_tir, placementGrilleAdversaire);
}




/** CROISEUR (classe fille) **/

Croiseur::Croiseur(vector< vector<bool> >& placementGrille)
    : Bateau(TAILLE_CROISEUR, PUISSANCE_CROISEUR, ASCII_CROISEUR, placementGrille)
{

}

Croiseur::~Croiseur()
{

}


bool Croiseur::avancer(vector<vector<bool> >& placementGrille)
{
    if(!m_coule)
    {
        if(!this->estTouche())
        {
            if(Bateau::avancer(placementGrille) == false)
            {
                this->messageInformation("Deplacement du CROISEUR impossible, veuillez essayer autre chose.");
                return false;
            }
            return true;
        }
        this->messageInformation("Le CROISEUR est endommage, vous ne pouvez pas le deplacer.");
        return false;
    }

    this->messageInformation("Le CROISEUR a ete coule, vous ne pouvez plus l'utiliser.");
    return false;
}

bool Croiseur::tourner(vector<vector<bool> >& placementGrille)
{
    if(!m_coule)
    {
        if(!this->estTouche())
        {
            if(Bateau::tourner(placementGrille) == false)
            {
                this->messageInformation("Rotation du CROISEUR impossible, veuillez essayer autre chose.");
                return false;
            }
            return true;
        }
        this->messageInformation("Le CROISEUR est touche, vous ne pouvez pas le tourner.");
        return false;
    }

    this->messageInformation("Le CROISEUR a ete coule, vous ne pouvez plus l'utiliser.");
    return false;
}

TypeRetourdeTir_CoordonneesRetour Croiseur::tirer(Coordonnees coord_tir, vector<vector<bool> > placementGrilleAdversaire)
{
    return Bateau::tirer(coord_tir, placementGrilleAdversaire);
}




/** DESTROYER (classe fille) **/

Destroyer::Destroyer(vector< vector<bool> >& placementGrille)
    : Bateau(TAILLE_DESTROYER, PUISSANCE_DESTROYER, ASCII_DESTROYER, placementGrille), m_premierTirEffectue(false), m_puissanceFuseeEclairante(PUISSANCE_DESTROYER_FUSEE_ECLAIRANTE)
{

}

Destroyer::~Destroyer()
{

}


bool Destroyer::avancer(vector<vector<bool> >& placementGrille)
{
    if(!m_coule)
    {
        if(!this->estTouche())
        {
            if(Bateau::avancer(placementGrille) == false)
            {
                this->messageInformation("Deplacement du DESTROYER impossible, veuillez essayer autre chose.");
                return false;
            }
            return true;
        }
        this->messageInformation("Le DESTROYER est endommage, vous ne pouvez pas le deplacer.");
        return false;
    }

    this->messageInformation("Le DESTROYER a ete coule, vous ne pouvez plus l'utiliser.");
    return false;
}

bool Destroyer::tourner(vector<vector<bool> >& placementGrille)
{
    if(!m_coule)
    {
        if(!this->estTouche())
        {
            if(Bateau::tourner(placementGrille) == false)
            {
                this->messageInformation("Rotation du DESTROYER impossible, veuillez essayer autre chose.");
                return false;
            }
            return true;
        }
        this->messageInformation("Le DESTROYER est touche, vous ne pouvez pas le tourner.");
        return false;
    }

    this->messageInformation("Le DESTROYER a ete coule, vous ne pouvez plus l'utiliser.");
    return false;
}


TypeRetourdeTir_CoordonneesRetour Destroyer::tirerFuseeEclairante(Coordonnees coord_tir, vector<vector<bool> > placementGrilleAdversaire)
{
    short int iBorneInf = coord_tir.i - ((short int)sqrt(m_puissanceFuseeEclairante)-1)/2;
    if(iBorneInf < 0) //Si valeur négative, on remet à 0 pour ne pas faire de dépassement de tableau
        iBorneInf = 0;
    short int iBorneSup = coord_tir.i + ((short int)sqrt(m_puissanceFuseeEclairante))/2 + 1;
    if(iBorneSup > TAILLE_GRILLE) //Si valeur > TAILLE_GRILLE, on remet à TAILLE_GRILLE pour ne pas faire de dépassement de tableau
        iBorneSup = TAILLE_GRILLE;
    short int jBorneInf = coord_tir.j - ((short int)sqrt(m_puissanceFuseeEclairante)-1)/2;
    if(jBorneInf < 0) //Si valeur négative, on remet à 0 pour ne pas faire de dépassement de tableau
        jBorneInf = 0;
    short int jBorneSup = coord_tir.j + ((short int)sqrt(m_puissanceFuseeEclairante))/2 + 1;
    if(jBorneSup > TAILLE_GRILLE) //Si valeur négative, on  à 0 pour ne pas faire de dépassement de tableau
        jBorneSup = TAILLE_GRILLE;

    vector<Coordonnees> casesEclairees;
    for(usint i=(unsigned)iBorneInf; i<iBorneSup; i++)
    {
        for(usint j=(unsigned)jBorneInf; j<jBorneSup; j++)
        {
            if(placementGrilleAdversaire[i][j] == true) //Si une case est occupée dans la zone du tir, elle est touchée
                casesEclairees.push_back({i,j}); //On enregistre les coordonnées de la case touchée, pour récupérer l'indice du bateau concerné plus tard
        }
    }

    return {CASES_ECLAIREES, casesEclairees};
}

TypeRetourdeTir_CoordonneesRetour Destroyer::tirer(Coordonnees coord_tir, vector<vector<bool> > placementGrilleAdversaire)
{
    if(!m_premierTirEffectue)
    {
        Son::bruitageFusee();
        m_premierTirEffectue = true;
        return this->tirerFuseeEclairante(coord_tir, placementGrilleAdversaire);
    }

    return Bateau::tirer(coord_tir, placementGrilleAdversaire);
}




/** SOUS-MARIN (classe fille) **/

SousMarin::SousMarin(vector< vector<bool> >& placementGrille)
    : Bateau(TAILLE_SOUSMARIN, PUISSANCE_SOUSMARIN, ASCII_SOUSMARIN, placementGrille)
{

}

SousMarin::~SousMarin()
{

}


bool SousMarin::avancer(vector<vector<bool> >& placementGrille)
{
    if(!m_coule)
    {
        if(Bateau::avancer(placementGrille) == false)
        {
            this->messageInformation("Deplacement du SOUS-MARIN impossible, veuillez essayer autre chose.");
            return false;
        }
        return true;
    }

    this->messageInformation("Le SOUS-MARIN a ete coule, vous ne pouvez plus l'utiliser.");
    return false;
}

bool SousMarin::tourner(vector<vector<bool> >& placementGrille)
{
    if(!m_coule)
    {
        if(Bateau::tourner(placementGrille) == false)
        {
            this->messageInformation("Rotation du SOUS-MARIN impossible, veuillez essayer autre chose.");
            return false;
        }
        return true;
    }

    this->messageInformation("Le SOUS-MARIN a ete coule, vous ne pouvez plus l'utiliser.");
    return false;
}

TypeRetourdeTir_CoordonneesRetour SousMarin::tirer(Coordonnees coord_tir, vector<vector<bool> > placementGrilleAdversaire)
{
    return Bateau::tirer(coord_tir, placementGrilleAdversaire);
}
