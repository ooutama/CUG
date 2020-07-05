#include <QInputDialog>
#include <QXmlStreamWriter>
#include <QMouseEvent>
#include <algorithm>
#include <functional>
#include <QTextEdit>
#include <QVBoxLayout>
#include "inputdata.h"
#include <QTextStream>
#include <QLabel>
#include <QFormLayout>
#include <QSpinBox>
#include <QDialogButtonBox>
#include "surinterface.h"
#include <QRadioButton>
#include <QCheckBox>
#include <QMessageBox>
#include "sudoku.h"
#include <QDebug>
using namespace CUG;


SurInterface::SurInterface(QWidget* parent)
    : QGraphicsView(parent)
{
    QGraphicsScene* newScene = new QGraphicsScene(0, 0, WIDTH, HEIGHT);
    setScene(newScene);
    setMinimumSize(667,600);
    setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    setDragMode(QGraphicsView::RubberBandDrag);
    graphe = new Graphe();
    drawingLine = NULL;
    aSommet = [] (QGraphicsItem *item) {
        return qgraphicsitem_cast<SommetGraphics *>(item);
    };
    aArete = [] (QGraphicsItem *item) {
        return qgraphicsitem_cast<AreteGraphics *>(item);
    };

}

void SurInterface::mouseMoveEvent(QMouseEvent *event)
{
    switch (toggledMode)
    {
        case defaultMode:
        {
            SommetGraphics* sommet = aSommet(scene()->itemAt(mapToScene(event->pos()), QTransform()));
            if (sommet)
                ensureVisible(sommet, 40, 40);
            break;
        }
        case renommerMode:
        {
            setElementContent(getSelectedItem());
            break;
        }
        case pondererMode:
        {
            setElementContent(getSelectedItem());
            break;
        }
        case areteCreationMode:
        case arcCreationMode:
        {
            if (drawingLine)
            {
                drawingLine->setLine(QLineF(selectedSommets.first->getCenterPos(), mapToScene(event->pos())));
                ensureVisible(drawingLine);
            }
            break;

        }
    }
    QGraphicsView::mouseMoveEvent(event);
    setCursor(Qt::OpenHandCursor);
}

void SurInterface::mousePressEvent(QMouseEvent *event)
{
    switch (toggledMode)
    {
        case defaultMode:
        {
            QGraphicsView::mousePressEvent(event);
            break;
        }
        case sommetCreationMode:
        {
            creerSommet(event->pos());
            break;
        }
        case areteCreationMode:
        case arcCreationMode:
        {
            manageAreteCreation(event->pos());
            break;
        }
        case deletionMode:
        {
            deleteElementAtPosition(event->pos());
            break;
        }
    }
    setCursor(Qt::CrossCursor);
}

void SurInterface::keyPressEvent(QKeyEvent *event)
{
    clearDrawingLine();
    switch (event->key())
    {
        case Qt::Key_Delete:
        {
            deleteSelectedElements();
            break;
        }
        case Qt::Key_Space:
        {
            setElementContent(getSelectedItem());
            break;
        }
        default:
        {
            clearDrawingLine();
            QWidget::keyPressEvent(event);
        }
    }
}

void SurInterface::focusOutEvent(QFocusEvent *event)
{
    clearDrawingLine();
    QGraphicsView::focusOutEvent(event);
}

void SurInterface::creerSommet(const QPoint& pos, const QString& idtf)
{
    Sommet::const_ref newGraphSommet = graphe->addSommet(idtf);
    SommetGraphics* newSommetItem = new SommetGraphics(mapToScene(pos), newGraphSommet);
    scene()->addItem(newSommetItem);
}

void SurInterface::deleteElementAtPosition(const QPoint &pos)
{
    QGraphicsItem* item = scene()->itemAt(QPointF(mapToScene(pos)), QTransform());
    deleteElement(item);
}

void SurInterface::deleteElement(QGraphicsItem *item)
{
    if (SommetGraphics* sommet = aSommet(item))
    {
        foreach (AreteGraphics* arete, sommet->getAreteItems())
            deleteElement(arete);
        Sommet::const_ref graphSommet = sommet->getGraphSommet();
        delete sommet;
        graphe->delSommet(graphSommet);
    }
    else if (AreteGraphics* arete = aArete(item))
    {
        Arete::const_ref grapheArete = arete->getGrapheArete();
        delete arete;
        graphe->delArete(grapheArete);
    }
}

void SurInterface::deleteSelectedElements()
{
    QList<QGraphicsItem *> items = scene()->selectedItems();
    std::partition(items.begin(), items.end(), aArete);
    foreach (QGraphicsItem* item, items)
        deleteElement(item);
}

void SurInterface::manageAreteCreation(const QPoint &location)
{
    SommetGraphics* topmostSommet = getTopmostSommetItem(scene()->items(QPointF(mapToScene(location))));
    if (topmostSommet)
    {
        topmostSommet->setSelected(true);
        if (!selectedSommets.first)
        {
            selectedSommets.first = topmostSommet;
            drawingLine = scene()->addLine(QLine(location, location));
        }
        else if (selectedSommets.first && !selectedSommets.second)
        {
            selectedSommets.second = topmostSommet;
            creerArete(selectedSommets.first, selectedSommets.second);
            clearDrawingLine();
            clearSelection();
        }
    }
    else
    {
        clearDrawingLine();
        clearSelection();
    }
}

void SurInterface::setElementContent(QGraphicsItem* item)
{
    if (SommetGraphics* sommet = aSommet(item))
    {
        QString idtf =QInputDialog::getText(this,tr("Renommer le sommet"), tr("Nouvel identifiant:"), QLineEdit::Normal,sommet->getGraphSommet().getText());
        graphe->setSommetIdtf(sommet->getGraphSommet(), idtf);
    }
    else if (AreteGraphics* arete = aArete(item))
    {
        int poids = QInputDialog::getInt(this,tr("Changer le poids d'une Arete"),tr("Nouveau poids:"));
        graphe->setAretePoids(arete->getGrapheArete(), poids);
    }
}

void SurInterface::creerArete(SommetGraphics* firstSommet, SommetGraphics* secondSommet, int poids, bool oriented)
{
    if (firstSommet && secondSommet)
    {
        try
        {
            bool isOriented = (toggledMode == arcCreationMode) || oriented;
            Arete::const_ref newGraphArete = graphe->addArete(firstSommet->getGraphSommet(),secondSommet->getGraphSommet(),poids,isOriented);
            AreteGraphics* nouveauArete = new AreteGraphics(firstSommet,secondSommet,newGraphArete);
            scene()->addItem(nouveauArete);
            clearSelection();
        }
        catch (const mauvais_arete& e)
        {
            clearSelection();
            return;
        }
    }
}

void SurInterface::creerArete(int firstSommetIndex, int secondSommetIndex, int poids, bool oriented)
{
    SommetGraphics* firstSommet = NULL;
    SommetGraphics* secondSommet = NULL;
    foreach (SommetGraphics* sommet, getSommets())
    {
        if (sommet->getGraphSommet().getIndex() == firstSommetIndex)
            firstSommet = sommet;
        if (sommet->getGraphSommet().getIndex() == secondSommetIndex)
            secondSommet = sommet;
    }
    creerArete(firstSommet, secondSommet, poids, oriented);
}

void SurInterface::toggleSelectionMode(bool isToggled)
{
    setMouseTracking(false);
    toggleMode(defaultMode, isToggled);
}

void SurInterface::toggleRenommerMode(bool isToggled)
{
    setMouseTracking(false);
    toggleMode(renommerMode, isToggled);
}

void  SurInterface::togglePondererMode(bool isToggled)
{
    setMouseTracking(false);
    toggleMode(pondererMode, isToggled);
}
void SurInterface::toggleMode(int mode, bool toggled)
{
    if (toggled)
    {
        toggledMode = mode;
        scene()->clearSelection();
    }
    else
        toggledMode = defaultMode;
}

bool SurInterface::saveGraphToFile(const QString& saveFileName)
{
    QFile file(saveFileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return false;

    QXmlStreamWriter stream(&file);
    stream.setAutoFormatting(true);
    stream.writeStartDocument();
    stream.writeStartElement("graphe");

    foreach (SommetGraphics* sommet, getSommets()) {
        stream.writeStartElement("sommet");
        QXmlStreamAttributes attribs;
        attribs.append("x", QString::number(sommet->getCenterPos().x()));
        attribs.append("y", QString::number(sommet->getCenterPos().y()));
        attribs.append("Index", QString::number(sommet->getGraphSommet().getIndex()));
        attribs.append("contenu", sommet->getGraphSommet().getText());
        stream.writeAttributes(attribs);
        stream.writeEndElement();
    }
    foreach (AreteGraphics *arete, getAretes())
    {
        stream.writeStartElement("arete");
        QXmlStreamAttributes attribs;
        attribs.append("source", QString::number(arete->getGrapheArete().getIndex().first));
        attribs.append("destination", QString::number(arete->getGrapheArete().getIndex().second));
        attribs.append("poids", QString::number(arete->getGrapheArete().getPoids()));
        attribs.append("oriente", arete->isOriented() ? "oui" : "non");
        stream.writeAttributes(attribs);
        stream.writeEndElement();
    }

    stream.writeEndElement();
    stream.writeEndDocument();
    file.close();
    return true;
}

bool SurInterface::loadGraphFromFile(const QString& loadFileName)
{
    deleteGraph();

    QFile file(loadFileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    QXmlStreamReader stream(&file);

    while (!stream.atEnd() && !stream.hasError())
    {
        QXmlStreamReader::TokenType token = stream.readNext();
        if (token == QXmlStreamReader::StartDocument)
            continue;
        if (token == QXmlStreamReader::StartElement)
        {
            if (stream.name() == "sommet")
            {
                QXmlStreamAttributes attribs = stream.attributes();

                QPoint pos(attribs.value("x").toInt(), attribs.value("y").toInt());
                QString text = attribs.value("contenu").toString();

                creerSommet(pos, text);
            }
            if (stream.name() == "arete")
            {
                QXmlStreamAttributes attribs = stream.attributes();

                int beginSommetIndex = attribs.value("source").toInt();
                int endSommetIndex = attribs.value("destination").toInt();
                int poids = attribs.value("poids").toInt();
                //bool oriented = attribs.value("oriente").toString() == "oui" ;
                QString oriented = attribs.value("oriente").toString();
                if(oriented=="oui"){
                    creerArete(beginSommetIndex, endSommetIndex, poids, true);
                }
                else{
                    creerArete(beginSommetIndex, endSommetIndex, poids, false);
                }
            }
        }
    }
    file.close();
    return true;
}

void SurInterface::deleteGraph()
{
    scene()->clear();
    delete drawingLine;
    delete graphe;

    graphe = new Graphe();
}

void SurInterface::toggleSommetCreationMode(bool isToggled)
{
    toggleMode(sommetCreationMode, isToggled);
}


void SurInterface::toggleArcCreationMode(bool isToggled)
{
    setMouseTracking(true);
    toggleMode(arcCreationMode, isToggled);
}

void SurInterface::toggleAreteCreationMode(bool isToggled)
{
    setMouseTracking(true);
    toggleMode(areteCreationMode, isToggled);
}

void SurInterface::toggleDeletionMode(bool isToggled)
{
    deleteSelectedElements();
    toggleMode(deletionMode, isToggled);
}
void SurInterface::nbrchromatique(int chrom){
    txt->setText("Le nombre chromatqiue de ce graphe est :" );
    txt->append(QString::number(chrom));
    txt->setTextColor(Qt::darkCyan);
    txt->setFontFamily("monospace");
    txt->setReadOnly(true);
    txt->setMaximumSize(350,40);
    txt->setMinimumSize(350,40);
    txt->setStyleSheet("background:rgb(44,44,44);font-size:10pt;font-weight:bold;color:Cyan;");
    txt->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    txt->setWindowIcon(QPixmap(":/icons/cuglogo1.png"));
    txt->show();
    QVBoxLayout* lay= new QVBoxLayout();
    lay->addWidget(txt,0,Qt::AlignBottom |Qt::AlignLeft);
    lay->setMargin(0);
    this->setLayout(lay);
}

void SurInterface::colorationGlouton()
{
    txt->clear();
    int nbSommets = graphe->compterSommets();

        QList<QColor> colors;

        QList<SommetGraphics*> sommetsGraphics = getSommets();

        QList<QColor> sommetsColorier;

        int nbChromatique = 0;

        for (int i = 0 ; i < nbSommets; i++) {
            int x = 0;
            int y = 0;
            int z = 0;

            // While x == y == z
            // Generate another color
            while(x == y && y == z)
            {
                x =  rand()%255;
                y =  rand()%255;
                z =  rand()%255;
            }

            QColor m(x, y, z);

            colors.append(m);

            sommetsColorier.append(QColor("#000"));
        }

        foreach(Sommet *sommet, graphe->lista())
        {
            // index de color
            int colorIndex = 0;
            foreach(QColor color, colors)
            {
                colorIndex++;
                // Compter le nombre des adjancents qui non utilise pas (color)
                int compter = 0;

                // Nombre des adjacents
                int nbAdjacentes = 0;

                QPair<int, Sommet::Index> sommetPaire;

                foreach(sommetPaire, sommet->getSommetsAdjacentspC())
                {
                    nbAdjacentes++;
                    if (sommetsColorier.at(sommetPaire.second).rgb() != color.rgb())
                    {
                        // Si oui incrementer le nb des successuer qu'ont pas la couleur i
                        compter++;
                    }
                }

                if (nbAdjacentes == compter)
                {
                    /*QTimer tmp;
                    QEventLoop boc;
                    tmp.setInterval(500);
                    QObject::connect(&tmp,&QTimer::timeout,&boc,&QEventLoop::quit);
                    tmp.start();
                    boc.exec();*/
                    sommetsColorier.replace(sommet->getIndex(), color);
                    sommetsGraphics.at(sommet->getIndex())->setBrush(color);
                    break;
                }
            }

            if(colorIndex > nbChromatique)
                nbChromatique = colorIndex;
        }

        //compter le nbr de couleur dans la table couleur
        nbrchromatique(nbChromatique);



}

bool SurInterface::check(QList<int> v,int a){
    int taille =v.size();
    for (int i=0;i<taille;i++) {
        if(v[i]==a){
            return true;
        }
    }
    return false;

}

void SurInterface::colorationDsatur()
{
    txt->clear();
    int nbrdeSommets=graphe->compterSommets();

    QList<QColor> colors;
    QList<SommetGraphics*> sommetsGraphics = getSommets();

    for (int i = 0 ; i < nbrdeSommets; i++) {
        int x = 0;
        int y = 0;
        int z = 0;

        // While x == y == z
        // Generate another color
        while(x == y && y == z)
        {
            x =  rand()%255;
            y =  rand()%255;
            z =  rand()%255;
        }

        QColor m(x, y, z);

        colors.append(m);
    }

    QVector<int> couleur(nbrdeSommets,-1);
    QVector<int> Degree(nbrdeSommets,0);
    QVector<int> Dsat(nbrdeSommets,0);

    QLinkedList<Sommet*> sommm=graphe->lista();
    QList<Sommet*> so;
    foreach(Sommet* k,sommm){
          so.append(k);
     }
    std::sort(so.begin(),so.end(),Graphe::compare);

        //initialisation
        for (int d=0;d<nbrdeSommets;d++) {
            Degree[d]=so[d]->getDegree();
            Dsat[d]=so[d]->getDegree();
        }
        couleur[so[0]->getIndex()]=0;

        for (int i=1;i<nbrdeSommets;i++) {
            QVector<bool> Libre(nbrdeSommets,true);

            QPair<int,Sommet::Index> pair;
            foreach(pair, graphe->recupererSommet(so[i]->getIndex()).getSommetsAdjacentspC()){
                if (couleur[pair.second] != -1)
                     Libre[couleur[pair.second]] = false;

            }
            int index=0;
            while (Libre[index]==false) {
                index=index+1;
            }

            foreach(pair,graphe->recupererSommet(so[i]->getIndex()).getSommetsAdjacentspC()){
                        if(Dsat[pair.second]==Degree[pair.second]){
                            Dsat[pair.second]=1;

                        QPair<int,Sommet::Index> pairy;
                        foreach(pairy,graphe->recupererSommet(so[pair.second]->getIndex()).getSommetsAdjacentspC()){
                            if (index!=couleur[pairy.second]){
                                Dsat[pair.second]=Dsat[pair.second]+1;

                          }

                        }

                     }

             }
            couleur[so[i]->getIndex()]=index;
    }

    //nombre chromoatique
    std::set<int> unset;
    int s=0;
    for(int c=0;c<nbrdeSommets;c++){
        if(unset.find(couleur[c]) == unset.end()){
            unset.insert(couleur[c]);
            s++;
        }
    }

    for (int i=0;i<nbrdeSommets;i++) {
        sommetsGraphics[i]->setBrush(colors[couleur[i]]);
    }

    //affichage du nombre chromatique
    nbrchromatique(s);


}
void SurInterface::colorationWelshPowell()
{
    txt->clear();
    int nbrsommets=graphe->compterSommets();

        std::vector<bool> disponible(nbrsommets, false);
        std::vector<int> couleur(nbrsommets, -1);
        QList<int> couleursint;

        QList<QColor> colors;

        for (int i = 0 ; i < nbrsommets; i++) {
            int x = 0;
            int y = 0;
            int z = 0;

            // While x == y == z
            // Generate another color
            while(x == y && y == z)
            {
                x =  rand()%255;
                y =  rand()%255;
                z =  rand()%255;
            }

            QColor m(x, y, z);

            colors.append(m);
        }

        QLinkedList<Sommet*> sommm=graphe->lista();
        QList<Sommet*> so;
        foreach(Sommet* k,sommm){
            so.append(k);
        }
        std::sort(so.begin(),so.end(),Graphe::compare);

        couleur[so[0]->getIndex()]=0;
        couleursint.append(so[0]->getIndex());
        for(int i=1;i<nbrsommets;i++){
            QTextStream out(stdout);

                int is=so[i]->getIndex();
                out<< is << endl;
                QPair<int,Sommet::Index> pair;
                foreach(pair, graphe->recupererSommet(is).getSommetsAdjacentspC()){
                    if(couleur[pair.second]!=-1){
                        disponible[couleur[pair.second]]=true;
                    }
                }

                int color;
                for(color=0;color<nbrsommets;color++)
                    if(disponible[color]==false)
                           break;
                couleur[is]=color;
                couleursint.append(is);

                foreach(pair, graphe->recupererSommet(is).getSommetsAdjacentspC()){

                    if(couleur[pair.second]!=-1){
                          disponible[couleur[pair.second]]=false;
                     }
                }

            }

        QList<SommetGraphics*> sg=getSommets();

        for(int m=0;m<nbrsommets;m++){
            /*QTimer tmp;
            QEventLoop boc;
            tmp.setInterval(500);
            QObject::connect(&tmp,&QTimer::timeout,&boc,&QEventLoop::quit);
            tmp.start();
            boc.exec();*/
            sg[couleursint[m]]->setBrush(colors[couleur[couleursint[m]]]);
        }


        //nombre chromoatique
        std::set<int> unset;
        int s=0;
        for(int c=0;c<nbrsommets;c++){
            if(unset.find(couleur[c]) == unset.end()){
                unset.insert(couleur[c]);
                s++;
            }
        }
         nbrchromatique(s);

}


void SurInterface::resolutionSudoku()
{
    for(int i=0;i<81;i++){
        int size=sqrt(81-0.1);
        int dx=5000/(81+1);
        int dy=5000/(81+1);

        QPoint pos(i%(size+1)*dx+350, i/(size+1)*dy+100);

        creerSommet(pos);
    }

    for(int i=0;i<81;i++){
           static int j;

            // Connections
            static int quadrant_row;
            static int quadrant_column;
            quadrant_row = (i % 9) / 3;
            quadrant_column = (i / 9) / 3;
            j = i + 1;
            while ( ((j / 9) / 3) == quadrant_column )
            {
                if ( ((j % 9) / 3)  == quadrant_row )
                {
                    creerArete(i,j);
                    j++;
                }
                else
                {
                    j += 6;
                }
            }

            // row Connections
            j = i+1;

            while ( (j % 9) / 3 == quadrant_row ) j++;
            while ( j % 9 > i % 9 )
            {
                creerArete(i,j);
                j++;
            }

             //column Connections
            j = i + 9;

            while ( ((j / 9) / 3) == quadrant_column ) j += 9;
            while ( j < 81 )
            {
                creerArete(i,j);
                j += 9;
            }


    }
    QLinkedList<Sommet*> smt=graphe->lista();
    foreach(Sommet* sm,smt){
        sm->setText("");
    }

    QList<SommetGraphics*> sommg=getSommets();
    foreach(SommetGraphics* som,sommg){
        som->setPen(Qt::NoPen);
    }
    QList<AreteGraphics*> ars=getAretes();
    foreach(AreteGraphics* arete,ars){
        arete->setVisible(false);
    }

    solvesudo();
    QList<Sommet*> sommetempo;
    foreach(Sommet* sommt,smt){
        sommetempo.append(sommt);
    }
    int tailletos=(rand()%24)+15;
    for(int i=0;i<tailletos;i++){
        int f=rand()%80;
        sommetempo[f]->setText(QString::number(lesindicesdegrille[f]));
        sommg[f]->label->setFont(QFont("Helvetica", 12,QFont::Bold));
        resetindis.append(f);
    }



}
void SurInterface::resetsudo(){

    int taille=resetindis.size();
    QLinkedList<Sommet*> smt=graphe->lista();
    QList<SommetGraphics*> sommg=getSommets();
    QList<Sommet*> sommetempo;
    foreach(Sommet* sommt,smt){
        sommetempo.append(sommt);
    }
    for (int j=0;j<sommetempo.size();j++) {
        sommetempo[j]->setText("");
    }
    for (int i=0;i<taille;i++) {
        int f= resetindis[i];
        sommetempo[f]->setText(QString::number(lesindicesdegrille[f]));
        sommg[f]->label->setFont(QFont("Helvetica", 12,QFont::Bold));
    }
    update();
}


void SurInterface::graphevisi(){
    QList<SommetGraphics*> sommg=getSommets();
    foreach(SommetGraphics* som,sommg){
        som->setPen(QPen(Qt::cyan,2));
        som->setBrush(Qt::white);
    }
    QList<AreteGraphics*> ars=getAretes();
    foreach(AreteGraphics* arete,ars){
        arete->setVisible(true);
    }
}


void SurInterface::grapheinvis(){
    QList<SommetGraphics*> sommg=getSommets();
    foreach(SommetGraphics* som,sommg){
        som->setPen(Qt::NoPen);
        som->setBrush(Qt::NoBrush);
    }
    QList<AreteGraphics*> ars=getAretes();
    foreach(AreteGraphics* arete,ars){
        arete->setVisible(false);
    }
}
void SurInterface::colorDsudoko()
{
    Sudoku* sdl=new Sudoku();
    //Sudoku::generlegraphe();
    sdl->show();

}




void SurInterface::verifiersudoku(){
    int V=graphe->compterSommets();
    QList<SommetGraphics*> sommetsgraphics=getSommets();
    QLinkedList<Sommet*> smt=graphe->lista();
    QList<Sommet*> sommetempo;
    foreach(Sommet* sommt,smt){
        sommetempo.append(sommt);
    }
    for (int i=0;i<V;i++) {
        sommetsgraphics[i]->setPen(Qt::NoPen);
    }
    QMessageBox msg;
    QList<int> erro;
    for (int i=0;i<V;i++) {
        int temp= sommetempo[i]->getText().toInt();
        if(temp!=lesindicesdegrille[i] && temp!=0){
            erro.append(i);
        }
    }
    int taille=erro.size();
    if(taille!=0){
        for (int i=0;i<taille;i++) {
            sommetsgraphics[erro[i]]->setPen(QPen(Qt::red, 1));
        }}
    else{
        msg.setStyleSheet("background:rgb(44,44,44);color:rgb(255,255,255)");
        msg.setMinimumWidth(200);
        msg.setWindowIcon(QIcon(":icons/cuglogo.png"));
        for (int i=0;i<V;i++) {
            QString temp= sommetempo[i]->getText();
            if(temp.isEmpty()){
                msg.setText("Completer la grille");
            }
            else{
                msg.setText("Felecitation");
            }
    }
        msg.exec();
    }
}

void SurInterface::demo()
{
    solvesudo();
    int V=graphe->compterSommets();
    QList<QColor> colors;

    for (int i = 0 ; i < V; i++) {
        int x = 0;
        int y = 0;
        int z = 0;

        // While x == y == z
        // Generate another color
        while(x == y && y == z)
        {
            x =  rand()%255;
            y =  rand()%255;
            z =  rand()%255;
        }

        QColor m(x, y, z);

        colors.append(m);
    }
    QList<SommetGraphics*> sommetsgraphics=getSommets();
    QLinkedList<Sommet*> smt=graphe->lista();
    QList<Sommet*> sommetempo;
    foreach(Sommet* sommt,smt){
        sommetempo.append(sommt);
    }
    for (int i=0;i<V;i++) {
        sommetsgraphics[i]->setBrush(colors[lesindicesdegrille[i]]);
        sommetempo[i]->setText(QString::number(lesindicesdegrille[i]));

    }
}
//========================Sudokuu====================/////
bool SurInterface::estSecuriser(int v, int color[], int c)
{
    for (int i=0;i<v;i++) {
        if(graphe->contientArete(v,i) && c== color[i])
            return false;
    }
    return true;
}

bool SurInterface::utilColoration(int m, int color[], int v)
{
    int V=graphe->compterSommets();
    if(v == V){
        return true;
    }
    for (int c=1;c<=m;c++) {
        if (estSecuriser(v,color,c)){
            color[v]=c;
            if (utilColoration(m,color,v+1) == true)
                return true;
            color[v]=0;
        }
    }
    return false;
}

//backtracking
void SurInterface::solvesudo()
{
    int V=graphe->compterSommets();

    int m=9;
    int color[V];
    for(int i=0;i<V;i++){
        color[i]=0;
    }

    if(utilColoration(m,color,0)==false){
        printf("solution does not exists");
        return;
    }

    for(int i=0;i<V;i++){
        lesindicesdegrille.append(color[i]);
    }
}
//generateur de graphe

void SurInterface::generateur()
{
    QDialog dialog(this);
    QFormLayout form(&dialog);
    form.addRow(new QLabel("Est ce que le graphe est orinte:"));

    QString valueOui = QString("Checker");
    QCheckBox* check1 = new QCheckBox(&dialog);
    form.addRow(valueOui, check1);

    form.addRow(new QLabel("Est ce que le graphe est Ponderer:"));

    QString valuePo = QString("Checker");
    QCheckBox* check2 = new QCheckBox(&dialog);
    form.addRow(valuePo, check2);

    // Value1
    QString value1 = QString("Nombre de sommet: ");
    QSpinBox *spinbox1 = new QSpinBox(&dialog);
    form.addRow(value1, spinbox1);
    // Value2
    QString value2 = QString("Nombre d'aretes/arc ");
    QSpinBox *spinbox2 = new QSpinBox(&dialog);
    form.addRow(value2, spinbox2);
    spinbox1->setRange(0,1000);
    spinbox2->setRange(0,1000);
    // Add Cancel and OK button
    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
        Qt::Horizontal, &dialog);
    form.addRow(&buttonBox);
    QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
    QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));


    // Process when OK button is clicked
    if (dialog.exec() == QDialog::Accepted) {

            int nbrdesommet = spinbox1->value();
            int nbrdearete = spinbox2->value();
            int nbMax = (nbrdesommet * (nbrdesommet-1)) / 2;
            if (check1->isChecked())
                nbMax = nbMax * 2;
            if (nbMax < nbrdearete) {
                QMessageBox msg;
                msg.setStyleSheet("background:rgb(44,44,44);color:rgb(255,255,255)");
                msg.setMinimumWidth(200);
                msg.setWindowIcon(QIcon(":icons/cuglogo.png"));
                msg.setText("Le nombre d'arete est invalide !");
                msg.exec();
                return;
            }
            for (int i = 0; i < nbrdesommet; i++) {
                // Genere la postion du sommet aleotoirement sur l'interface
                int a = qrand() % (480) + 420;
                int b = -(qrand() % (250)) + 320;
                QPoint pos(a,b);
                // Creation du sommet
                creerSommet(pos);
            }


            int k = nbrdearete;
            while(k > 0){
                // Trouver les indices de sommets qui vont etre liees
                int a = rand() % nbrdesommet;
                int b = rand() % nbrdesommet;
                // La valeur de l'arete/arc
                int p;
                // Le graphe oriente ou pas
                bool oriente;
                // Verification l'existence
                if ( a==b || graphe->contientArete(a,b)) {
                    // si oui, augmente k et continue.
                    //k++;
                    continue;
                } else {
                    // Generie la valeur du p si le graphe est pondere
                    p = (check2->isChecked()) ? rand() % 100 : 0;
                    oriente = (check1->isChecked()) ? true : false;
                    // Creation de l'arete/arc
                    creerArete(a, b, p, oriente);
                }
                // Decrementer la valeur du k
                k--;
            }
        }
}

SommetGraphics *SurInterface::getTopmostSommetItem(QList<QGraphicsItem *> items)
{
    if (!items.isEmpty())
    {
        auto isNotSommet = [this](QGraphicsItem* item) -> bool
                        { return !aSommet(item); };
        items.erase(std::remove_if(items.begin(), items.end(), isNotSommet), items.end());
        if (!items.isEmpty())
        {
            auto sortByZ = [](QGraphicsItem* firstItem, QGraphicsItem* secondItem)
                            { return firstItem->zValue() < secondItem->zValue(); };
            std::sort(items.begin(), items.end(), sortByZ);

            return aSommet(items.first());
        }
    }
    return NULL;
}

QGraphicsItem* SurInterface::getSelectedItem()
{
    QList<QGraphicsItem*> selectedItems = scene()->selectedItems();
    if (selectedItems.length() == 1)
        return selectedItems.first();
    else
        return NULL;
}

QList<SommetGraphics *> SurInterface::getSommets() const
{
    QList<QGraphicsItem *> items = scene()->items();
    QList<SommetGraphics *> sommets;
    foreach (QGraphicsItem *item, items)
    {
        if (SommetGraphics *sommet = aSommet(item))
            sommets.append(sommet);
    }
    auto indexComp = [](SommetGraphics* first, SommetGraphics* second)
    {
        return first->getGraphSommet().getIndex() < second->getGraphSommet().getIndex();
    };

    std::sort(sommets.begin(), sommets.end(), indexComp);
    return sommets;
}

QList<AreteGraphics *> SurInterface::getAretes() const
{
    QList<QGraphicsItem *> items = scene()->items();
    QList<AreteGraphics *> aretes;
    foreach (QGraphicsItem *item, items)
    {
        if (AreteGraphics *arete = aArete(item))
            aretes.append(arete);
    }
    return aretes;
}

void SurInterface::clearDrawingLine()
{
    delete drawingLine;
    drawingLine = NULL;
}

void SurInterface::clearSelection()
{
    scene()->clearSelection();
    selectedSommets = SommetPair();
}
