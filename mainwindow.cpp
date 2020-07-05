#include <QAction>
#include <QMessageBox>
#include <QFileDialog>
#include <QToolBar>
#include <QStatusBar>
#include <QLineEdit>
#include <QLabel>
#include <QMenuBar>
#include "surinterface.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    workingArea = new QTabWidget(this);
    newFile();
    setCentralWidget(workingArea);
    connect(workingArea, SIGNAL(currentChanged(int)), this, SLOT(connectToolsToCurrentWidget()));
    QStatusBar* statusBar = new QStatusBar(this);
    setStatusBar(statusBar);
    statusBar->setStyleSheet("background-color:rgb(44,44,44);color:rgb(0,255,255);");
    createMenuActions();
    createEditActions();
    createMenus();
    createToolBars();
    connectToolsToCurrentWidget();
    setWindowIcon(QIcon(":icons/cuglogo.png"));

}

MainWindow::~MainWindow()
{
}


void MainWindow::createMenuActions()
{


    newFileAction = new QAction(QIcon(":/icons/onglet.png"), tr("&Nouveau fichier"), this);
    newFileAction->setShortcut(QKeySequence::New);
    newFileAction->setStatusTip(tr("Creer un nouveau fichier"));
    connect(newFileAction, SIGNAL(triggered()), this, SLOT(newFile()));

    openFileAction = new QAction(QIcon(":/icons/ouvrir.png"), tr("&Ouvrir un fichier"), this);
    openFileAction->setShortcut(QKeySequence::Open);
    openFileAction->setStatusTip(tr("Ouvrir un fichier"));
    connect(openFileAction, SIGNAL(triggered()), this, SLOT(open()));

    saveFileAction = new QAction(QIcon(":/icons/enregistrer.png"), tr("&Enregistrer le fichier"), this);
    saveFileAction->setShortcut(QKeySequence::Save);
    saveFileAction->setStatusTip(tr("Enregistrer le fichier"));
    connect(saveFileAction, SIGNAL(triggered()), this, SLOT(save()));

    saveAsFileAction = new QAction(QIcon(":/icons/enregistrercomme.png"), tr("&Enregistrer le fichier comme..."), this);
    saveAsFileAction->setShortcut(QKeySequence::SaveAs);
    saveAsFileAction->setStatusTip(tr("Enregistrer le fichier comme ..."));
    connect(saveAsFileAction, SIGNAL(triggered()), this, SLOT(saveAs()));

    closeTabAction = new QAction(QIcon(":/icons/fermer.png"), tr("&Fermer l'onlget"), this);
    closeTabAction->setShortcut(QKeySequence::Close);
    closeTabAction->setStatusTip(tr("Fermer l'onlget"));
    connect(closeTabAction, SIGNAL(triggered()), this, SLOT(closeTab()));

    exitAction = new QAction(tr("E&xit"), this);
    exitAction->setShortcut(tr("Ctrl+Q"));
    exitAction->setStatusTip(tr("Exit l'application"));
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

    aboutQtAction = new QAction(tr("About &Qt"), this);
    aboutQtAction->setStatusTip(tr("A propos de nous"));
    connect(aboutQtAction, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
}

void MainWindow::createEditActions()
{
    editActionGroup = new QActionGroup(this);

    selectSommet = new QAction(QIcon(":/icons/selectionner.png"), tr("Selectionner un element"), editActionGroup);
    selectSommet->setCheckable(true);
    selectSommet->setStatusTip(tr("Selectionner un element dans le graphe"));

    renommer = new QAction(QIcon(":/icons/renommer.png"), tr("Renommer un sommet"), editActionGroup);
    renommer->setCheckable(true);
    renommer->setStatusTip(tr("Chagnger l'identifiant d'un sommet dans le graphe"));

    creerSommet = new QAction(QIcon(":/icons/sommet.png"), tr("Ajouter un sommet"), editActionGroup);
    creerSommet->setStatusTip(tr("Ajouter un sommet au graphe"));
    creerSommet->setCheckable(true);

    ponderer = new QAction(QIcon(":/icons/ponderer.png"), tr("Changer le poids d'une arete /arc"), editActionGroup);
    ponderer->setCheckable(true);
    ponderer->setStatusTip(tr("Changer le poids d'une arete /arc dans le graphe"));

    creerArete = new QAction(QIcon(":/icons/arete.png"), tr("Ajouter une arete"), editActionGroup);
    creerArete->setStatusTip(tr("Ajouter une arete entre deux sommet"));
    creerArete->setCheckable(true);

    creerArc = new QAction(QIcon(":/icons/arc.png"), tr("Ajouter un arc"), editActionGroup);
    creerArc->setStatusTip(tr("Ajouter un arc entre deux sommet"));
    creerArc->setCheckable(true);

    deleteElement = new QAction(QIcon(":/icons/supprimer.png"), tr("Supprimer un element"), editActionGroup);
    deleteElement->setStatusTip(tr("Supprimer un element (sommet/arete/arc)"));
    deleteElement->setCheckable(true);

    colorationGlouton = new QAction(QIcon(":/icons/gloutoun.png"), tr("Coloration par methode de Glouton"), this);
    colorationGlouton->setShortcut(QKeySequence("Ctrl+R"));
    colorationGlouton->setStatusTip(tr("Coloration par methode de Glouton"));

    colorationDsatur = new QAction(QIcon(":/icons/dsatur.png"), tr("Coloration par methode de Dsatur"), this);
    colorationDsatur->setShortcut(QKeySequence("Ctrl+T"));
    colorationDsatur->setStatusTip(tr("Coloration par methode de Dsatur"));

    colorationWelshPowel = new QAction(QIcon(":/icons/welshpowel.png"), tr("Coloration par methode de Welsh Et Powel"), this);
    colorationWelshPowel->setShortcut(QKeySequence("Ctrl+"));
    colorationWelshPowel->setStatusTip(tr("Coloration par methode de Welsh Et Powel"));

    resolutionSudoku = new QAction(QIcon(":/icons/sudocolo.png"), tr("Coloration de Sudoku"), this);
    resolutionSudoku->setShortcut(QKeySequence("Ctrl+I"));
    resolutionSudoku->setStatusTip(tr("Resolution d'un Sudoku par la coloration"));

    colorDsudoku = new QAction(QIcon(":/icons/sudoku.png"), tr("Sudoku"), this);
    colorDsudoku ->setShortcut(QKeySequence("Ctrl+I"));
    colorDsudoku ->setStatusTip(tr("Resolution d'un Sudoku par la coloration"));

    generateur = new QAction(QIcon(":/icons/generateur.png"), tr("Generateur"), this);
    generateur->setShortcut(QKeySequence("Ctrl+G"));
    generateur->setStatusTip(tr("Generer un graphe alearoire"));
}

void MainWindow::createMenus()
{

    fileMenu = menuBar()->addMenu(tr("&Fichier"));
    fileMenu->addAction(newFileAction);
    fileMenu->addAction(openFileAction);
    fileMenu->addAction(saveFileAction);
    fileMenu->addAction(saveAsFileAction);
    fileMenu->addSeparator();
    fileMenu->addAction(closeTabAction);
    fileMenu->addAction(exitAction);


    //couleur de filemenu
    fileMenu->setStyleSheet("background-color:rgb(80,80,80);color:rgb(0,255,255);");

    editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addActions(editActionGroup->actions());


    //couleur de editmenu
    editMenu->setStyleSheet("background-color:rgb(80,80,80);color:rgb(0,255,255);");

    menuBar()->addSeparator();
    menuBar()->setStyleSheet("background-color:rgb(44,44,44);color:rgb(0,255,255);");

    helpMenu = menuBar()->addMenu(tr("&Aide"));
    helpMenu->addAction(aboutQtAction);
    //couleur de helpMenu
    helpMenu->setStyleSheet("background-color:rgb(80,80,80);color:rgb(0,255,255);");
}


void MainWindow::createToolBars()
{
    QLabel* logo =new QLabel;
    logo->setPixmap(QPixmap(":/icons/cuglogo1.png"));
    logo->setFixedSize(80,80);

    fileToolBar = addToolBar(tr("&File"));
    fileToolBar->setIconSize(QSize(70, 50));
    addToolBar(Qt::LeftToolBarArea, fileToolBar);

    fileToolBar->addWidget(logo);
    fileToolBar->addAction(newFileAction);
    fileToolBar->addAction(openFileAction);
    fileToolBar->addAction(saveFileAction);
    fileToolBar->addAction(saveAsFileAction);
    fileToolBar->addAction(closeTabAction);
    fileToolBar->setStyleSheet("background-color:rgb(66,66,66);");

    editToolBar = new QToolBar(tr("&Edit"));
    editToolBar->setIconSize(QSize(70, 50));
    addToolBar(Qt::RightToolBarArea, editToolBar);
    editToolBar->addActions(editActionGroup->actions());
    editToolBar->addAction(colorationGlouton);
    editToolBar->addAction(colorationDsatur);
    editToolBar->addAction(colorationWelshPowel);
    //editToolBar->addAction(resolutionSudoku);
    editToolBar->addAction(colorDsudoku);
    editToolBar->addAction(generateur);
    editToolBar->setStyleSheet("background-color:rgb(66,66,66);");
}

void MainWindow::newFile()
{
    int index = workingArea->addTab(new SurInterface(), QString());
    workingArea->setTabText(index, QString("Graphe%1").arg(index + 1));
    workingArea->setCurrentIndex(index);

}

bool MainWindow::saveConfirmation()
{
    QString currentTabTitle = workingArea->tabText(workingArea->indexOf(getCurrentSurInterface()));
    QString warning = QString("%1 a ete modifie.\n Voulez-vous enregistrer les modifications?").arg(currentTabTitle);
    int respond = QMessageBox::warning(this, tr("Saving"), warning,QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
    if (respond == QMessageBox::Yes)
        return save();
    else if (respond == QMessageBox::Cancel)
        return false;
    return true;
}

void MainWindow::open()
{
    if (saveConfirmation())
    {
        QString loadFileName = QFileDialog::getOpenFileName(this,tr("Chargement du graphe... "),"",tr("Graphe (*.gph)"));
        if (!loadFileName.isEmpty())
        {
            getCurrentSurInterface()->loadGraphFromFile(loadFileName);
            currentFile = loadFileName;
        }
    }
}

bool MainWindow::save()
{
    if (currentFile.isEmpty())
        return saveAs();
    else
        return getCurrentSurInterface()->saveGraphToFile(currentFile);
}

bool MainWindow::saveAs()
{
    QString saveFileName = QFileDialog::getSaveFileName(this,tr("Enregistrer le graphe"),"nouveau_graphe.gph",tr("Graphe (*.gph)"));
    if (!saveFileName.isEmpty())
    {
        currentFile = saveFileName;
        return getCurrentSurInterface()->saveGraphToFile(saveFileName);
    }
    else
        return false;
}

void MainWindow::closeTab()
{
    if (saveConfirmation())
    {
        SurInterface *surinterface = getCurrentSurInterface();
        workingArea->removeTab(workingArea->indexOf(surinterface));
        surinterface->close();
    }
}

void MainWindow::connectToolsToCurrentWidget()
{
    SurInterface *surinterface = getCurrentSurInterface();
    connect(renommer, SIGNAL(toggled(bool)), surinterface, SLOT(toggleRenommerMode(bool)));
    connect(selectSommet, SIGNAL(toggled(bool)), surinterface, SLOT(toggleSelectionMode(bool)));
    connect(ponderer, SIGNAL(toggled(bool)), surinterface, SLOT(togglePondererMode(bool)));
    connect(creerSommet, SIGNAL(toggled(bool)), surinterface, SLOT(toggleSommetCreationMode(bool)));
    connect(creerArete, SIGNAL(toggled(bool)), surinterface, SLOT(toggleAreteCreationMode(bool)));
    connect(creerArc, SIGNAL(toggled(bool)), surinterface, SLOT(toggleArcCreationMode(bool)));
    connect(deleteElement, SIGNAL(toggled(bool)), surinterface, SLOT(toggleDeletionMode(bool)));
    selectSommet->toggle();

    connect(colorationGlouton, SIGNAL(triggered(bool)), surinterface, SLOT(colorationGlouton()));
    connect(colorationDsatur, SIGNAL(triggered(bool)), surinterface, SLOT(colorationDsatur()));
    connect(colorationWelshPowel, SIGNAL(triggered(bool)), surinterface, SLOT(colorationWelshPowell()));
    connect(resolutionSudoku, SIGNAL(triggered(bool)), surinterface, SLOT(resolutionSudoku()));
    connect(colorDsudoku, SIGNAL(triggered(bool)), surinterface, SLOT(colorDsudoko()));
    connect(generateur, SIGNAL(triggered(bool)), surinterface, SLOT(generateur()));
}

SurInterface *MainWindow::getCurrentSurInterface()
{
    QWidget *currentWidget = workingArea->currentWidget();
    return static_cast<SurInterface *>(currentWidget);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    for (int i = 0; i < workingArea->count(); i++)
    {
        workingArea->setCurrentIndex(i);
        if (!saveConfirmation())
            break;
    }
}
