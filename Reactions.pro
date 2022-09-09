QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    chronometre.cpp \
    colorimetre.cpp \
    conductimetre.cpp \
    descriptionfichier.cpp \
    dialogaide.cpp \
    dialogapropos.cpp \
    dialogchargersimulation.cpp \
    dialogeditionparticule.cpp \
    dialogeditionreaction.cpp \
    dialogimporterparticule.cpp \
    dialogparticules.cpp \
    dialogreactions.cpp \
    dialogremplissage.cpp \
    dialogsauvegardeparticule.cpp \
    dialogsauvegardesimulation.cpp \
    dialogsimulation.cpp \
    donneeparticule.cpp \
    donneereaction.cpp \
    filetext.cpp \
    general.cpp \
    instrument.cpp \
    listedialogedition.cpp \
    listeparticulereaction.cpp \
    main.cpp \
    mainwindow.cpp \
    ordinateur.cpp \
    ordinateurwidgetminmax.cpp \
    particule.cpp \
    particuleajout.cpp \
    particuleliste.cpp \
    phmetre.cpp \
    remplissage.cpp \
    simulation.cpp \
    thermometre.cpp

HEADERS += \
    chronometre.h \
    colorimetre.h \
    conductimetre.h \
    descriptionfichier.h \
    dialogaide.h \
    dialogapropos.h \
    dialogchargersimulation.h \
    dialogeditionparticule.h \
    dialogeditionreaction.h \
    dialogimporterparticule.h \
    dialogparticules.h \
    dialogreactions.h \
    dialogremplissage.h \
    dialogsauvegardeparticule.h \
    dialogsauvegardesimulation.h \
    dialogsimulation.h \
    donneeparticule.h \
    donneereaction.h \
    filetext.h \
    general.h \
    instrument.h \
    listedialogedition.h \
    listeparticulereaction.h \
    mainwindow.h \
    ordinateur.h \
    ordinateurwidgetminmax.h \
    particule.h \
    particuleajout.h \
    particuleliste.h \
    phmetre.h \
    remplissage.h \
    simulation.h \
    thermometre.h

FORMS += \
    descriptionfichier.ui \
    dialogaide.ui \
    dialogapropos.ui \
    dialogchargersimulation.ui \
    dialogeditionparticule.ui \
    dialogeditionreaction.ui \
    dialogimporterparticule.ui \
    dialogparticules.ui \
    dialogreactions.ui \
    dialogremplissage.ui \
    dialogsauvegardeparticule.ui \
    dialogsauvegardesimulation.ui \
    dialogsimulation.ui \
    listedialogedition.ui \
    listeparticulereaction.ui \
    mainwindow.ui \
    ordinateur.ui \
    ordinateurwidgetminmax.ui \
    particuleajout.ui \
    particuleliste.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
