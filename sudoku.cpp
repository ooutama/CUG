#include "sudoku.h"
#include "surinterface.h"
#include <QLabel>
#include <QInputDialog>
#include <QToolBar>
#include <QTime>
#include <QTimer>
#include <QMouseEvent>
#include <QVBoxLayout>

Sudoku::Sudoku(QWidget* parent)
    : QMainWindow(parent)
{

    sudokuBoard = new SurInterface;
    sudokuBoard->setDragMode(QGraphicsView::NoDrag);
    sudokuBoard->setFixedSize(541,541);
    sudokuBoard->resolutionSudoku();
    sudokuBoard->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    sudokuBoard->setHorizontalScrollBarPolicy((Qt::ScrollBarAlwaysOff));
    sudokuBoard->centerOn(590,340);
    sudokuBoard->setStyleSheet("background-image:url(:icons/grid.jpg);");
    this->setWindowIcon(QIcon(":icons/cuglogo.png"));

    QMainWindow::setCentralWidget(sudokuBoard);
    sudtools = new QToolBar(tr("&SudTools"));
    sudtools->setIconSize(QSize(95, 50));
    sudtools->setAllowedAreas(Qt::TopToolBarArea);
    sudtools->setStyleSheet("background:rgb(66,66,66);");

    checker = new QAction(QIcon(":/icons/checker.png"), tr("Verifier"), this);
    checker->setStatusTip(tr("Verifier votre solution"));
    connect(checker, SIGNAL(triggered(bool)), this, SLOT(checkerf()));

    reset = new QAction(QIcon(":/icons/reset.png"), tr("Recommancer"), this);
    reset->setStatusTip(tr("Recommancer"));
    connect(reset, SIGNAL(triggered(bool)), this, SLOT(resetf()));

    visi = new QAction(QIcon(":/icons/visi.png"), tr("Graphe visible"), this);
    visi->setStatusTip(tr("Voir le graphe de sudoku"));
    visi->setCheckable(true);
    connect(visi, SIGNAL(triggered(bool)), this, SLOT(visif()));

    inivisi = new QAction(QIcon(":/icons/invis.png"), tr("Graphe invisible"), this);
    inivisi->setStatusTip(tr("Masquer le graphe de sudoku"));
    inivisi->setCheckable(true);
    connect(inivisi, SIGNAL(triggered(bool)), this, SLOT(invisif()));

    demo = new QAction(QIcon(":/icons/demo.png"), tr("Demonstration"), this);
    demo->setStatusTip(tr("Voir la demonstration"));
    connect(demo, SIGNAL(triggered(bool)), this, SLOT(demof()));

    sudtools->addAction(checker);
    sudtools->addAction(reset);
    sudtools->addAction(visi);
    sudtools->addAction(inivisi);
    sudtools->addAction(demo);

    this->addToolBar(sudtools);

}

Sudoku::~Sudoku()
{

}

void Sudoku::checkerf()
{
    sudokuBoard->verifiersudoku();
}
void Sudoku::resetf(){
    sudokuBoard->resetsudo();
}
void Sudoku::visif(){
    sudokuBoard->graphevisi();
}

void Sudoku::invisif(){
    sudokuBoard->grapheinvis();
}

void Sudoku::demof()
{
    sudokuBoard->demo();
}

