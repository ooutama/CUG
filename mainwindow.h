#include <QMainWindow>

class QActionGroup;

class SurInterface;


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void newFile();
    void open();
    bool save();
    bool saveAs();
    void closeTab();
    void connectToolsToCurrentWidget();
private:
    SurInterface *getCurrentSurInterface();

    void createMenuActions();
    void createEditActions();
    void createMenus();
    void createToolBars();
    bool saveConfirmation();

    QString currentFile;
    QMenu* fileMenu;
    QMenu* editMenu;
    QMenu* helpMenu;

    QToolBar* fileToolBar;
    QToolBar* editToolBar;


    QAction* newFileAction;
    QAction* openFileAction;
    QAction* saveFileAction;
    QAction* saveAsFileAction;
    QAction* closeTabAction;
    QAction* exitAction;
    QAction* aboutQtAction;

    QActionGroup* editActionGroup;
    QAction* renommer;
    QAction* selectSommet;
    QAction* ponderer;
    QAction* creerSommet;
    QAction* creerArete;
    QAction* creerArc;
    QAction* deleteElement;
    QAction* colorationGlouton;
    QAction* colorationDsatur;
    QAction* colorationWelshPowel;
    QAction* resolutionSudoku;
    QAction* colorDsudoku;
    QAction* generateur;

    QTabWidget* workingArea;
};
