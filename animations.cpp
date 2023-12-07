#include "animations.h"

// To make the animations work

#include <QLabel>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QGraphicsOpacityEffect>

Animations::Animations(Ui::MainWindow *ui, QObject *parent) : QObject(parent), ui(ui) {

}

void Animations::initialize()
{
    // Initialization code
    originalBottomBarSize = ui->bottomBar->size();
    originalBottomBarPos = ui->bottomBar->pos();

    // Connects signals after ui is fully constructed
    connect(ui->bottomBar, &QLabel::destroyed, this, &Animations::opacityFinished);
}

void Animations::animateUp()
{
    QLabel *bottomBar = ui->bottomBar;

    // Get the current size and position of the QLabel
    QSize currentSize = bottomBar->size();
    QPoint currentPosition = bottomBar->pos();

    // Get the current position of the search bar
    QPoint positionOfSearch = ui->searchBar->pos();

    // Calculate the new height and position
    int newHeight = (currentPosition.y() - positionOfSearch.y()) - ui->searchBar->height() - 10;

    // Create property animations for size and position
    QPropertyAnimation *sizeAnimation = new QPropertyAnimation(bottomBar, "size");
    sizeAnimation->setEndValue(QSize(currentSize.width(), currentSize.height() + newHeight));
    sizeAnimation->setDuration(150);

    QPropertyAnimation *positionAnimation = new QPropertyAnimation(bottomBar, "pos");
    positionAnimation->setEndValue(QPoint(currentPosition.x(), currentPosition.y() - newHeight));
    positionAnimation->setDuration(150);

    // Create a parallel animation group to run both animations simultaneously
    QParallelAnimationGroup *animationGroup = new QParallelAnimationGroup(this);
    animationGroup->addAnimation(sizeAnimation);
    animationGroup->addAnimation(positionAnimation);

    // Starts the animations
    animationGroup->start();
}

void Animations::animateDown(bool animateUpAfter)
{
    QLabel *bottomBar = ui->bottomBar;

    // Create property animations for size and position to return to the original state
    QPropertyAnimation *sizeAnimation = new QPropertyAnimation(bottomBar, "size");
    sizeAnimation->setEndValue(originalBottomBarSize);
    sizeAnimation->setDuration(150);

    QPropertyAnimation *positionAnimation = new QPropertyAnimation(bottomBar, "pos");
    positionAnimation->setEndValue(originalBottomBarPos);
    positionAnimation->setDuration(150);

    // Create a parallel animation group to run both animations simultaneously
    QParallelAnimationGroup *animationGroup = new QParallelAnimationGroup(this);
    animationGroup->addAnimation(sizeAnimation);
    animationGroup->addAnimation(positionAnimation);

    // Connect the finished signal to onAnimateUpFinished only if animateUpAfter is true
    if (animateUpAfter) {
        connect(animationGroup, &QParallelAnimationGroup::finished, this, &Animations::animateUp);

    }

    // Start the animations
    animationGroup->start();
}

void Animations::opacityFinished()
{
    if(!atTop()){
        QGraphicsOpacityEffect *eff = new QGraphicsOpacityEffect(this);
        ui->searchBarOpacity->setGraphicsEffect(eff);

        QPropertyAnimation *animation = new QPropertyAnimation(eff, "opacity");
        animation->setDuration(400);
        animation->setStartValue(0);  // Start from fully transparent
        animation->setEndValue(1);    // End at fully opaque
        animation->setEasingCurve(QEasingCurve::InBack);

        connect(animation, &QPropertyAnimation::valueChanged, this, [this, eff]() {
            // Update the background color with opacity during the animation
            ui->searchBarOpacity->setStyleSheet(
                QString("background-color: rgba(255, 255, 255, %1);").arg(eff->opacity() * 255)
                );
        });

        animation->start(QPropertyAnimation::DeleteWhenStopped);

    } else{
        QGraphicsOpacityEffect *eff = new QGraphicsOpacityEffect(this);
        ui->searchBarOpacity->setGraphicsEffect(eff);

        QPropertyAnimation *animation = new QPropertyAnimation(eff, "opacity");
        animation->setDuration(300);
        animation->setStartValue(1);  // Start from fully transparent
        animation->setEndValue(0);    // End at fully opaque
        animation->setEasingCurve(QEasingCurve::InBack);

        connect(animation, &QPropertyAnimation::valueChanged, this, [this, eff]() {
            // Update the background color with opacity during the animation
            ui->searchBarOpacity->setStyleSheet(
                QString("background-color: rgba(255, 255, 255, %1);").arg(eff->opacity() * 255)
                );
        });

        animation->start(QPropertyAnimation::DeleteWhenStopped);
    }
}

bool Animations::atTop()
{
    return ui->bottomBar->pos().y() < originalBottomBarPos.y();
}

void Animations::onBottomBarDestroyed()
{
    // Disconnect signals when the bottom bar is destroyed
    disconnect(ui->bottomBar, &QLabel::destroyed, this, &Animations::opacityFinished);
}

