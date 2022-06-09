#ifndef DIALOGIMPORTERPARTICULE_H
#define DIALOGIMPORTERPARTICULE_H

#include <QDialog>
#include <QWidget>
#include <QList>
#include <QMessageBox>
#include "filetext.h"
#include "general.h"
#include "descriptionfichier.h"

/*
 * [Dialog Importer Particule]
 *
 * Fenêtre d'importation de particules
 *
 */

namespace Ui {
class DialogImporterParticule;
}

class DialogImporterParticule : public QDialog
{
    Q_OBJECT

public:
    explicit DialogImporterParticule(General* generalU, QWidget *parent = nullptr);
    ~DialogImporterParticule();

public slots:
    void on_Importer_clicked();
    void on_Annuler_clicked();
    void on_Recherche_textEdited();
    void refaireListe();

signals:
    void dialogQuit(int);

protected:
    virtual void resizeEvent(QResizeEvent *event) override;

private:
    Ui::DialogImporterParticule *ui;

    bool enSuppression;

    General* general;

    QWidget *liste;
    QList<DescriptionFichier*> descriptions;
};

#endif // DIALOGIMPORTERPARTICULE_H
