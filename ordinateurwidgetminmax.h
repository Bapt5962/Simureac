#ifndef ORDINATEURWIDGETMINMAX_H
#define ORDINATEURWIDGETMINMAX_H

#include <QWidget>
#include <QPainter>

/*
 * [OrdinateurWidgetMinMax]
 *
 * Permet de définir un Min et un Max à chacun des trois axes du graphique
 * Laisse choisir si les valeurs maximum des axes sont extensibles ou non
 *
 */

namespace Ui {
class OrdinateurWidgetMinMax;
}

class OrdinateurWidgetMinMax : public QWidget
{
    Q_OBJECT

public:
    explicit OrdinateurWidgetMinMax(QWidget *parent = nullptr);
    ~OrdinateurWidgetMinMax();

    void verifier();

    double getMinX();
    double getMaxX();
    double getMinYG();
    double getMaxYG();
    double getMinYD();
    double getMaxYD();
    bool getLimite();
    bool getManuel();

public slots:
    void on_XMin_valueChanged();
    void on_XMax_valueChanged();
    void on_YGMin_valueChanged();
    void on_YGMax_valueChanged();
    void on_YDMin_valueChanged();
    void on_YDMax_valueChanged();
    void on_Limite_toggled();
    void on_Auto_toggled();
    void on_Sauvegarder_clicked();

signals:
    void changement();

protected:
    virtual void paintEvent(QPaintEvent *event) override;

private:
    Ui::OrdinateurWidgetMinMax *ui;
};

#endif // ORDINATEURWIDGETMINMAX_H
