#include "inputdata.h"

#include <QLabel>
#include <QLineEdit>
#include <QDialogButtonBox>
#include <QFormLayout>

inputDATA::inputDATA(QWidget *parent, QString title, QList<QString> labelText) : QDialog(parent)
{
    QFormLayout *lytMain = new QFormLayout(this);
    setWindowTitle(title);
    for (int i = 0; i < labelText.size(); ++i)
    {
        QLabel *tLabel = new QLabel(labelText[i], this);
        QLineEdit *tLine = new QLineEdit(this);
        lytMain->addRow(tLabel, tLine);

        fields << tLine;
    }

    QDialogButtonBox *buttonBox = new QDialogButtonBox
            ( QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
              Qt::Horizontal, this );
    lytMain->addWidget(buttonBox);

    bool conn = connect(buttonBox, &QDialogButtonBox::accepted,
                   this, &inputDATA::accept);
    Q_ASSERT(conn);
    conn = connect(buttonBox, &QDialogButtonBox::rejected,
                   this, &inputDATA::reject);
    Q_ASSERT(conn);

    setLayout(lytMain);
}

QStringList inputDATA::getStrings(QWidget *parent, QString title, QList<QString> labelText, bool *ok)
{
    inputDATA *dialog = new inputDATA(parent, title, labelText);

    QStringList list;

    const int ret = dialog->exec();
    if (ok)
        *ok = !!ret;

    if (ret) {
        foreach (auto field, dialog->fields) {
            list << field->text();
        }
    }

    dialog->deleteLater();

    return list;
}
