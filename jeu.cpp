#include "jeu.h"


Jeu::Jeu()
    : m_tourDuJoueur(0)
{



}

Jeu::~Jeu()
{

}

/** DESIGN A AMELIORER **/
void Jeu::afficherMenu() const
{
    Console* console = Console::getInstance();

    console->clear();
    console->setColor(BLANC, NOIR);
    console->writeLine("\n");
    console->writeLine("                                      # #  ( )      ");
    console->writeLine("                                   ___#_#___|__ ");
    console->writeLine("                               _  |____________|  _ ");
    console->writeLine("                        _=====| | |            | | |==== _ ");
    console->writeLine("                  =====| |.---------------------------. | |==== ");
    console->writeLine("    <--------------------'   .  .  .  .  .  .  .  .   '-----------------) ");
    console->writeLine("      (                                                                ) ");
    console->setColor(BLANC, NOIR);
    console->write("       (");
    console->setColor(BLEU_GRIS, NOIR);
    console->write("                         BATTLESHIP 2.0                       ");
    console->setColor(BLANC, NOIR);
    console->writeLine(")");
    console->setColor(BLANC, NOIR);
    console->writeLine("        (                                                            )  ");
    console->writeLine("         (                       1. JOUER                           )   ");
    console->writeLine("          (                      2. CHARGER PARTIE                 ) ");
    console->writeLine("           (                     3. AIDE                          ) ");
    console->writeLine("            (                    4. QUITTER                      )   ");
    console->writeLine("             (__________________________________________________) ");
    console->setColor(BLEU_GRIS, NOIR);
    console->writeLine("   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ ");
    console->writeLine("  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  ");
    console->writeLine("   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~    ");
    console->setColor(GRIS_CLAIR, NOIR);

    console->gotoLigCol(14, LARGEUR_CONSOLE/2); //Positionnement du curseur sur la coque du bateau, à la même ligne que l'option quitter
}


void Jeu::menu()
{
    Console* console = Console::getInstance();
    bool quitter = false;

    this->afficherMenu();

    while(!quitter)
    {
        if(console->isKeyboardPressed()) //Choix de l'action dans le menu
        {
            switch(console->getInputKey())
            {
                case '1': this->creerPartie();
                          this->boucleDeJeu();
                    break;
                case '2': this->chargerPartie();
                          this->boucleDeJeu();
                    break;
                case '3': this->aide();
                    break;
                case '4':
                case ECHAP : quitter = true;
                    break;
            }
            this->afficherMenu();
        }
    }
}


void Jeu::creerPartie()
{
    //Instanciation de NB_JOUEUR nouveaux joueurs à chaque fois que je le jeu est lancé
    m_joueur.clear();
    for(int i=0; i<NB_JOUEURS; i++)
        m_joueur.push_back(Joueur());
    m_tourDuJoueur = 0;
}


void Jeu::gererGrille() const
{
    Grille g = Grille();
    g.genererGrilleBateaux(m_joueur[m_tourDuJoueur].getFlotte());
    g.afficherGrilleBateaux();
    g.genererDegatsAdversaire(m_joueur[m_tourDuJoueur].getCoordDegatsAdversaire(), m_joueur[m_tourDuJoueur].getCoordCasesEclairees());
    g.afficherDegatsAdversaire();
}



unsigned int Jeu::sizeMax(vector<string> v) const
{
    unsigned int sizeMax = 0, indice = 0;
    for(unsigned int i=0; i<v.size(); i++)
    {
        if(v[i].size() > sizeMax)
        {
            sizeMax = v[i].size();
            indice = i;
        }
    }
    return indice;
}

char Jeu::choixAction() const
{
    Console* console = Console::getInstance();
    char choix;

    //Affichage des optoions en dessous des grilles
    vector<string> option = {"  Avancer  ", "  Tourner  ", "  Tirer  ", "Passer un tour"};
    short int ligne = POS_Y_ACTIONS;
    console->setColor(GRIS_CLAIR, NOIR);
    for(usint i=0; i<option.size(); i++)
    {
        console->gotoLigCol(ligne++, (LARGEUR_CONSOLE-option[this->sizeMax(option)].size())/2-2);
        cout << i+1 << ")";
    }
    ligne = POS_Y_ACTIONS;
    for(usint i=0; i<option.size(); i++)
    {
        console->gotoLigCol(ligne++, (LARGEUR_CONSOLE-option[i].size())/2+1);
        console->write(option[i]);
    }
    console->gotoLigCol(ligne++, LARGEUR_CONSOLE/2);

    //Choix de l'option
    do
        choix = console->getInputKey();
    while(!((choix >= '1' && choix <= '4') || (choix == ECHAP)));

    //Effacement des options après que le choix ait été fait
    console->gotoLigCol(POS_Y_ACTIONS, 0);
    for(unsigned short i=0; i<option.size(); i++)
    {
        for(unsigned short j=0; j<LARGEUR_CONSOLE; j++)
            console->write(" ");
        console->writeLine("");
    }

    return choix;
}


void Jeu::boucleDeJeu()
{
    Console* console = Console::getInstance();
    bool quitter = false, finPartie = false;

    console->clear();

    while(!(quitter || finPartie))
    {
        //console->clear();
        bool action_effectuee = false;
        do
        {
            //Affichage du numéro du joueur
            console->effacerZoneActions();
            console->setColor(ROUGE, NOIR);
            stringstream stream;
            stream << "JOUEUR " << m_tourDuJoueur+1;
            console->gotoLigCol(1, (LARGEUR_CONSOLE-stream.str().size())/2);
            cout << stream.str(); //Utilisation de cout au lieu de l'objet console pour afficher la variable

            //On fait tourner le cuirassé s'il le joueur l'a demandé au tour précédent
            m_joueur[m_tourDuJoueur].avancerCuirasse();

            //Création et affichage de la grille du joueur
            this->gererGrille();

            m_joueur[m_tourDuJoueur].eteindreFuseeEclairante();

            //Affichage et choix entre avancer / tourner / tirer
            switch(this->choixAction())
            {
                case '1': action_effectuee = m_joueur[m_tourDuJoueur].avancer();
                    break;
                case '2': action_effectuee = m_joueur[m_tourDuJoueur].tourner();
                    break;
                case '3': action_effectuee = m_joueur[m_tourDuJoueur].tirer(m_joueur[(m_tourDuJoueur+1)%NB_JOUEURS]);
                    break;
                case ECHAP : action_effectuee = true;
                             quitter = true;
                    break;
                default: action_effectuee = true;
                    break;
            }

            //Si je joueur a avancé tourné ou tiré, on réaffiche la grille mise à jour, avec des pauses pour pouvoir visualiser l'action
            if(action_effectuee && !quitter)
            {
                console->wait(SLEEP);
                this->gererGrille();
                console->wait(SLEEP);
            }
        }
        while(!action_effectuee);

        ++m_tourDuJoueur %= NB_JOUEURS;
        if(m_joueur[m_tourDuJoueur].aPerdu())
        {
            allegro_message("Joueur %d a perdu !", m_tourDuJoueur+1);
            finPartie = true;
        }
    }

    if(!finPartie)
        this->sauvegarder();
}

void Jeu::aide()
{
    Console* console = Console::getInstance();
    console->clear();

    ifstream fichier_aide("aide.txt", ios::in);

    if(fichier_aide)
    {
        string ligne;
        console->setColor(BLEU_GRIS, NOIR);
        getline(fichier_aide, ligne);
        console->writeLine(ligne);
        console->setColor(GRIS_CLAIR, NOIR);
        while(getline(fichier_aide, ligne))
            console->writeLine(ligne);
        console->setColor(GRIS, NOIR);
        console->writeLine("\n\nAppuyez sur Echap pour revenir au menu.");

        fichier_aide.close();
    }
    else
        cerr << "Impossible de charger l'aide." << endl;

    while(Console::getInstance()->getInputKey() != ECHAP);
}

void Jeu::chargerPartie()
{
    /** CHARGER LA PARTIE DU FICHIER BINAIRE**/
    allegro_message("La sauvegarde et le chargement de la partie n'ont pas été codés par manque de temps.\nUne nouvelle partie va donc démarrer.");
    this->creerPartie();
}

void Jeu::sauvegarder() const
{
    /** SAUVEGARDER TOUS LES ATTRIBUTS DES JOUEURS **/

    /*ofstream fichier_sauvegarde;
    fichier_sauvegarde.open("sauvegarde",ios::binary | ios::out);

    fichier_sauvegarde.write((char*)&m_tourDuJoueur, sizeof(m_tourDuJoueur)); //Tour du joueur
    for(usint n=0; n<NB_JOUEURS; n++)
    {
        fichier_sauvegarde.write((char*)&m_tourDuJoueur, sizeof(m_tourDuJoueur)); //Tour du joueur

        Joueur joueur = m_joueur[n];
        vector<vector<bool> >* placementGrille = joueur.getpPlacementGrille();
        fichier_sauvegarde.write((char*)placementGrille, sizeof(bool));


    }*/
}
