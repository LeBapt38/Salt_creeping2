#include "Reseau.hpp"
#include "Case.hpp"
#include "Cristaux.hpp"
#include<iostream>

using namespace std;


int nx = 100;
int ny = 10;
float dx = 1;


int main(){
    Case val_defaut = Case();
    Reseau res(nx,ny,dx,val_defaut);
    for(int x = 0; x < nx; x++){
        for(int y = 0; y < ny/2; y++){
            res[res.site_xy(x,y)].set_type(-2);
        }
    }
    for(int x = nx/2; x < nx; x+=2){
        for(int y = ny/2; y < ny; y++){
            res.cristallisation_1case(res.site_xy(x,y), x);
        }
    }
    cout << res[res.site_xy(0,0)].get_type() << res[res.site_xy(1,0)].get_type() << res[res.site_xy(0,8)].get_type() << endl;
    cout << res.energie_liaison_site(res.site_xy(nx/2 - 1, ny/2)) << endl;
    sf::ContextSettings settings; settings.antialiasingLevel = 8; 
    sf::RenderWindow win (sf::VideoMode(nx*2,ny*2), "Mon super projet");
    while (win.isOpen()){
        win.clear(); 
        sf::Event event;
        while (win.pollEvent(event)) {
            if (event.type == sf::Event::Closed) win.close();
        }
        res.affiche_SFML(win,0,0);
        win.display();
    }
}