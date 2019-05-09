#include "grille.h"


Grille::Grille()
{
    m_grilleBateaux.assign(TAILLE_GRILLE, vector<string>(TAILLE_GRILLE, "  "));
    m_grilleDegatsAdversaire.assign(TAILLE_GRILLE, vector<string>(TAILLE_GRILLE, "  "));
}

Grille::~Grille()
{

}

string Grille::toUpper(string s)
{
    string upper = "";
    for(usint i=0; i<s.size(); i++)
        upper += toupper((int)s[i]);
    return upper;
}


void Grille::genererGrilleBateaux(vector<Bateau*> flotte)
{
    for(unsigned int n=0; n<flotte.size(); n++)
    {
        if(!flotte[n]->getCoule())
        {
            //Construction de la chaine représentant le bateau, puis attribution dans la grille
            stringstream stream1;
            stream1 << (char)flotte[n]->getAscii() << (char)flotte[n]->getAscii();
            string ascii = stream1.str();

            stringstream stream2;
            stream2 << (char)ASCII_TOUCHE << (char)ASCII_TOUCHE;
            string ascii_touche = stream2.str();

            Orientation o = flotte[n]->getOrientation();
            Taille t = flotte[n]->getTaille();
            int I = flotte[n]->getI(), J = flotte[n]->getJ();

            switch(o)
            {
                case NORD:
                    m_grilleBateaux[I-(t-1)/2][J] = this->toUpper(ascii);
                    for(int i=I-(t-1)/2+1; i<I+(t-1)/2+1; i++)
                        m_grilleBateaux[i][J] = ascii;
                    break;

                case SUD:
                    for(int i=I-(t-1)/2; i<I+(t-1)/2; i++)
                        m_grilleBateaux[i][J] = ascii;
                    m_grilleBateaux[I+(t-1)/2][J] = this->toUpper(ascii);
                    break;

                case EST:
                    for(int j=J-(t-1)/2; j<J+(t-1)/2; j++)
                        m_grilleBateaux[I][j] = ascii;
                    m_grilleBateaux[I][J+(t-1)/2] = this->toUpper(ascii);
                    break;

                case OUEST:
                    m_grilleBateaux[I][J-(t-1)/2] = this->toUpper(ascii);
                    for(int j=J-(t-1)/2+1; j<J+(t-1)/2+1; j++)
                        m_grilleBateaux[I][j] = ascii;
                    break;

                default:
                    break;
            }

            //On regarde si des cases ont été touchées. Si oui on les génère comme telles
            vector<Coordonnees> casesTouchees = flotte[n]->getCaseTouchees();
            for(usint k=0; k<casesTouchees.size(); k++)
                m_grilleBateaux[casesTouchees[k].i][casesTouchees[k].j] = ascii_touche;
        }
    }
}


void Grille::afficherGrilleBateaux() const
{
    Console* console = Console::getInstance();
    usint ligne = POS_GRILLE_BATEAUX_Y;
    char lettre = 'a';

    console->gotoLigCol(ligne++, POS_GRILLE_BATEAUX_X);
    console->setColor(GRIS_CLAIR, NOIR);
    console->write("    0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 ");
    console->gotoLigCol(ligne++, POS_GRILLE_BATEAUX_X);
    console->write("  ----------------------------------------------");

    for(unsigned int i=0; i<TAILLE_GRILLE; i++)
    {
        console->gotoLigCol(ligne++, POS_GRILLE_BATEAUX_X);
        console->setColor(GRIS_CLAIR, NOIR);
        cout << lettre++ << " |";
        for(unsigned int j=0; j<TAILLE_GRILLE; j++)
        {
            switch(m_grilleBateaux[i][j][0])
            {
                case ASCII_CUIRASSE:
                case ASCII_CUIRASSE_TETE:
                    console->setColor(JAUNE_FLUO, BLEU_FONCE);
                    break;
                case ASCII_CROISEUR:
                case ASCII_CROISEUR_TETE:
                    console->setColor(ROUGE, BLEU_FONCE);
                    break;
                case ASCII_DESTROYER:
                case ASCII_DESTROYER_TETE:
                    console->setColor(VERT_FLUO, BLEU_FONCE);
                    break;
                case ASCII_SOUSMARIN:
                case ASCII_SOUSMARIN_TETE:
                    console->setColor(ROSE_FLUO, BLEU_FONCE);
                    break;
                default:
                    console->setColor(GRIS_CLAIR, BLEU_FONCE);
                    break;
            }
            console->write(m_grilleBateaux[i][j]);
            console->setColor(GRIS_CLAIR, BLEU_FONCE);
            if(j != TAILLE_GRILLE-1)
                console->write("|");
        }
        console->setColor(GRIS_CLAIR, NOIR);
        console->write("|");
    }

    console->gotoLigCol(ligne++, POS_GRILLE_BATEAUX_X);
    console->setColor(GRIS_CLAIR, NOIR);
    console->write("  ----------------------------------------------");
}




void Grille::genererDegatsAdversaire(vector<Coordonnees> coord_degatsAdversaire, vector<Coordonnees> coord_casesEclairees)
{
    //Construction de la chaine représentant le bateau, puis attribution dans la grille
    stringstream stream1;
    stream1 << (char)ASCII_TOUCHE << (char)ASCII_TOUCHE;
    string ascii_touche = stream1.str();

    stringstream stream2;
    stream2 << (char)ASCII_ECLAIRE << (char)ASCII_ECLAIRE;
    string ascii_eclaire = stream2.str();

    //Cases touchées
    for(usint n=0; n<coord_degatsAdversaire.size(); n++)
        m_grilleDegatsAdversaire[coord_degatsAdversaire[n].i][coord_degatsAdversaire[n].j] = ascii_touche;
    //Cases éclairees
    for(usint n=0; n<coord_casesEclairees.size(); n++)
        m_grilleDegatsAdversaire[coord_casesEclairees[n].i][coord_casesEclairees[n].j] = ascii_eclaire;
}


void Grille::afficherDegatsAdversaire() const
{
    Console* console = Console::getInstance();
    usint ligne = POS_GRILLE_DEGATS_Y;
    char lettre = 'a';

    console->gotoLigCol(ligne++, POS_GRILLE_DEGATS_X);
    console->setColor(GRIS_CLAIR, NOIR);
    console->write("    0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 ");
    console->gotoLigCol(ligne++, POS_GRILLE_DEGATS_X);
    console->write("  ----------------------------------------------");

    for(unsigned int i=0; i<TAILLE_GRILLE; i++)
    {
        console->setColor(GRIS_CLAIR, NOIR);
        console->gotoLigCol(ligne++, POS_GRILLE_DEGATS_X);
        //console->setColor(GRIS_CLAIR, BLEU_FONCE);
        cout << lettre++ << " |";
        for(unsigned int j=0; j<TAILLE_GRILLE; j++)
        {
            switch(m_grilleDegatsAdversaire[i][j][0])
            {
                case ASCII_TOUCHE:
                    console->setColor(JAUNE_FLUO, ROUGE);
                    break;
                case ASCII_ECLAIRE:
                    console->setColor(BLEU, BLEU_FONCE);
                    break;
                default:
                    console->setColor(GRIS_CLAIR, BLEU_FONCE);
                    break;
            }
            console->write(m_grilleDegatsAdversaire[i][j]);
            console->setColor(GRIS_CLAIR, BLEU_FONCE);
            if(j != TAILLE_GRILLE-1)
                console->write("|");
        }
        console->setColor(GRIS_CLAIR, NOIR);
        console->write("|");
    }

    console->setColor(GRIS_CLAIR, NOIR);
    console->gotoLigCol(ligne++, POS_GRILLE_DEGATS_X);
    console->write("  ----------------------------------------------");
}
