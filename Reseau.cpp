#include "Reseau.hpp"
#include <cstdlib>
#include <array>
#include <cmath>
#include <algorithm>
//Site



//Reseau

Reseau::Reseau(int NX, int NY, float DX, Case val_def) : nx(NX), ny(NY), dx(DX), tab(nullptr), case_defaut(val_def), cristaux({}){
    tab = new Case[nx*ny];
    for(int l = 0; l < nx*ny; l++){
        tab[l] = val_def;
    }
    case_defaut.type = -1;
}

Reseau::Reseau() : Reseau(0,0,1,Case()){}

Reseau::Reseau(const Reseau& other) : nx(other.nx), ny(other.ny), dx(other.dx), tab(nullptr), case_defaut(other.case_defaut), cristaux(other.cristaux) {
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
    Site a(index, index/ny, index%ny);
    return a;
}

Site Reseau::site_xy(int x, int y) const{
    Site a(x*ny+y,x,y);
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
        // ajustement du type
        tab[site._index].type = cristaux.size();
        // Création d'un nouveau cristal avec angle aléatoire
        std::vector<Site> sites;
        sites.push_back(site);
        Cristal nouveau(sites,float(rand())/RAND_MAX);
        cristaux.push_back(nouveau);
    }
    
}

std::vector<int> Reseau::type_crist_vois(std::array<Site,8> liste_sites){
    std::vector<int> type = {};
    // On parcours les sites et on prends ceux où il y a des cristaux 
    for(Site site : liste_sites){
        if(tab[site._index].type >= 0){
            // on test si on a deja vu ce cristal
            bool test = false;
            for(int x : type){
                if(!test){
                    test = (x == tab[site._index].type);
                }
            }
            // Si on a jamais vu le cristal on l'ajoute
            if(!test){
                type.push_back(tab[site._index].type);
            }
        }
    }
    return type;
}

int Reseau::nb_bord_commun(Site site){
    int nb = 0;
    int x = site._x;
    int y = site._y;
    if(tab[site_xy(x-1,y)._index].type >= 0) nb++;
    if(tab[site_xy(x+1,y)._index].type >= 0) nb++;
    if(tab[site_xy(x,y-1)._index].type >= 0) nb++;
    if(tab[site_xy(x,y+1)._index].type >= 0) nb++;
    return nb;
}

float Reseau::energie_liaison_site(Site site, float El){
    std::array<Site,8> voisins = voisins_immediat(site);
    std::vector<int> type_cristaux_vois = type_crist_vois(voisins);
    int nb_bord_crist = nb_bord_commun(site);
    float energie = nb_bord_crist * El; // nb bord * energie de liaison
    if(type_cristaux_vois.size() == 2){
        float angle = cristaux[type_cristaux_vois[0]].orientation - cristaux[type_cristaux_vois[1]].orientation;
        float defaut_grain = joint_grain(angle);
        energie *= 1-defaut_grain;
    }else if(type_cristaux_vois.size() > 2){
        energie = -1; // les energies negative seront traite comme empechant liaison
    }
    return energie;
}

float Reseau::proba_site(Site site, float long_liaison, float T, float z0){
    float energie = energie_liaison_site(site);
    float proba = exp(energie * long_liaison / (dx * 1.38e-23 * T));
    if(energie < 0){
        proba = 0;
    }
    float CsurCsat = exp(tab[site._index].dist_cristal / z0);
    proba *= CsurCsat;
    tab[site._index].proba_cristallisation = proba;
    return proba;
}

std::vector<Site> Reseau::sites_a_traiter(std::vector<Site>& a_traiter){
    std::vector<Site> surface;
    for(int i = 0; i < nx*ny; i++){
        if(tab[i].type == -1){
            // ajout de la case
            a_traiter.push_back(site_index(i));
        }
        if(tab[i].type > -2){
            // Mis à jour de la surface
            Site site = site_index(i);
            int x = site._x;
            int y = site._y;
            if((*this)[site_xy(x-1,y)].type == -2){
                surface.push_back(site_xy(x-1,y));
            }
            if((*this)[site_xy(x+1,y)].type == -2){
                surface.push_back(site_xy(x+1,y));
            }
            if((*this)[site_xy(x,y-1)].type == -2){
                surface.push_back(site_xy(x,y-1));
            }
            if((*this)[site_xy(x,y+1)].type == -2){
                surface.push_back(site_xy(x,y+1));
            }
        }
    }
    return surface;
}

void Reseau::proximite_cristal(std::vector<Site>& sites, std::vector<float>& distances, std::vector<Cristal>& cristaux_proches){
    for(Site site : sites){
        float d_min2 = nx*nx + ny*ny; 
        Cristal crist_proche;
        // Utilise l'heuristique que le cristal le plus proche est celui ayant le premier site le plus proche, sinon, augmente trop la complexité
        for(Cristal crist : cristaux){
            float d2 = (site._x - crist.listeSites[0]._x) * (site._x - crist.listeSites[0]._x);
            d2 += (site._y - crist.listeSites[0]._y) * (site._y - crist.listeSites[0]._y);
            if(d2 < d_min2){
                d_min2 = d2;
                crist_proche = crist;
            }
        }
        for(Site site_test : crist_proche.listeSites){
            float d2 = (site._x - site_test._x) * (site._x - site_test._x);
            d2 += (site._y - site_test._y) * (site._y - site_test._y);
            if(d2 < d_min2){
                d_min2 = d2;
            }
        }
        float d = dx * sqrt(d_min2);
        distances.push_back(d);
        cristaux_proches.push_back(crist_proche);
    }
}

void Reseau::proximite_gros_cristal(std::vector<Site>& sites, std::vector<float>& distances, std::vector<Site>& cristaux_proches){
    for(Site site : sites){
        float d_min2 = nx*nx + ny*ny; 
        Cristal crist_proche;
        // Utilise l'heuristique que le cristal le plus proche est celui ayant le premier site le plus proche, sinon, augmente trop la complexité
        for(Cristal crist : cristaux){
            if(crist.nbSites*dx*dx > 1e-12){
                float d2 = (site._x - crist.listeSites[0]._x) * (site._x - crist.listeSites[0]._x);
                d2 += (site._y - crist.listeSites[0]._y) * (site._y - crist.listeSites[0]._y);
                if(d2 < d_min2){
                    d_min2 = d2;
                    crist_proche = crist;
                }
            }
        }
        Site site_proche;
        for(Site site_test : crist_proche.listeSites){
            float d2 = (site._x - site_test._x) * (site._x - site_test._x);
            d2 += (site._y - site_test._y) * (site._y - site_test._y);
            if(d2 < d_min2){
                d_min2 = d2;
                site_proche = site_test;
            }
        }
        float d = dx * sqrt(d_min2);
        distances.push_back(d);
        cristaux_proches.push_back(site_proche);
    }
}

void Reseau::pas_de_temps(float proportion_cristalliser){
    // extraction des cases a traiter
    std::vector<Site> a_traiter;
    std::vector<Site> surface = sites_a_traiter(a_traiter);
    // calcul des probabilites de cristallisations
    float normalisation_proba = 0;
    for(Site site : a_traiter){
        normalisation_proba += proba_site(site);
    }
    float nb_site_a_cristalliser = 1 + surface.size() * proportion_cristalliser;
    normalisation_proba /= nb_site_a_cristalliser;
    // Cristallisation
    for(Site site : a_traiter){
        tab[site._index].proba_cristallisation /= normalisation_proba;
        float a = float(rand()) / RAND_MAX;
        if(a < tab[site._index].proba_cristallisation){
            int x = site._x;
            int y = site._y;
            std::array<int,4> type_vois = { tab[site_xy(x-1,y)._index].type,
                                            tab[site_xy(x+1,y)._index].type,
                                            tab[site_xy(x,y-1)._index].type,
                                            tab[site_xy(x,y+1)._index].type};
            int *type = std::max_element(type_vois.begin(), type_vois.end());
            cristallisation_1case(site,*type);
        }
    }
    // Calcul la distance de ces cases
    std::vector<float> distance;
    std::vector<Cristal> cristal_proche;
    proximite_cristal(surface, distance, cristal_proche);
    for(size_t i = 0; i < surface.size(); i++){
        // Ajuste la surface pour que le menisque suive le cristal
        if(tab[surface[i]._index].type == -2){
            if(distance[i] < sqrt(cristal_proche[i].nbSites) * 3 * dx){
                tab[surface[i]._index].type = -1;
            }
        }
    }
    // Recherche des petits cristaux (moins de 1µm2)
    std::vector<Site> petits_cristaux;
    for(Cristal crist : cristaux){
        if(crist.nbSites*dx*dx < 1e-13){
            for(Site site : crist.listeSites){
                surface.push_back(site);
            }
        }
    }
    std::vector<float> dist;
    std::vector<Site> site_proche;
    proximite_gros_cristal(petits_cristaux, dist, site_proche);
    // Ajustement des petits cristaux
    for(size_t i = 0; i < petits_cristaux.size(); i++){
        float z = dist[i];
        float r = sqrt(cristaux[tab[petits_cristaux[i]._index].type].nbSites)*dx;
        float h = sqrt(cristaux[tab[site_proche[i]._index].type].nbSites)*dx;
        if(r*z/sqrt(h) < 1.122e-10){
            cout << "yes" << endl;
            //insertion du cristal au nouvel emplacement
            int x = site_proche[i]._x;
            int y = site_proche[i]._y;
            if((*this)[site_xy(x-1,y)].type == -1 && x > 0){
                cristallisation_1case(site_xy(x-1,y), (*this)[site_xy(x,y)].type);
            }else if((*this)[site_xy(x+1,y)].type == -1 && x <= nx){
                cristallisation_1case(site_xy(x+1,y), (*this)[site_xy(x,y)].type);
            }else if((*this)[site_xy(x,y-1)].type == -1 && y > 0){
                cristallisation_1case(site_xy(x,y-1), (*this)[site_xy(x,y)].type);
            }else if((*this)[site_xy(x,y+1)].type == -1 && y <= ny){
                cristallisation_1case(site_xy(x,y+1), (*this)[site_xy(x,y)].type);
            }
            // destruction du cristal à l'ancien emplacement
            int index = petits_cristaux[i]._index;
            tab[index].type = -1;
            Cristal crist = cristaux[tab[petits_cristaux[i]._index].type];
            int l;
            for(size_t k = 0; k < crist.listeSites.size(); k++){
                if(crist.listeSites[k]._index == index){
                    l = k;
                }
            }
            crist.listeSites.erase(crist.listeSites.begin() + l);
        }
    }


}

void Reseau::affiche_SFML(sf::RenderWindow& window, float x, float y) const{
    for(int i=0; i<nx; i++){
        for(int j=0; j<ny; j++){
            sf::RectangleShape rectangle(sf::Vector2f(1,1));
            if (tab[site_xy(i,j)._index].type == -2){
                rectangle.setFillColor(sf::Color(200,200,200));
            }else if(tab[site_xy(i,j)._index].type == -1){
                rectangle.setFillColor(sf::Color(0, 100, 128));
            }else{
                int col = 100 + tab[site_xy(i,j)._index].type;
                rectangle.setFillColor(sf::Color(col,col,col));
            }
            rectangle.setPosition(x+i, y+j);
            window.draw(rectangle);
        }
    }
}


