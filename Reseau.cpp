#include "Reseau.hpp"
#include <cstdlib>
#include <array>
#include <cmath>
//Site



//Reseau

Reseau::Reseau(int NX, int NY, Case val_def) : nx(NX), ny(NY), tab(nullptr), case_defaut(val_def){
    tab = new Case[nx*ny];
    for(int l = 0; l < nx*ny; l++){
        tab[l] = val_def;
    }
}

Reseau::Reseau() : Reseau(0,0,Case()){}

Reseau::Reseau(const Reseau& other) : nx(other.nx), ny(other.ny), tab(nullptr), case_defaut(other.case_defaut){
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
    Case* val;
    if(site._index < 0) {
        *val = case_defaut;
    }else{
        *val = tab[site._index];
    }
    return *val;
}
Case& Reseau::operator[](Site site){
        Case val;
    if(site._index < 0) {
        val = case_defaut;
    }else{
        val = tab[site._index];
    }
    return val;
}

std::array<Site,8> Reseau::voisins_immediat(Site site){
    int x = site._x;
    int y = site._y;
    std::array<Site,8> sitesVoisins = {site_xy(x-1, y), site_xy(x-1, y-1), site_xy(x-1, y+1), site_xy(x, y-1), site_xy(x, y+1), site_xy(x+1, y), site_xy(x+1, y-1), site_xy(x+1, y+1)};
    return sitesVoisins;
}

void Reseau::affiche_SFML(sf::RenderWindow& window, float x, float y) const{
    for(int i=0; i<nx; i++){
        for(int j=0; j<ny; j++){
            sf::RectangleShape rectangle(sf::Vector2f(2,2));
            if (tab[site_xy(i,j)._index].type == -1){
                rectangle.setFillColor(sf::Color(200,200,200));
            }else if(tab[site_xy(i,j)._index].type ==0){
                rectangle.setFillColor(sf::Color(0, 100, 128));
            }else{
                rectangle.setFillColor(sf::Color(100,100,100));
            }
            rectangle.setPosition(x+(2*i), y+(2*j));
            window.draw(rectangle);
        }
    }
}


