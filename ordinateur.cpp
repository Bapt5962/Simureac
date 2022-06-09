#include "ordinateur.h"
#include "ui_ordinateur.h"

Ordinateur::Ordinateur(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Ordinateur)
{
    ui->setupUi(this);

    limitesAffiche = false;
    limites = new OrdinateurWidgetMinMax(this);
    limites->move((width() / 2) - (limites->width() / 2), (height() / 2) - (limites->height() / 2));
    limites->hide();
    connect(limites, SIGNAL(changement()), this, SLOT(changementMinMax()));

    grandeurX = 0;
    grandeurYG = 0;
    grandeurYD = 0;

    affichage[0] = "";
    affichage[1] = "t (en s)";
    affichage[2] = "V (en mL)";
    affichage[3] = "Θ (en °C)";
    affichage[4] = "Θ (en °K)";
    affichage[5] = "pH";
    affichage[6] = "[H30+] (en mol.L⁻¹)";
    affichage[7] = "[HO-] (en mol.L⁻¹)";
    affichage[8] = "σ (en S.cm⁻¹)";
    affichage[9] = "A";
    affichage[10] = "T (en %)";

    unites[0] = "";
    unites[1] = " s";
    unites[2] = " mL";
    unites[3] = "°C";
    unites[4] = "°K";
    unites[5] = "";
    unites[6] = " mol.L⁻¹";
    unites[7] = " mol.L⁻¹";
    unites[8] = " S.cm⁻¹";
    unites[9] = "";
    unites[10] = "%";

    tempsDepart = 0;
    tempsDernierPoint = 0;

    enAssimilation = false;

    echelleContrainteMin = false;
    echelleContrainteMax = false;
    minX = 0;
    maxX = 1;
    minYG = 0;
    maxYG = 1;
    minYD = 0;
    maxYD = 1;

    ajouterPointsCouleurs.append("red");
    ajouterPointsCouleurs.append("cyan");
    ajouterPointsCouleurs.append("green");
    ajouterPointsCouleurs.append("magenta");
    ajouterPointsCouleurs.append("blue");
    ajouterPointsCouleurs.append("yellow");
    ajouterPointsCouleurs.append("darkRed");
    ajouterPointsCouleurs.append("darkCyan");
    ajouterPointsCouleurs.append("darkGreen");
    ajouterPointsCouleurs.append("darkMagenta");
    ajouterPointsCouleurs.append("darkBlue");
    ajouterPointsCouleurs.append("darkYellow");

    idCouleur[0] = -1;
    idCouleur[1] = -1;

    dernierePositionCurseur = QPoint(0, 0);
    curseurFixe = 0;
    QTimer::singleShot(100, this, SLOT(toolTipLoop()));
}

Ordinateur::~Ordinateur()
{
    delete ui;
}

void Ordinateur::loop(double temps, double x, double yG, double yD)
{
    if(!enAssimilation)
    {
        tempsDepart = temps;
        tempsDernierPoint = temps - ui->DeltaT->value();
        return;
    }
    if(tempsDernierPoint <= temps + ui->DeltaT->value())
    {
        if(ajouterPointsCoordonnees.size() == 0 && (!echelleContrainteMin))
        {
            minX = x - 1E-7;
            maxX = x + 1E-7;
            minYG = yG - 1E-7;
            maxYG = yG + 1E-7;
            minYD = yD - 1E-7;
            maxYD = yD + 1E-7;
        }
        else if(!echelleContrainteMax)
        {
            if(x < minX)
            {
                minX = x;
            }
            if(x > maxX)
            {
                maxX = x;
            }
            if(yG < minYG)
            {
                minYG = yG;
            }
            if(yG > maxYG)
            {
                maxYG = yG;
            }
            if(yD < minYD)
            {
                minYD = yD;
            }
            if(yD > maxYD)
            {
                maxYD = yD;
            }
        }
        if(grandeurYG != 0)
        {
            graphiques[0].last().append(QPointF(x, yG));
            ajouterPointsCoordonnees.append(QPointF(x, yG));
            ajouterPointsAxe.append(true);
            ajouterPointsCouleursID.append(idCouleur[0]);
        }
        if(grandeurYD != 0)
        {
            graphiques[1].last().append(QPointF(x, yD));
            ajouterPointsCoordonnees.append(QPointF(x, yD));
            ajouterPointsAxe.append(false);
            ajouterPointsCouleursID.append(idCouleur[1]);
        }
        while (tempsDernierPoint <= temps + ui->DeltaT->value()) {
            tempsDernierPoint += ui->DeltaT->value();
        }
    }
    if(tempsDepart + ui->TotalT->value() <= temps)
    {
        on_Assimiler_clicked();
    }
}

int Ordinateur::getGrandeurX() const
{
    return grandeurX;
}

int Ordinateur::getGrandeurYG() const
{
    return grandeurYG;
}

int Ordinateur::getGrandeurYD() const
{
    return grandeurYD;
}

bool Ordinateur::getEnAssimilation() const
{
    return enAssimilation;
}

double Ordinateur::interpolation(double valeur, double min, double max)
{
    double valeurRenvoi(400 * ((valeur - min) / (max - min)));
    if(valeurRenvoi < 0 || valeurRenvoi > 400)
    {
        return 1000;
    }
    else
    {
        return valeurRenvoi;
    }
}

double Ordinateur::inverseInterpolation(double valeur, double min, double max)
{
    double valeurRenvoi((valeur / 400) * (max - min) + min);
    if(valeurRenvoi < min || valeurRenvoi > max)
    {
        return 1000;
    }
    else
    {
        return valeurRenvoi;
    }
}

void Ordinateur::on_Reset_clicked()
{
    graphiques[0].clear();
    graphiques[1].clear();
    ajouterPointsCoordonnees.clear();
    ajouterPointsAxe.clear();
    ajouterPointsCouleursID.clear();
    idCouleur[0] = -1;
    idCouleur[1] = -1;
    ui->Donnees->setEnabled(true);
}

void Ordinateur::on_Assimiler_clicked()
{
    if(!(grandeurX != 0 && (grandeurYD != 0 || grandeurYG != 0)))
    {
        return;
    }
    if(enAssimilation)
    {
        enAssimilation = false;
        ui->Assimiler->setText("[A]ssimiler");
        ui->Reset->setEnabled(true);
        ui->Assimilation->setEnabled(true);
        ui->MinMax->setEnabled(true);
    }
    else
    {
        enAssimilation = true;
        ui->Assimiler->setText("Fin de l'[A]ssimilation");
        ui->Assimilation->setDisabled(true);
        ui->Donnees->setDisabled(true);
        ui->Reset->setDisabled(true);
        ui->MinMax->setDisabled(true);
        graphiques[0].append(QList<QPointF>());
        graphiques[1].append(QList<QPointF>());
        if(grandeurYG != 0 && grandeurYD != 0)
        {
            if(idCouleur[0] == -1 || idCouleur[0] == 10)
            {
                idCouleur[0] = 0;
                idCouleur[1] = 1;
            }
            else
            {
                idCouleur[0] += 2;
                idCouleur[1] += 2;
            }
        }
        else
        {
            if(grandeurYG != 0)
            {
                idCouleur[0]++;
                if(idCouleur[0] == 12)
                {
                    idCouleur[0] = 0;
                }
            }
            else
            {
                idCouleur[1]++;
                if(idCouleur[1] == 12)
                {
                    idCouleur[1] = 0;
                }
            }
        }
    }
    emit assimiliationClic(enAssimilation);
}

void Ordinateur::on_Abscisse_clicked()
{
    grandeurX = ui->Grandeur->currentIndex();
}

void Ordinateur::on_OrdonneeG_clicked()
{
    grandeurYG = ui->Grandeur->currentIndex();
}

void Ordinateur::on_OrdonneeD_clicked()
{
    grandeurYD = ui->Grandeur->currentIndex();
}

void Ordinateur::on_MinMax_clicked()
{
    limites->show();
    limitesAffiche = true;
}

void Ordinateur::changementMinMax()
{
    if(limites->getManuel())
    {
        echelleContrainteMin = true;
        echelleContrainteMax = limites->getLimite();
        minX = limites->getMinX();
        maxX = limites->getMaxX();
        minYG = limites->getMinYG();
        maxYG = limites->getMaxYG();
        minYD = limites->getMinYD();
        maxYD = limites->getMaxYD();
    }
    else
    {
        echelleContrainteMin = false;
        echelleContrainteMax = false;
    }
    limites->hide();
    limitesAffiche = false;
}

void Ordinateur::toolTipLoop()
{
    if(isVisible() && grandeurX != 0)
    {
        QPoint posCurseur(mapFromGlobal(QCursor::pos()));
        if(posCurseur == dernierePositionCurseur && QRect(214, 9, 400, 400).contains(posCurseur))
        {
            curseurFixe++;
            if(curseurFixe == 2)
            {
                QString aff("");
                aff += QString::number(inverseInterpolation(posCurseur.x() - 214, minX, maxX), 'g', 4) + unites[grandeurX];
                if(grandeurYG != 0)
                {
                    aff += " ; " + QString::number(inverseInterpolation(400 - (posCurseur.y() - 9), minYG, maxYG), 'g', 4) + unites[grandeurYG];
                }
                if(grandeurYD != 0)
                {
                    aff += " ; " + QString::number(inverseInterpolation(400 - (posCurseur.y() - 9), minYD, maxYD), 'g', 4) + unites[grandeurYD];
                }
                QToolTip::showText(QCursor::pos(), aff);
            }
        }
        else
        {
            curseurFixe = 0;
            dernierePositionCurseur = posCurseur;
        }
    }
    QTimer::singleShot(100, this, SLOT(toolTipLoop()));
}

void Ordinateur::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    QPainter dessin;
    dessin.begin(this);

    //Axes
    dessin.setPen(QColor(128, 128, 128));
    dessin.drawLine(214, 409, 214, 9);
    dessin.drawLine(614, 409, 614, 9);
    dessin.drawLine(214, 409, 614, 409);

    //Traits
    double nombreX(pow(10, qFloor(log10((maxX - minX) * 0.8))));
    for (double t(qFloor(minX / nombreX) * nombreX); t <= maxX ; t += nombreX) {
        double calcul(interpolation(t, minX, maxX));
        dessin.drawLine(calcul + 214, 409, calcul + 214, 404);
    }
    double nombreYG(pow(10, qFloor(log10((maxYG - minYG) * 0.8))));
    for (double t(qFloor(minYG / nombreYG) * nombreYG); t <= maxYG ; t += nombreYG) {
        double calcul(interpolation(t, minYG, maxYG));
        dessin.drawLine(214, (400 - calcul) + 9, 219, (400 - calcul) + 9);
    }
    double nombreYD(pow(10, qFloor(log10((maxYD - minYD) * 0.8))));
    for (double t(qFloor(minYD / nombreYD) * nombreYD); t <= maxYD ; t += nombreYD) {
        double calcul(interpolation(t, minYD, maxYD));
        dessin.drawLine(614, (400 - calcul) + 9, 609, (400 - calcul) + 9);
    }

    //Points
    for(int p(0); p < ajouterPointsCoordonnees.size(); p++) {
        if(ajouterPointsAxe[p])
        {
            dessin.setPen(QColor(ajouterPointsCouleurs[ajouterPointsCouleursID[p]]));
            dessin.drawEllipse(interpolation(ajouterPointsCoordonnees[p].x(), minX, maxX) + 214, (400 - interpolation(ajouterPointsCoordonnees[p].y(), minYG, maxYG)) + 9, 1, 1);
        }
        else
        {
            dessin.setPen(QColor(ajouterPointsCouleurs[ajouterPointsCouleursID[p]]));
            dessin.drawEllipse(interpolation(ajouterPointsCoordonnees[p].x(), minX, maxX) + 214, (400 - interpolation(ajouterPointsCoordonnees[p].y(), minYD, maxYD)) + 9, 1, 1);
        }
    }

    //Textes axes
    dessin.setPen(QColor(64, 64, 64, 128));
    dessin.drawText(216, 19, affichage[grandeurYG]);
    dessin.drawText(612 - 6 * affichage[grandeurYD].size(), 19, affichage[grandeurYD]);
    dessin.drawText(216, 404, affichage[grandeurX]);
}
