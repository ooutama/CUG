#pragma once

#include <QVector>
#include <QMap>
#include <set>
#include <QMainWindow>
#include <vector>
#include <limits.h>
#include <fstream>
#include <math.h>
#include "arete.h"
#include "exceptions.h"


namespace CUG
{

class Graphe
{
public:
    using ListeAdjacence = QMap<Sommet::Index, Sommet::SommetsAdjacents>;

    enum Type
    {
        Empty,
        Unoriented,
        Oriented,
        Mixed
    };


    Graphe();
    ~Graphe();

    Sommet::const_ref addSommet(QString idtf = "");
    Sommet::const_ref addSommet(Sommet::Index index, QString idtf = "");
    Sommet::const_ref addSommet(Sommet::const_ref sommet);
    Arete::const_ref addArete(Sommet::const_ref firstSommet,Sommet::const_ref secondSommet,int poids = 0, bool oriented = false);
    Arete::const_ref addArete(Sommet::Index firstIndex, Sommet::Index secondIndex, int poids = 0, bool oriented = false);

    void delArete(Arete::const_ref arete);
    void delSommet(Sommet::const_ref sommet);

    void setSommetIdtf(Sommet::const_ref sommet, QString idtf);
    void setAretePoids(Arete::const_ref arete, int poids);

    bool contientSommet(Sommet::const_ref sommet) const;
    bool contientArete(Sommet::const_ref firstSommet,Sommet::const_ref secondSommet) const;

    Sommet::const_ref recupererSommet(Sommet::Index index) const;
    Arete::const_ref recupererArete(Arete::Index index) const;

    bool isEmpty() const;
    Graphe::Type getType() const;
    bool estPonderer() const;

    int compterSommets() const;
    int compterAretes() const;
    QList<Sommet*> inddeg();
    static bool compare(Sommet* sommet1,Sommet* sommet2);

    QList<Arete::Index> getAretes() const;

    ListeAdjacence getListeAdjacence() const;

    QLinkedList<Sommet *> lista();

    Sommet* trouverSommetparIndex(Sommet::Index index);
    Arete* trouverAreteparIndex(const Arete::Index& index);

private:


    QLinkedList<Sommet *> sommets;
    QLinkedList<Arete *> aretes;

    int setSommetIndex();
    void delSommetIndex(Sommet::Index i);
    bool validerSommetIndex(Sommet::Index i) const;
    bool validerAreteIndex(Arete::Index i);


    std::set<Sommet::Index> sommetIndices;

};

}
