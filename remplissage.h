#ifndef REMPLISSAGE_H
#define REMPLISSAGE_H

#include <QList>

/*
 * [Remplissage]
 *
 * Elements renvoyés par DialogRemplissage
 *
 * Contient :
 * verrerie : true = Becher ; false = Burette
 * typeFormat : Becher -> type * 100 = volume ; Burette -> type * 25 = volume
 * remplir : Volume à remplir
 * temperature : Température de la solution
 * elements ET concentrations : Particules dans la solution et leurs concentrations
 *
 */

class Remplissage
{
public:
    Remplissage();

    int getTypeFormat() const;
    void setTypeFormat(int value);

    double getRemplir() const;
    void setRemplir(double value);

    QList<int> getElements() const;
    void setElements(const QList<int> &value);

    QList<double> getConcentrations() const;
    void setConcentrations(const QList<double> &value);

    bool getVerrerie() const;
    void setVerrerie(bool value);

    double getTemperature() const;
    void setTemperature(double value);

private:
    bool verrerie;
    int typeFormat;
    double remplir;
    double temperature;
    QList<int> elements;
    QList<double> concentrations;
};

#endif // REMPLISSAGE_H
