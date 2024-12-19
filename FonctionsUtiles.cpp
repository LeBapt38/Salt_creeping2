#include <cmath>
#include "FonctionsUtiles.hpp"


float joint_grain(float theta){
    float A = 0.23;
    float E = 0;
    // On s'assure que theta est compris entre 0 et pi/2
    theta = fmod(theta,M_PI/2);
    if(theta < 0){theta += M_PI/2;}
    // On traite les trois cas du modèle
    if(theta > 65*M_PI/180){
        theta = M_PI/2 -theta;
    }else{
        theta = 25*M_PI/180;
    }
    // On c'est ramener au cas 1 pour toutes les situations donc :
    E = theta * (A-log(theta));
    return E;
}


float concentration(float dist, float tailleCristal){
    float J = 1e-3 / (3.6*0.5);
    float h = sqrt(tailleCristal);
    float CsurCsat = sqrt(J / (1000*h*1.9e-9))*dist;
    CsurCsat = exp(CsurCsat);
    return CsurCsat;
}