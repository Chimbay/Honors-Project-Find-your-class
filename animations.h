#ifndef ANIMATIONS_H
#define ANIMATIONS_H

#include <QObject>

#include "ui_mainwindow.h"

class Animations : public QObject{
    Q_OBJECT

    private:
        Ui::MainWindow *ui; // Member variable to store the ui object

        // Private constant member variables to store the original size and position
        QSize originalBottomBarSize;
        QPoint originalBottomBarPos;

    public:
        Animations(Ui::MainWindow *ui, QObject *parent = nullptr);

        void initialize();

        // These are the animations that make the smooth transaction of it going up and down
        void animateUp();
        void animateDown(bool);
        void opacityFinished();
        bool atTop();

        void onBottomBarDestroyed();
};

#endif // ANIMATIONS_H
