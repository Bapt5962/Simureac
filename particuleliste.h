#ifndef PARTICULELISTE_H
#define PARTICULELISTE_H

#include <QWidget>
#include <QPainter>

/*
 * [ParticuleListe]
 *
 * Un élément de la liste à droite de MainWindow
 */

namespace Ui {
class ParticuleListe;
}

class ParticuleListe : public QWidget
{
    Q_OBJECT

public:
    explicit ParticuleListe(int y, QWidget *parent = nullptr);
    ~ParticuleListe();

    void setCouleur(QColor fond);
    void setFormule(QString formule);
    void setValeur(QString valeur);
    bool getVisible();

protected:
    virtual void paintEvent(QPaintEvent *event) override;

private:
    Ui::ParticuleListe *ui;

    QColor couleur;
};

#endif // PARTICULELISTE_H
