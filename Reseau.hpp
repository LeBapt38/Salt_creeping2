#ifndef RESEAU_H
#define RESEAU_H

#include <iostream>
#include <array>
#include <SFML/Graphics.hpp>
#include "Case.hpp"
using namespace std;

class Site {
private :
    int _index;
    int _x;
    int _y;

    friend class Reseau;
    Site (int i, int x, int y) : _index(i), _x(x), _y(y) {}
    Site () = default;
};

class Reseau{
private :
    int nx;
    int ny;
    Case* tab; 
    Case case_defaut;
    Site site_index(int) const;

public :
//constructeurs
    Reseau(int, int, Case);
    Reseau();
    Reseau(const Reseau&);
    Reseau& operator= (const Reseau&);

// Destructeur
    ~Reseau();

// autres methodes
    Site site_xy(int,int) const;
    Site site_aleatoire() const;
    int nbSite();
    Case operator[](Site) const;
    Case& operator[](Site);
    std::array<Site,8> voisins_immediat(Site);

// Rendue 
    void affiche_SFML(sf::RenderWindow& window, float x, float y) const;
};



#endif