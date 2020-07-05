#pragma once

#include <QString>
#include <QLinkedList>
#include <QPair>


namespace CUG
{

class Arete;


class Sommet
{
public:
    using const_ref = const Sommet&;
    using Index = int;
    using SommetsAdjacents = QList< QPair<int, Sommet::Index> >;

    Sommet(Sommet::Index i);
    Sommet(Sommet::Index i, const QString& str);
    ~Sommet();



    QString getText() const;
    void setText(const QString& str);

    Sommet::Index getIndex() const;
    void setIndex(Sommet::Index i);
    SommetsAdjacents getSommetsAdjacents() const;
    SommetsAdjacents getSommetsAdjacentspC() const;

    int getDegree() const;
    int DegreePlus() const;
    int DegreeMoins() const;

    friend bool operator== (Sommet::const_ref first, Sommet::const_ref second);
    friend bool operator!= (Sommet::const_ref first, Sommet::const_ref second);

private:

    void addArete(Arete* arete);
    void delArete(Arete* arete);

    Sommet::Index index;
    QString text;
    QLinkedList<Arete*> aretes;

    friend class Graphe;
    friend class Arete;
};

bool operator== (Sommet::const_ref first, Sommet::const_ref second);
bool operator!= (Sommet::const_ref first, Sommet::const_ref second);
}
