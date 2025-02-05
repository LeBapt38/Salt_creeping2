#include "Reseau.hpp"
#include "Case.hpp"
#include "Cristaux.hpp"
#include<iostream>

using namespace std;


int nx = 1000;
int ny = 1000;
float dx = 1e-7;
bool visualisation = false;
int nb_pas_tps_image = 500;
int nb_image = 100;
string folder_data = "/mnt/c/Users/aiad_/Documents/magistère/FPT/Algo_cristallisation2/Salt_creeping2/Data/";


int main(){
    Case val_defaut = Case();
    Reseau res(nx,ny,dx,val_defaut);
    for(int i = 0; i < nx; i++){
        for(int j = 0; j < 10; j++){
            res[res.site_xy(i,j)].set_type(-1);
        }
    }
    if(visualisation){
        sf::ContextSettings settings; settings.antialiasingLevel = 8; 
        sf::RenderWindow win (sf::VideoMode(nx,ny), "Mon super projet");
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