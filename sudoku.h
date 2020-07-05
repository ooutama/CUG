#pragma once

#ifndef SUDOKU_H
#define SUDOKU_H
#include <QGraphicsView>
#include "graphe.h"
#include <QInputDialog>
#include <QLayout>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QLabel>

class SurInterface;
class Sudoku : public QMainWindow
{
    Q_OBJECT

public:
    Sudoku(QWidget* parent =0);
    ~Sudoku();
    void drawgridH();
    void drawgridV();
private slots:
    void checkerf();
    void resetf();
    void visif();
    void invisif();
    void demof();
private:
    SurInterface* sudokuBoard;

    QToolBar* sudtools;
    QMainWindow* mw;

    QAction* checker;
    QAction* demo;
    QAction* reset;
    QAction* visi;
    QAction* inivisi;


};

#endif // SUDOKU_H
