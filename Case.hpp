#ifndef CASE_H
#define CASE_H


class Case {
    private :
        friend class Reseau;
        int type;
        float dist_front;
        float dist_cristal;
        float concentration;
        float proba_cristallisation;

    public :
        Case(int type, float Dist_front = 0,float Dist_cristal = 0, float concentration = 0, float proba_cristallisation = 0);
        Case();
        Case(const Case& old);
        Case& operator=(const Case& old);
        int get_type() const;




};





#endif