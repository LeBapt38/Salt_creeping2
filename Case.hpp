#ifndef CASE_H
#define CASE_H


class Case {
    private :
        friend class Reseau;
        float dist_front;
        float dist_cristal;
        float taille_crist_proche;
        float concentration;
        float proba_cristallisation;
        int type; // -2 air, -1 eau, >= 0 cristal numéro...

    public :
        
        Case(int type, float Dist_front, float _Taille, float Dist_cristal, float concentration, float proba_cristallisation);
        Case();
        Case(const Case& old);
        Case& operator=(const Case& old);
        int get_type() const;
        void set_type(int);




};





#endif