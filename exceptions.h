#pragma once

#include <exception>


namespace CUG
{

class mauvais_sommet: public std::exception
{
    virtual const char* what() const throw()
    {
        return "Erreur de sommet!";
    }
};

class mauvais_arete: public std::exception
{
    virtual const char* what() const throw()
    {
        return "Erreur d'Arete!";
    }
};

}
