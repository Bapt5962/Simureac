#ifndef DIALOGAPROPOS_H
#define DIALOGAPROPOS_H

#include <QDialog>

namespace Ui {
class DialogAPropos;
}

class DialogAPropos : public QDialog
{
    Q_OBJECT

public:
    explicit DialogAPropos(QWidget *parent = nullptr);
    ~DialogAPropos();

public slots:
    void on_Fermer_clicked();

signals:
    void dialogQuit(int);

private:
    Ui::DialogAPropos *ui;
};

#endif // DIALOGAPROPOS_H
