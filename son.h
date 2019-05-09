#ifndef SON_H_INCLUDED
#define SON_H_INCLUDED

#include "console.h"

class Son
{
    private:
        /**Attributs statiques*/
        static SAMPLE* m_explosion;
        static SAMPLE* m_fusee;
        static SAMPLE* m_klaxon;
        static SAMPLE* m_plouf;

    public:
        Son();

        /**Méthodes statiques (accessibles sans instancier un objet Son*/
        static void bruitageExplosion();
        static void bruitageFusee();
        static void bruitageKlaxon();
        static void bruitagePlouf();

};

#endif // SON_H_INCLUDED
