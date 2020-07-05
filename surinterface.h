#pragma once

#include <QGraphicsView>
#include <QTimer>
#include <vector>
#include <list>
#include "graphe.h"
#include "arete.h"
#include "mainwindow.h"
#include "aretegraphics.h"
#include "sommetgraphics.h"
#include "sommet.h"
#include <QTextEdit>
#include <QVBoxLayout>
#include <QInputDialog>
class SurInterface : public QGraphicsView
{
    Q_OBJECT
public:
    using SommetPair = QPair<SommetGraphics *, SommetGraphics *>;

    enum
    {
        defaultMode = 0,
        renommerMode,
        sommetCreationMode,
        pondererMode,
        areteCreationMode,
        arcCreationMode,
        deletionMode
    };

    SurInterface(QWidget* parent = 0);

    void mouseMoveEvent(QMouseEvent* event);
    void mousePressEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void focusOutEvent(QFocusEvent* event);
    void manageAreteCreation(const QPoint& location);
    void setElementContent(QGraphicsItem *item);
    void creerSommet(const QPoint& pos, const QString &idtf = "");
    void deleteElementAtPosition(const QPoint& pos);
    void deleteElement(QGraphicsItem* item);
    void deleteSelectedElements();

    void clearSelection();

    void toggleMode(int mode, bool toggled);

    bool saveGraphToFile(const QString &saveFileName);
    bool loadGraphFromFile(const QString &loadFileName);

    void deleteGraph();
    void graphevisi();
    void grapheinvis();
    //donner a chaque couleur un indice
    void couleursindi();
    //verifier la solution
    void verifiersudoku();
    void resetsudo();

    void nbrchromatique(int chrom);

private slots:
    void creerArete(SommetGraphics *firstSommet, SommetGraphics *secondSommet, int poids = 0, bool oriented = false);
    void creerArete(int firstSommetIndex, int secondSommetIndex, int poids = 0, bool oriented = false);

    void toggleSelectionMode(bool isToggled);
    void toggleRenommerMode(bool isToggled);
    void toggleSommetCreationMode(bool isToggled);
    void togglePondererMode(bool isToggled);
    void toggleArcCreationMode(bool isToggled);
    void toggleAreteCreationMode(bool isToggled);
    void toggleDeletionMode(bool isToggled);

    void colorationGlouton();
    void colorationDsatur();
    void colorationWelshPowell();
    void resolutionSudoku();
    void colorDsudoko();
    void solvesudo();
    void generateur();
    void demo();
    //backtracking
    bool estSecuriser(int v,int color[],int c);
    bool utilColoration(int m, int color[], int v);


    bool check(QList<int> v, int a);

private:

    void clearDrawingLine();

    SommetGraphics* getTopmostSommetItem(QList<QGraphicsItem*> items);
    QGraphicsItem* getSelectedItem();
    QList<SommetGraphics *> getSommets() const;
    QList<AreteGraphics *> getAretes() const;

    std::function<SommetGraphics *(QGraphicsItem *)> aSommet;
    std::function<AreteGraphics *(QGraphicsItem *)> aArete;

    const int WIDTH = 1024;
    const int HEIGHT = 768;

    CUG::Graphe* graphe;
    CUG::Sommet* sommet;
    QGraphicsLineItem* drawingLine;
    SommetPair selectedSommets;
    int toggledMode;

    friend class Sudoku;

    QTextEdit* txt =new QTextEdit();

    QList<QPair<int,QColor>> Colorindex;
    QList<int> lesindicesdegrille;
    QList<int> resetindis;

};
