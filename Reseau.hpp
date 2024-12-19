#ifndef RESEAU_H
#define RESEAU_H

#include <iostream>
#include <array>
#include <vector>
#include <SFML/Graphics.hpp>
#include "Case.hpp"
class Site;
#include"Cristaux.hpp"
#include"FonctionsUtiles.hpp"
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
    vector<Cristal> cristaux;
    Site site_index(int) const;
    friend class cristal;

public :
// valeur disponible
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
    vector<Cristal> get_cristaux() const;

//Dynamique
    std::array<Site,8> voisins_immediat(Site);
    void cristallisation_1case(Site, int type);

// Rendue 
    void affiche_SFML(sf::RenderWindow& window, float x, float y) const;
};



#endif