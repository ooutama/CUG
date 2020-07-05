#pragma once

#include <QPair>

#include "sommet.h"
#include "exceptions.h"


namespace CUG
{

class Arete
{
public:
    using const_ref = const Arete&;
    using Index = QPair<Sommet::Index, Sommet::Index>;

    Arete(Sommet* start, Sommet* end, int poids = 0, bool oriented = false);
    ~Arete();

    Arete::Index getIndex() const;
    bool isOriented() const;
    bool isLoop() const;

    int getPoids() const;
    void setPoids(int w);

    Sommet::const_ref getAdjacentSommet(Sommet::const_ref sommet) const;
    Sommet::const_ref getAdjacentSommetpC(Sommet::const_ref sommet) const;

    friend bool operator== (const Arete& firstArete, const Arete& secondArete);

private:
    void SommetSource(Sommet* sommet);
    void SommetDestination(Sommet* sommet);
    Sommet* Sommetsrc;
    Sommet* Sommetdest;
    bool oriented;
    int poids;
    friend class Graphe;
    friend class Sommet;
};

bool operator== (const Arete& firstArete, const Arete& secondArete);
bool operator== (const Arete::Index& first, const Arete::Index& second);

}
