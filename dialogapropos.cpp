#include "dialogapropos.h"
#include "ui_dialogapropos.h"

DialogAPropos::DialogAPropos(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogAPropos)
{
    ui->setupUi(this);
    ui->Img->setPixmap(QPixmap("data/assets/splash.png"));
    ui->Qt->setIcon(QIcon(QPixmap("data/assets/qt.png")));
    ui->Qt->setIconSize(QSize(64, 64));

    setAttribute(Qt::WA_DeleteOnClose);
}

DialogAPropos::~DialogAPropos()
{
    delete ui;
    emit dialogQuit(5);
}

void DialogAPropos::on_Fermer_clicked()
{
    close();
}

void DialogAPropos::on_Qt_clicked()
{
    QDesktopServices::openUrl(QUrl("https://www.qt.io/"));
}
