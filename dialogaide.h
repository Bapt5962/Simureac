#ifndef DIALOGAIDE_H
#define DIALOGAIDE_H

#include <QDialog>
#include <QTextEdit>
#include <QImage>
#include <QImageReader>
#include <QListWidgetItem>
#include "filetext.h"

/*
 * [Dialog Aide]
 *
 * Fenêtre d'aide
 *
 */

namespace Ui {
class DialogAide;
}

class DialogAide : public QDialog
{
    Q_OBJECT

public:
    explicit DialogAide(QWidget *parent = nullptr);
    ~DialogAide();

    void lirePage(int num);

    void ajouterImage(QString id);

public slots:
    void on_Fermer_clicked();
    void on_Sommaire_itemClicked(QListWidgetItem *item);

signals:
    void dialogQuit(int);

private:
    Ui::DialogAide *ui;

    QList<QListWidgetItem*> list;

    QStringList sommaire;
    QStringList pages;
};

#endif // DIALOGAIDE_H
