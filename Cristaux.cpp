#include "Cristaux.hpp"

Cristal::Cristal(vector<Site> liste, float orientation)
    : listeSites(liste), nbSites(int(liste.size())), orientation(orientation){}

Cristal::Cristal(){
    std::vector<Site> list;
    Cristal(list,0);
}

Cristal::Cristal(const Cristal& old)
    : listeSites(old.listeSites), nbSites(old.nbSites), orientation(old.orientation){}

Cristal& Cristal::operator=(const Cristal& old){
    Cristal temp(old);
    std::swap(listeSites, temp.listeSites);
    std::swap(nbSites, temp.nbSites);
    std::swap(orientation, temp.orientation);

    return *this;
}