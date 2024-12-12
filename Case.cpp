#include"Case.hpp"
#include <cstdlib>
#include <iostream>

Case::Case(int _type, float _Dist_front,float _Dist_cristal, float _concentration, float _proba_cristallisation) :
    type(_type), dist_front(_Dist_front), dist_cristal(_Dist_cristal), concentration(_concentration), proba_cristallisation(_proba_cristallisation){}

Case::Case() : Case(0){}

Case::Case(const Case& old) : type(old.type), dist_front(old.dist_front), dist_cristal(old.dist_cristal), concentration(old.concentration), proba_cristallisation(old.proba_cristallisation){}

Case& Case::operator=(const Case& old){
    Case temp(old);
    std::swap(type, temp.type);
    std::swap(dist_front, temp.dist_front);
    std::swap(dist_cristal, temp.dist_cristal);
    std::swap(concentration, temp.concentration);
    std::swap(proba_cristallisation, temp.proba_cristallisation);

    return *this;
}

int Case::get_type() const{
    return type;
}