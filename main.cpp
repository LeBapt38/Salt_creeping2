#include "Reseau.hpp"
#include "Case.hpp"
#include "Cristaux.hpp"
#include<iostream>

using namespace std;


int nx = 1000;
int ny = 500;
float dx = 1e-7;
bool visualisation = true;
int nb_pas_tps_image = 50;
int nb_image = 1000;
string folder_data = "/mnt/c/Users/aiad_/Documents/magistère/FPT/Algo_cristallisation2/Salt_creeping2/Data/";


int main(){
    Case val_defaut = Case();
    Reseau res(nx,ny,dx,val_defaut);
    for(int i = 0; i < 100; i++){
        for(int j = 0; j < ny; j++){
            res[res.site_xy(i,j)].set_type(-1);
        }
    }
    if(visualisation){
        sf::ContextSettings settings; settings.antiAliasingLevel = 8; 
        sf::RenderWindow win (sf::VideoMode(sf::Vector2u(nx, ny)), "Mon super projet");
        while (win.isOpen()){
            for(int i = 0; i < nb_pas_tps_image; i++){
                res.pas_de_temps(0.001);
            }
            win.clear();
            while (const std::optional event = win.pollEvent()) {
                if (event->is<sf::Event::Closed>()) win.close();
            }
            res.affiche_SFML(win,0,0);
            win.display();
        }
        cout << res[res.site_xy(-1,-1)].get_type() << endl;
    }else{
        for(int i = 0; i < nb_image; i++){
            for(int j = 0; j < nb_pas_tps_image; j++){
                res.pas_de_temps(0.0001);
                if(j%100 == 0){
                    cout << j << " pas de temps depuis dernière image. \n";
                }
            }
            string nom_fichier = folder_data + "image" + to_string(i) + ".dat";
            res.enregistre_grille(nom_fichier);
            string nom_fichier_taille = folder_data + "taille_cristaux.dat";
            res.liste_taille_crist(nom_fichier_taille);
            cout << i << " images enregistées.\n";
        }
    }
    return 0;
}