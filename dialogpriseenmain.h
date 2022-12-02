#ifndef DIALOGPRISEENMAIN_H
#define DIALOGPRISEENMAIN_H

#include <QWidget>
#include "filetext.h"

/*
 * [DialogPriseEnMain]
 *
 * Fenêtre d'aide à la prise en main
 *
 */

namespace Ui {
class DialogPriseEnMain;
}

class DialogPriseEnMain : public QWidget
{
    Q_OBJECT

public:
    explicit DialogPriseEnMain(QWidget *parent = nullptr);
    ~DialogPriseEnMain();

    void changerAide();

public slots:
    void on_Precedent_clicked();
    void on_Suivant_clicked();
    void on_Quitter_clicked();

private:
    Ui::DialogPriseEnMain *ui;

    int indexAide;
    QList<QString> titres;
    QList<QString> descriptions;

    void load();
};

#endif // DIALOGPRISEENMAIN_H
