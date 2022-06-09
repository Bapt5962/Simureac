#include "ordinateurwidgetminmax.h"
#include "ui_ordinateurwidgetminmax.h"

OrdinateurWidgetMinMax::OrdinateurWidgetMinMax(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OrdinateurWidgetMinMax)
{
    ui->setupUi(this);
}

OrdinateurWidgetMinMax::~OrdinateurWidgetMinMax()
{
    delete ui;
}

void OrdinateurWidgetMinMax::verifier()
{
    if(ui->Auto->isChecked())
    {
        if(ui->XMin->value() < ui->XMax->value() && ui->YGMin->value() < ui->YGMax->value() && ui->YDMin->value() < ui->YDMax->value())
        {
            ui->Sauvegarder->setEnabled(true);
        }
        else
        {
            ui->Sauvegarder->setDisabled(true);
        }
    }
    else
    {
        ui->Sauvegarder->setEnabled(true);
    }
}

double OrdinateurWidgetMinMax::getMinX()
{
    return ui->XMin->value();
}

double OrdinateurWidgetMinMax::getMaxX()
{
    return ui->XMax->value();
}

double OrdinateurWidgetMinMax::getMinYG()
{
    return ui->YGMin->value();
}

double OrdinateurWidgetMinMax::getMaxYG()
{
    return ui->YGMax->value();
}

double OrdinateurWidgetMinMax::getMinYD()
{
    return ui->YDMin->value();
}

double OrdinateurWidgetMinMax::getMaxYD()
{
    return ui->YDMax->value();
}

bool OrdinateurWidgetMinMax::getLimite()
{
    return ui->Limite->isChecked();
}

bool OrdinateurWidgetMinMax::getManuel()
{
    return ui->Auto->isChecked();
}

void OrdinateurWidgetMinMax::on_XMin_valueChanged()
{
    verifier();
}

void OrdinateurWidgetMinMax::on_XMax_valueChanged()
{
    verifier();
}

void OrdinateurWidgetMinMax::on_YGMin_valueChanged()
{
    verifier();
}

void OrdinateurWidgetMinMax::on_YGMax_valueChanged()
{
    verifier();
}

void OrdinateurWidgetMinMax::on_YDMin_valueChanged()
{
    verifier();
}

void OrdinateurWidgetMinMax::on_YDMax_valueChanged()
{
    verifier();
}

void OrdinateurWidgetMinMax::on_Limite_toggled()
{
    verifier();
}

void OrdinateurWidgetMinMax::on_Auto_toggled()
{
    verifier();
}

void OrdinateurWidgetMinMax::on_Sauvegarder_clicked()
{
    emit changement();
}

void OrdinateurWidgetMinMax::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    QPainter dessin;
    dessin.begin(this);
    dessin.fillRect(0, 0, width(), height(), QColor(196, 196, 196, 225));
}
