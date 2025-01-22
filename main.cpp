#include "Reseau.hpp"
#include "Case.hpp"
#include "Cristaux.hpp"
#include<iostream>

using namespace std;


int nx = 1000;
int ny = 1000;
float dx = 1e-7;


int main(){
    Case val_defaut = Case();
    Reseau res(nx,ny,dx,val_defaut);
    sf::ContextSettings settings; settings.antialiasingLevel = 8; 
    sf::RenderWindow win (sf::VideoMode(nx,ny), "Mon super projet");
    for(int i = 0; i < nx; i++){
        for(int j = 0; j < 10; j++){
            res[res.site_xy(i,j)].set_type(-1);
        }
    }
    while (win.isOpen()){
        res.pas_de_temps(0.00001);
        win.clear(); 
        sf::Event event;
        while (win.pollEvent(event)) {
            if (event.type == sf::Event::Closed) win.close();
        }
        res.affiche_SFML(win,0,0);
        win.display();
    }
}