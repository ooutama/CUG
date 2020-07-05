#include <QtAlgorithms>
#include <QList>

#include "sommet.h"
#include "arete.h"

using namespace CUG;


Sommet::Sommet(Index i)
{
    setIndex(i);
    setText(QString::number(getIndex()));
}

Sommet::Sommet(Index i, const QString& str)
{
    setIndex(i);
    setText(str);
}

Sommet::~Sommet()
{
    qDeleteAll(aretes);
}

Sommet::Index Sommet::getIndex() const
{
    return index;
}

void Sommet::addArete(Arete *arete)
{
    if (arete && !aretes.contains(arete))
        aretes.append(arete);
}

void Sommet::delArete(Arete *arete)
{
    if (arete)
        aretes.removeOne(arete);
}

Sommet::SommetsAdjacents Sommet::getSommetsAdjacents() const
{
    SommetsAdjacents sommetsAdjacents;
    foreach (Arete* arete, aretes)
    {
        try
        {
            QPair<int, Sommet::Index> adjacent;
            adjacent.first = arete->getPoids();
            adjacent.second = arete->getAdjacentSommet(*this).getIndex();
            sommetsAdjacents.append(adjacent);
        }
        catch (const mauvais_sommet& e)
        {
            continue;
        }
    }
    return sommetsAdjacents;
}

Sommet::SommetsAdjacents Sommet::getSommetsAdjacentspC() const
{
    SommetsAdjacents sommetsAdjacents;
    foreach (Arete* arete, aretes)
    {
        try
        {
            QPair<int, Sommet::Index> adjacent;
            adjacent.first = arete->getPoids();
            adjacent.second = arete->getAdjacentSommetpC(*this).getIndex();
            sommetsAdjacents.append(adjacent);
        }
        catch (const mauvais_sommet& e)
        {
            continue;
        }
    }
    return sommetsAdjacents;
}

int Sommet::getDegree() const
{
    int degree = 0;

    foreach (Arete* arete, aretes)
    {
        if (arete->isLoop())
            degree += 2;
        else
            degree += 1;
    }

    return degree;
}

int Sommet::DegreePlus() const
{
    int Degreeplus = 0;

    foreach (Arete* arete, aretes)
        if (arete->isOriented() && index == arete->getIndex().second)
            ++Degreeplus;

    return Degreeplus;
}

int Sommet::DegreeMoins() const
{
    int Degreemoins = 0;

    foreach (Arete* arete, aretes)
        if (arete->isOriented() && index == arete->getIndex().first)
            ++Degreemoins;

    return Degreemoins;
}

void Sommet::setIndex(Index i)
{
    index = i;
}

void Sommet::setText(const QString &str)
{
    text = str;
}

QString Sommet::getText() const
{
    return text;
}

bool CUG::operator==(Sommet::const_ref first, Sommet::const_ref second)
{
    bool equalAretes = std::is_permutation(first.aretes.begin(), first.aretes.end(), second.aretes.begin());
    bool equalIndices = (first.getIndex() == second.getIndex());
    bool equalText = (first.getText() == second.getText());
    return (equalAretes && equalIndices && equalText);
}

bool CUG::operator!=(Sommet::const_ref first, Sommet::const_ref second)
{
    return !(first == second);
}
