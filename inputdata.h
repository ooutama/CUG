#ifndef INPUTDATA_H
#define INPUTDATA_H

#include <QDialog>

class QLineEdit;
class QLabel;
class inputDATA : public QDialog
{
    Q_OBJECT
public:
    explicit inputDATA(QWidget *parent = nullptr, QString title = "Input", QList<QString> labelText = QList<QString>());

    static QStringList getStrings(QWidget *parent, QString title, QList<QString> labelText = QList<QString>(), bool *ok = nullptr);

private:
    QList<QLineEdit*> fields;

};

#endif // INPUTDATA_H

