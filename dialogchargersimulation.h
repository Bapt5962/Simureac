#ifndef DIALOGCHARGERSIMULATION_H
#define DIALOGCHARGERSIMULATION_H

#include <QDialog>
#include <QWidget>
#include <QList>
#include <QMessageBox>
#include "filetext.h"
#include "general.h"
#include "descriptionfichier.h"

#include <QDebug>

/*
 * [Dialog Charger Simulation]
 *
 * Fenêtre de chargement de simulation
 *
 */

namespace Ui {
class DialogChargerSimulation;
}

class DialogChargerSimulation : public QDialog
{
    Q_OBJECT

public:
    explicit DialogChargerSimulation(General* generalU, QWidget *parent = nullptr);
    ~DialogChargerSimulation();

public slots:
    void on_Importer_clicked();
    void on_Annuler_clicked();
    void on_Recherche_textEdited();
    void simulationCoche(QString chemin);
    void refaireListe();

signals:
    void dialogQuit(int, General*);

protected:
    virtual void resizeEvent(QResizeEvent *event) override;

private:
    Ui::DialogChargerSimulation *ui;

    bool enSuppression;

    General* general;
    General* importation;

    QWidget *liste;
    QList<DescriptionFichier*> descriptions;
};

#endif // DIALOGCHARGERSIMULATION_H
