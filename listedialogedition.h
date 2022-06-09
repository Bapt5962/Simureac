#ifndef LISTEDIALOGEDITION_H
#define LISTEDIALOGEDITION_H

#include <QWidget>
#include <QPainter>

/*
 * [ListeDialogEdition]
 *
 * Un élement d'une liste de DialogParticules ou de DialogReactions
 *
 */

namespace Ui {
class ListeDialogEdition;
}

class ListeDialogEdition : public QWidget
{
    Q_OBJECT

public:
    explicit ListeDialogEdition(int ID, QString nom, QColor fond, QWidget *parent = nullptr);
        ~ListeDialogEdition();

public slots:
    void on_Editer_pressed();

signals:
    void boutonClic(int);

protected:
    virtual void paintEvent(QPaintEvent *event) override;

private:
    Ui::ListeDialogEdition *ui;

    int ID;
    QColor couleur;
};

#endif // LISTEDIALOGEDITION_H
