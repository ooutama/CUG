#include "arete.h"
#include "sommet.h"

using namespace CUG;


Arete::Arete(Sommet* start, Sommet* end, int poids, bool oriented)
{
    SommetSource(start);
    SommetDestination(end);
    setPoids(poids);
    this->oriented = oriented;
}

Arete::~Arete()
{
    Sommetsrc->delArete(this);
    Sommetdest->delArete(this);
}

Arete::Index Arete::getIndex() const
{
    return Index(Sommetsrc->getIndex(), Sommetdest->getIndex());
}

bool Arete::isOriented() const
{
    return oriented;
}

bool Arete::isLoop() const
{
    return Sommetsrc == Sommetdest;
}

void Arete::SommetSource(Sommet *sommet)
{
    Sommetsrc = sommet;
    sommet->addArete(this);
}

void Arete::SommetDestination(Sommet *sommet)
{
    Sommetdest = sommet;
    sommet->addArete(this);
}

int Arete::getPoids() const
{
    return poids;
}

void Arete::setPoids(int w)
{
    poids = w;
}

Sommet::const_ref Arete::getAdjacentSommet(Sommet::const_ref sommet) const
{
    if (!isLoop())
    {
        if (sommet == *Sommetsrc)
            return *Sommetdest;
        else if (sommet == *Sommetdest && !isOriented())
            return *Sommetsrc;
    }
    throw mauvais_sommet();
}

Sommet::const_ref Arete::getAdjacentSommetpC(Sommet::const_ref sommet) const
{
    if (!isLoop())
    {
        if (sommet == *Sommetsrc)
            return *Sommetdest;
        else if (sommet == *Sommetdest)
            return *Sommetsrc;
    }
    throw mauvais_sommet();
}

bool CUG::operator== (const Arete& firstArete, const Arete& secondArete)
{
    bool equalSommetsStraight = (firstArete.Sommetsrc == secondArete.Sommetsrc) &&
                              (firstArete.Sommetdest == secondArete.Sommetdest);
    bool equalSommetsMix = (firstArete.Sommetdest == secondArete.Sommetsrc) &&
                         (firstArete.Sommetsrc == secondArete.Sommetdest);
    bool equalPoids = (firstArete.getPoids() == secondArete.getPoids());
    return (equalSommetsStraight || equalSommetsMix) && equalPoids;
}

bool CUG::operator==(const Arete::Index &i, const Arete::Index &j)
{
    bool equalStraight = (i.first == j.first) && (i.second == j.second);
    bool equalReverse = (i.first == j.second) && (i.second == j.first);
    return (equalStraight || equalReverse);
}
