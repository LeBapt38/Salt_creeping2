#include "Reseau.hpp"
#include "Case.hpp"
#include "Cristaux.hpp"
#include<iostream>

using namespace std;

// Paramettres for the simulation
int nx = 700;
int ny = 300;
float dx = 1e-7;
bool visualisation = false; // Set to true for visualisation with SFML
int nb_pas_tps_image = 10000;
int nb_image = 10;
string folder_data = "/Users/baptisteguilleminot/Documents/FPT/Salt_creeping2/Data_J/";
// Physical constants
float J = 1e-3 / (3.6 * 0.5); // Evaporation constant
float El = 6.4e-18; // Energy of the link in Joules
float T = 273.15 + 15; // Temperature in Kelvin 
float long_liaison = 2.36e-10; // Link length in meters


int main(){
    if(visualisation){
        Case val_defaut = Case();
        Reseau res(nx,ny,dx,val_defaut);
        for(int i = 0; i < 100; i++){
            for(int j = 0; j < ny; j++){
                res[res.site_xy(i,j)].set_type(-1);
            }
        }
        sf::ContextSettings settings; settings.antiAliasingLevel = 8; 
        sf::RenderWindow win (sf::VideoMode(sf::Vector2u(nx, ny)), "Mon super projet");
        while (win.isOpen()){
            for(int i = 0; i < nb_pas_tps_image; i++){
                res.pas_de_temps(0.001, long_liaison, T, J, El);
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
        #pragma omp parallel for num_threads(5)
        for(int k = 1; k <= 15; k++){
            Case val_defaut = Case();
            Reseau res(nx,ny,dx,val_defaut);
            for(int i = 0; i < 50; i++){
                for(int j = 0; j < ny; j++){
                    res[res.site_xy(i,j)].set_type(-1);
                }
            }
            float J_thread = J * (0.1 * k); //Increase temperature for each parallel thread
            for(int i = 0; i < nb_image; i++){
                for(int j = 0; j < nb_pas_tps_image; j++){
                    res.pas_de_temps(0.001, long_liaison, T, J_thread, El);
                    if(j%1000 == 0){
                        cout << j << " pas de temps depuis dernière image. \n";
                    }
                }
                string nom_fichier = folder_data + "image" + to_string(i+1) + "_J" + to_string(J_thread) + ".dat";
                res.enregistre_grille(nom_fichier);
                cout << i + 1 << " images enregistées.\n";
            }
            string nom_fichier_taille = folder_data + "taille_cristaux" + to_string(J_thread) + ".dat";
            res.liste_taille_crist(nom_fichier_taille);
            cout << "Thread " << k << " finished with evaporation: " << J_thread << endl;
        }
    }
    return 0;
}