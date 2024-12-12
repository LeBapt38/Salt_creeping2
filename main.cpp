#include "Reseau.hpp"


int nx = 10;
int ny = 10;


int main(){
    Case val_defaut = Case();
    Reseau res(nx,ny,val_defaut);
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