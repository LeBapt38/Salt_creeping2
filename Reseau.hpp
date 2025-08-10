#ifndef RESEAU_H
#define RESEAU_H

#include <iostream>
#include <array>
#include <vector>
#include <SFML/Graphics.hpp>
#include <string>
#include <omp.h>
#include "Case.hpp"
class Site;
#include"Cristaux.hpp"
#include"FonctionsUtiles.hpp"
#include <queue>
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
    float dx;
    Case* tab; 
    Case case_defaut;
    vector<Cristal> cristaux;
    Site site_index(int) const;
    friend class cristal;

public :
// valeur disponible
//constructeurs
    Reseau(int, int, float, Case);
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
    std::array<Site,8> voisins_immediat(Site); // donne la liste des 8 sites entourant le site donné
    void cristallisation_1case(Site, int type);
    std::vector<int> type_crist_vois(std::array<Site,8>); // renvoie false si la case est est en contact avec deux cristaux différents.
    int nb_bord_commun(Site);
    float energie_liaison_site(Site,float El);
    float proba_site(Site, float long_liaison,float T, float J, float El); // non normalise!!!, met a jour la proba de cristallisation dans la classe case et renvoie la meme proba
    std::vector<Site> sites_a_traiter(std::vector<Site>&); //Prends le vecteur avec les sites a traiter, en profite pour renvoyer les cases definissant la surface
    void ajuste_dist(Site site);
    void pas_de_temps(float proportion_cristalliser, float long_liaison = 2.36e-10 ,float T = 315, float J = 1e-3 / (3.6*0.5), float El = 100*6.4e-19); //renvoie dt

// Rendue 
    void affiche_SFML(sf::RenderWindow& window, float x, float y) const;
    void liste_taille_crist(std::string nom_fich = "taille_cristaux.dat");
    void enregistre_grille(std::string nom_fich = "image.dat");
};



#endif