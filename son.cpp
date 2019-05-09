#include "son.h"


SAMPLE* Son::m_explosion = load_sample("sounds/explosion.wav");
SAMPLE* Son::m_fusee = load_sample("sounds/fusee.wav");
SAMPLE* Son::m_klaxon = load_sample("sounds/klaxon.wav");
SAMPLE* Son::m_plouf = load_sample("sounds/plouf.wav");


void Son::bruitageExplosion()
{
    if(m_explosion != NULL)
        play_sample(m_explosion, 170, 170, 1000, 0); //170 volume baffle gauche - droite | 1000 vitesse | 0 pas de répétition (1 pour boucle infini)
}

void Son::bruitageFusee()
{
    if(m_fusee != NULL)
        play_sample(m_fusee, 140, 140, 1000, 0);
}

void Son::bruitageKlaxon()
{
    if(m_klaxon != NULL)
        play_sample(m_klaxon, 140, 140, 1000, 0);
}

void Son::bruitagePlouf()
{
    if(m_plouf != NULL)
        play_sample(m_plouf, 170, 170, 1000, 0);
}
