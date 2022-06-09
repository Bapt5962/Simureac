#ifndef DESCRIPTIONFICHIER_H
#define DESCRIPTIONFICHIER_H

#include <QWidget>

namespace Ui {
class DescriptionFichier;
}

class DescriptionFichier : public QWidget
{
    Q_OBJECT

public:
    explicit DescriptionFichier(QString titre, QString description, QString chemin, QWidget *parent = nullptr);
    ~DescriptionFichier();
    void cocher(bool checked);
    bool estCoche() const;

    bool contient(QString recherche);

    QString getCheminFichier() const;

public slots:
    void on_Case_toggled();

signals:
     void coche(QString);

private:
    Ui::DescriptionFichier *ui;

    QString cheminFichier;
};

#endif // DESCRIPTIONFICHIER_H
