#include "Reseau.hpp"
#include <cstdlib>
#include <array>
#include <cmath>
//Site



//Reseau

Reseau::Reseau(int NX, int NY, Case val_def) : nx(NX), ny(NY), tab(nullptr), case_defaut(val_def), cristaux({}){
    tab = new Case[nx*ny];
    for(int l = 0; l < nx*ny; l++){
        tab[l] = val_def;
    }
}

Reseau::Reseau() : Reseau(0,0,Case()){}

Reseau::Reseau(const Reseau& other) : nx(other.nx), ny(other.ny), tab(nullptr), case_defaut(other.case_defaut), cristaux(other.cristaux) {
// le constructeur a acces au donnees prives du meme type    
    tab = new Case[nx*ny];
    for(int i = 0; i < nx*ny; i++){
        tab[i] = other.tab[i];
    }
}

Reseau::~Reseau(){
    if (tab != nullptr){
        delete[] tab;
    }
    tab = nullptr;
}

Reseau& Reseau::operator=(const Reseau& reseau){
    Reseau temp(reseau);
    std::swap(nx, temp.nx);
    std::swap(ny, temp.ny);
    std::swap(tab, temp.tab);
    std::swap(case_defaut, temp.case_defaut);
    std::swap(cristaux, temp.cristaux);

    return *this;
    // Destruction auto de temp
}

Site Reseau::site_index(int index) const{
    Site a(index, index/nx, index%ny);
    return a;
}

Site Reseau::site_xy(int x, int y) const{
    Site a(x*nx+y,x,y);
    //Si on sort du tableau, on donne une valeur par defaut
    if(x >= nx || x < 0 || y >= ny || y <0){
        a._index = -1;
        a._x = -1;
        a._y = -1;
    }
    return a;
}

Site Reseau::site_aleatoire() const{
    int x = abs(std::rand() % nx);
    int y = abs(std::rand() % ny);
    return site_xy(x,y);
}

int Reseau::nbSite(){
    return nx*ny;
}

Case Reseau::operator[](Site site) const{
    Case* val = nullptr;
    if(site._index < 0) {
        *val = case_defaut;
    }else{
        *val = tab[site._index];
    }
    return *val;
}
Case& Reseau::operator[](Site site) {
    if (site._index >= 0 && site._index < nx*ny) {
        return tab[site._index]; 
    }
    return case_defaut; 
}

vector<Cristal> Reseau::get_cristaux() const{
    return cristaux;
}

std::array<Site,8> Reseau::voisins_immediat(Site site){
    int x = site._x;
    int y = site._y;
    std::array<Site,8> sitesVoisins = {site_xy(x-1, y), site_xy(x-1, y-1), site_xy(x-1, y+1), site_xy(x, y-1), site_xy(x, y+1), site_xy(x+1, y), site_xy(x+1, y-1), site_xy(x+1, y+1)};
    return sitesVoisins;
}

void Reseau::cristallisation_1case(Site site, int type){
    // Gestion de la case
    tab[site._index].type = type;
    tab[site._index].dist_cristal = 0;
    tab[site._index].concentration = 0;
    tab[site._index].proba_cristallisation = 0;

    // Gestion du cristal
    if(type >= 0 && type < int(cristaux.size())){
        cristaux[type].listeSites.push_back(site);
        cristaux[type].nbSites ++;
    }else{
        // Création d'un nouveau cristal avec angle aléatoire
        std::vector<Site> sites;
        sites.push_back(site);
        Cristal nouveau({sites},float(rand())/RAND_MAX);
        cristaux.push_back(nouveau);
    }
}

void Reseau::affiche_SFML(sf::RenderWindow& window, float x, float y) const{
    for(int i=0; i<nx; i++){
        for(int j=0; j<ny; j++){
            sf::RectangleShape rectangle(sf::Vector2f(2,2));
            if (tab[site_xy(i,j)._index].type == -2){
                rectangle.setFillColor(sf::Color(200,200,200));
            }else if(tab[site_xy(i,j)._index].type == -1){
                rectangle.setFillColor(sf::Color(0, 100, 128));
            }else{
                rectangle.setFillColor(sf::Color(100,100,100));
            }
            rectangle.setPosition(x+(2*i), y+(2*j));
            window.draw(rectangle);
        }
    }
}


