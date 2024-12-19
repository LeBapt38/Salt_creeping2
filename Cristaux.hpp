#ifndef CRISTAUX_H
#define CRISTAUX_H
#include <vector>
class Cristal;
#include "Case.hpp"
#include "Reseau.hpp"

class Cristal {
    private :
        std::vector<Site> listeSites;
        int nbSites;
        float orientation;
        friend class Reseau;

    public : 
    // Constructeurs
        Cristal(std::vector<Site> liste, float orientation);
        Cristal();
        Cristal(const Cristal&);
        Cristal& operator=(const Cristal&);
};





#endif