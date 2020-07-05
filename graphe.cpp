#include <queue>

#include "graphe.h"

using namespace CUG;


Graphe::Graphe()
{
}

Graphe::~Graphe()
{
    qDeleteAll(aretes);
    aretes.clear();
    qDeleteAll(sommets);
    sommets.clear();
}

Sommet::const_ref Graphe::addSommet(QString idtf)
{
    Sommet* nouveauSommet;
    if (idtf.isEmpty())
        nouveauSommet = new Sommet(setSommetIndex());
    else
        nouveauSommet = new Sommet(setSommetIndex(), idtf);
    sommets.append(nouveauSommet);
    return *nouveauSommet;
}

Sommet::const_ref Graphe::addSommet(Sommet::Index index, QString idtf)
{
    if (validerSommetIndex(index))
    {
        sommetIndices.insert(index);
        Sommet* nouveauSommet;
        if (idtf.isEmpty())
            nouveauSommet = new Sommet(index);
        else
            nouveauSommet = new Sommet(index, idtf);
        sommets.append(nouveauSommet);
        return *nouveauSommet;
    }
    else
        throw mauvais_sommet();
}

Sommet::const_ref Graphe::addSommet(Sommet::const_ref sommet)
{
    return addSommet(sommet.getText());
}

void Graphe::delSommet(Sommet::const_ref sommet)
{
    Sommet* sommet_ptr = trouverSommetparIndex(sommet.getIndex());
    delSommetIndex(sommet.getIndex());
    sommets.removeOne(sommet_ptr);
    delete sommet_ptr;
}

void Graphe::setSommetIdtf(Sommet::const_ref sommet, QString idtf)
{
    Sommet* sommet_ptr = trouverSommetparIndex(sommet.getIndex());
    if (sommet_ptr)
        sommet_ptr->setText(idtf);
}




Arete::const_ref Graphe::addArete(Sommet::const_ref firstSommet,Sommet::const_ref secondSommet,int poids,bool oriented)
{
    if (!contientArete(firstSommet, secondSommet))
    {
        Arete* nouveauArete = new Arete(trouverSommetparIndex(firstSommet.getIndex()),trouverSommetparIndex(secondSommet.getIndex()),poids,oriented);
        aretes.append(nouveauArete);
        return *nouveauArete;
    }
    else
        throw mauvais_arete();
}

Arete::const_ref Graphe::addArete(Sommet::Index firstIndex, Sommet::Index secondIndex, int poids, bool oriented)
{
    Sommet* firstSommet = trouverSommetparIndex(firstIndex);
    Sommet* secondSommet = trouverSommetparIndex(secondIndex);

    if (firstSommet && secondSommet)
        return addArete(*firstSommet, *secondSommet, poids, oriented);
    else
        throw mauvais_arete();
}

Sommet* Graphe::trouverSommetparIndex(Sommet::Index index)
{
    foreach (Sommet* sommet, sommets)
        if (sommet->getIndex() == index)
            return sommet;
    return NULL;
}

Arete* Graphe::trouverAreteparIndex(const Arete::Index &index)
{
    foreach (Arete* arete, aretes)
        if (arete->getIndex() == index)
            return arete;
    return NULL;
}

void Graphe::delArete(Arete::const_ref arete)
{
    Arete* arete_ptr = trouverAreteparIndex(arete.getIndex());
    aretes.removeOne(arete_ptr);
    delete arete_ptr;
}

void Graphe::setAretePoids(Arete::const_ref arete, int poids)
{
    Arete* arete_ptr = trouverAreteparIndex(arete.getIndex());
    if (arete_ptr)
        arete_ptr->setPoids(poids);
}

bool Graphe::contientArete(Sommet::const_ref firstSommet, Sommet::const_ref secondSommet) const
{
    Arete::Index areteIndex(firstSommet.getIndex(), secondSommet.getIndex());
    foreach (Arete* arete, aretes)
    {
        if (arete->getIndex() == areteIndex)
        {
            if (arete->isOriented())
                return arete->getIndex().first == areteIndex.first;
            else
                return true;
        }
    }
    return NULL;
}

Sommet::const_ref Graphe::recupererSommet(Sommet::Index index) const
{
    foreach (Sommet* sommet, sommets)
        if (sommet->getIndex() == index)
            return *sommet;
    throw mauvais_sommet();
}

Arete::const_ref Graphe::recupererArete(Arete::Index index) const
{
    foreach (Arete* arete, aretes)
        if (arete->getIndex() == index)
            return *arete;
    throw mauvais_arete();
}

bool Graphe::isEmpty() const
{
    return compterSommets() == 0;
}

Graphe::Type Graphe::getType() const
{
    bool aDesArcs = false;
    bool aDesAretes = false;

    foreach (Arete* arete, aretes) {
        if (arete->isOriented())
            aDesArcs = true;
        else
            aDesAretes = true;
    }

    if (aDesArcs && aDesAretes)
        return Type::Mixed;
    else if (aDesArcs)
        return Type::Oriented;
    else if (aDesAretes)
        return Type::Unoriented;
    else
        return Type::Empty;
}

bool Graphe::estPonderer() const
{
    foreach (Arete* arete, aretes)
        if (arete->getPoids() != 0)
            return true;

    return false;
}

int Graphe::compterSommets() const
{
    return sommets.size();
}

int Graphe::compterAretes() const
{
    return aretes.size();
}

QList<Arete::Index> Graphe::getAretes() const
{
    QList<Arete::Index> areteIndices;

    foreach (Arete* arete, aretes)
        areteIndices.append(arete->getIndex());

    return areteIndices;
}

// added
QLinkedList<Sommet*> Graphe::lista(){
    QLinkedList<Sommet*> somm;
    somm=sommets;
    return somm;
}

bool Graphe::compare(Sommet *sommet1, Sommet *sommet2){
    Graphe::Type type = Type();
    if(type == Graphe::Oriented)
        return ((sommet1->DegreePlus())+(sommet1->DegreeMoins()))>((sommet2->DegreePlus())+(sommet2->DegreeMoins()));
    else
        return (sommet1->getDegree())>(sommet2->getDegree());
}

Graphe::ListeAdjacence Graphe::getListeAdjacence() const
{
    ListeAdjacence list;
    foreach (Sommet::Index sommetIndex, sommetIndices)
    {
        Sommet::const_ref sommet = recupererSommet(sommetIndex);
        QList< QPair<int, Sommet::Index> > sommetsAdjacents = sommet.getSommetsAdjacents();
        list.insert(sommet.getIndex(), sommetsAdjacents);
    }
    return list;
}

int Graphe::setSommetIndex()
{
    Sommet::Index i = 0;
    while (!validerSommetIndex(i))
        i++;
    sommetIndices.insert(i);
    return i;
}
void Graphe::delSommetIndex(Sommet::Index i)
{
    std::set<Sommet::Index>::iterator erased = sommetIndices.erase(sommetIndices.find(i));

    std::set<Sommet::Index> shiftedIndices;
    for (std::set<Sommet::Index>::iterator it = erased; it != sommetIndices.end(); it++)
    {
        Sommet::Index index = *it;
        trouverSommetparIndex(index)->setIndex(index - 1);
        shiftedIndices.insert(index - 1);
    }
    sommetIndices.erase(erased, sommetIndices.end());
    sommetIndices.insert(shiftedIndices.begin(), shiftedIndices.end());
}

bool Graphe::validerSommetIndex(Sommet::Index i) const
{
     return !((i < 0) || (sommetIndices.find(i) != sommetIndices.end()));

}

bool Graphe::validerAreteIndex(Arete::Index i)
{
    return !validerSommetIndex(i.first) || !validerSommetIndex(i.second);
}
