#include <QLabel>
#include <QPixmap>
#include <QWidget>
#include <QVBoxLayout>
#include <QKeyEvent>
#include <QMouseEvent>

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsRectItem>
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>

#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "animations.h"

#include <QPainter>
#include <cmath>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow), programAnimations(ui, this)
{
    // Initialize ui
    ui->setupUi(this);
    // Initialize EventFilter
    ui->map->installEventFilter(this);
    ui->searchBar->installEventFilter(this);

    // Initialize programAnimations
    programAnimations.initialize();

    // Initializations for proper organization
    valueInitializer();
    mappingInitializer();
    assignmentOfNodes();
    nodeDotInitializer();
    searchBarInitializer();
    suggestionListInitializer();
}

MainWindow::~MainWindow(){
    // Disconnect signals in the destructor
    delete ui;
}

// Initializers
void MainWindow::valueInitializer(){
    step = 7;  // Movement

    leftSideOfTheMap = 0 + step; // The left side of the UI. But we want to make sure it doesnt pass it so it would be one step after.
    rightSideOfTheMap = (-(ui->map->size().width() - size().width()) / step) * step - step; // Finds the right side of the UI.

    topSideOfTheMap = 0 + step; // The top of the UI. But we want to make sure it doesnt pass so it would be one step after.
    borderOfBottom = (-(ui->map->size().height() - ui->buttonHolderBar->pos().y()) / step) * step - step; // Finds the top border of the buttons to stop

    stopWhereForX = size().width() / 2; // Gets a division of where the dot should stop.
    stopWhereForY = size().height() / 2; // Gets. division of where the dot should stop.
}
void MainWindow::mappingInitializer(){
    // Sets up the map
    QPixmap pixmap;
    pixmap.load("/Users/tony/Downloads/mapLayoutFloor.png");
    ui->map->setPixmap(pixmap);

    ui->map->move(-525, -250);

    // Sets size for the dot.
    ui->dot->setFixedSize(10, 10);
    ui->dot->move(180, 380);
}
// Assignments for Nodes
void MainWindow::assignmentOfNodes(){
    // List that initializes the nodes placed along with the coords
    nodesPlaced = {"Elev 1", "Elev 1: front", "Hall 1", "Hall 1: room", "F1201", "Hall 3: room", "Mens bathroom", "Elev 2", "Elev 2: front", "Hall 2", "F1202"};
    coords = {{180, 315}, {180, 200}, {370, 200}, {370, 240}, {400, 240}, {30, 200}, {30, 225}, {-125, 315}, {-125, 200}, {-310,200}, {-310, 255}};
    // Actual rooms
    locations = {"Elev 1", "F1201", "Elev 2", "Mens bathroom", "F1202"};
}
void MainWindow::nodeDotInitializer(){
    // Sets instances of objects/nodes for pathfinding.
    for (int i = 0; i < coords.size(); ++i){
        QLabel* nodeLabel = new QLabel(this);
        nodeLabel->setStyleSheet("background-color: pink;");
        nodeLabel->setFixedSize(10, 10);
        nodeLabel->hide();

        alignNodes(nodeLabel, i);
    }
}
void MainWindow::alignNodes(QLabel *nodeLabel, int i){
    if(!nodesPlaced[i].empty()){
        nodesList.append(NodeInfo(nodeLabel, QString::fromStdString(nodesPlaced[i])));
        nodesList[i].label->move(coords[i][0], coords[i][1]);
    }
}

void MainWindow::searchBarInitializer(){
    // Set a search placeholder text
    ui->searchBar->setPlaceholderText("Search...");
    // Reads input from typing in the search bar
    connect(ui->searchBar, &QLineEdit::textChanged, this, &MainWindow::handleTextChanged);
}
void MainWindow::suggestionListInitializer(){
    // Setting up a new suggestion list
    suggestionList = new QListWidget(this);
    suggestionList->setHidden(true);
    // Properities to design the suggestion list
    suggestionList->setStyleSheet("background-color: white; border: 1px solid black; color: black;");
    // To move the suggestion list right under the search bar
    suggestionList->move(ui->searchBar->pos().x(), ui->searchBar->pos().y() + ui->searchBar->size().height());

    // Connecting suggestion list so when clicked, handlesuggesitonClicked registers it
    connect(suggestionList, &QListWidget::itemClicked, this, &MainWindow::handleSuggestionClicked);
}
bool MainWindow::eventFilter(QObject *obj, QEvent *event){
    if (event->type() == QEvent::MouseButtonPress) {
        // Handle mouse press event
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);

        if (obj == ui->map) {
            // Clicking on the map allows arrow key movement
            ui->map->setFocus();
        } else if (obj == ui->searchBar) {
            // Clicking on the search bar allows typing
            ui->searchBar->setFocus();
        }
    }
    // Continue with the normal event processing
    return QObject::eventFilter(obj, event);
}

void MainWindow::on_bookmarksButton_clicked(){
    // Checks whether bookmarks is not up.
    if(programAnimations.atTop() == true && bottomBarAtTopForBookmarks == false){
        programAnimations.animateDown(true);
    }

    if(programAnimations.atTop() == false && bottomBarAtTopForBookmarks == false){
        programAnimations.animateUp();
        programAnimations.opacityFinished();
    }

    bottomBarAtTopForBookmarks = true;
    bottomBarAtTopForAlerts = false;
}
void MainWindow::on_exploreButton_clicked(){
    // Checks whether explore is already down
    programAnimations.animateDown(false);
    if(programAnimations.atTop()){
        programAnimations.opacityFinished();
    }

    bottomBarAtTopForAlerts = false;
    bottomBarAtTopForBookmarks = false;
}
void MainWindow::on_alertsButton_clicked(){
    // Checks whether alerts is up
    if(programAnimations.atTop() == true && bottomBarAtTopForAlerts == false){
        programAnimations.animateDown(true);
    }

    if(programAnimations.atTop() == false && bottomBarAtTopForAlerts == false){
        programAnimations.animateUp();
        programAnimations.opacityFinished();
    }

    bottomBarAtTopForAlerts = true;
    bottomBarAtTopForBookmarks = false;
}

void MainWindow::keyPressEvent(QKeyEvent *event){
    // Moves the map based on the pressed key
    switch (event->key()) {
        case Qt::Key_Left:

            if(ui->dot->pos().x() > stopWhereForX || outOfBoundsForNegativeX()){
                ui->dot->move(ui->dot->pos().x() - step, ui->dot->pos().y());
            }

            if(!outOfBoundsForNegativeX() && !(ui->dot->pos().x() > stopWhereForX)){
                ui->map->move(ui->map->pos().x() + step, ui->map->pos().y());
                for (const NodeInfo &nodes : nodesList){
                    nodes.label->move(nodes.label->pos().x() + step, nodes.label->pos().y());
                }
                for(QLabel* line : connectionLines){
                    line->move(line->pos().x() + step, line->pos().y());
                }
            }
            break;

        case Qt::Key_Right:

            if(ui->dot->pos().x() < stopWhereForX || outOfBoundsForPositiveX()){
                ui->dot->move(ui->dot->pos().x() + step, ui->dot->pos().y());
            }

            if(!outOfBoundsForPositiveX() && !(ui->dot->pos().x() < stopWhereForX)){
                ui->map->move(ui->map->pos().x() - step, ui->map->pos().y());
                for (const NodeInfo &nodes : nodesList){
                    nodes.label->move(nodes.label->pos().x() - step, nodes.label->pos().y());
                }
                for(QLabel* line : connectionLines){
                    line->move(line->pos().x() - step, line->pos().y());
                }
            }
            break;

        case Qt::Key_Up:

            if(ui->dot->pos().y() > stopWhereForY || outOfBoundsForPositiveY()){
                ui->dot->move(ui->dot->pos().x(), ui->dot->pos().y() - step);
            }

            if(!outOfBoundsForPositiveY() && !(ui->dot->pos().y() > stopWhereForY)){
                ui->map->move(ui->map->pos().x(), ui->map->pos().y() + step);
                for (const NodeInfo &nodes : nodesList){
                    nodes.label->move(nodes.label->pos().x(), nodes.label->pos().y() + step);
                }
                for(QLabel* line : connectionLines){
                    line->move(line->pos().x(), line->pos().y() + step);
                }
            }
            break;

        case Qt::Key_Down:

            if(ui->dot->pos().y() < stopWhereForY || outOfBoundsForNegativeY()){
                ui->dot->move(ui->dot->pos().x(), ui->dot->pos().y() + step);
            }

            if(!outOfBoundsForNegativeY() && !(ui->dot->pos().y() < stopWhereForY)){
                ui->map->move(ui->map->pos().x(), ui->map->pos().y() - step);
                for (const NodeInfo &nodes : nodesList){
                    nodes.label->move(nodes.label->pos().x(), nodes.label->pos().y() - step);
                }
                for(QLabel* line : connectionLines){
                    line->move(line->pos().x(), line->pos().y() - step);
                }
            }
            break;

        default:
            break;
    }


    qDebug() << "Cords rel to map: ("<< ui->dot->pos().x() - ui->map->pos().x() - 525  << ", " << ui->dot->pos().y() - ui->map->pos().y() - 250<< ")";
    qDebug() << "Cords of map: " << ui->map->pos();
    qDebug() << "Cords of dot" << ui->dot->pos();

    qDebug() << "-------------------------------------------------------------";
}

// Out of bounds check for the map
bool MainWindow::outOfBoundsForPositiveX(){
    return ui->map->pos().x() - step <= rightSideOfTheMap ? true : false;
}
bool MainWindow::outOfBoundsForNegativeX(){
    return ui->map->pos().x() + step >= leftSideOfTheMap ? true : false;
}

bool MainWindow::outOfBoundsForPositiveY(){
    return ui->map->pos().y() + step >= topSideOfTheMap ? true : false;
}
bool MainWindow::outOfBoundsForNegativeY(){
    return ui->map->pos().y() - step <= borderOfBottom ? true : false;
}

void MainWindow::handleTextChanged(const QString &text){
    // Stores and fetches suggestions
    QStringList suggestions = getMatchingSuggestions(text);

    // Update the suggestion list widget
    updateSuggestionList(suggestions);

    // Show or hide the suggestion list based on whether there are suggestions
    suggestionList->setHidden(suggestions.isEmpty());
}
QStringList MainWindow::getMatchingSuggestions(const QString &text){
    QStringList suggestions;

    if (!text.trimmed().isEmpty()) {
        for (const std::string &room : locations) {
            if (QString::fromStdString(room).contains(text, Qt::CaseInsensitive)) {
                suggestions << QString::fromStdString(room);
            }
        }
    }
    suggestionList->resize(ui->searchBar->size().width(), (suggestions.size() * 20) - (suggestions.size() * 2));

    return suggestions;
}
void MainWindow::updateSuggestionList(const QStringList &suggestions){
    suggestionList->clear();

    suggestionList->setSelectionMode(QAbstractItemView::SingleSelection);

    // Add each suggestion to the list
    for (const QString &suggestion : suggestions) {
        suggestionList->addItem(suggestion);
    }
}

void MainWindow::handleSuggestionClicked(QListWidgetItem *item){
    if (item){
        clearSolidLines();
        accessedAlready.clear();
        pathFinding(item);
    }
}

void MainWindow::pathFinding(QListWidgetItem *item){
    const std::string clickedText = item->text().toStdString();
    const QPoint dotPos(ui->dot->pos().x() - ui->map->pos().x() - 525, ui->dot->pos().y() - ui->map->pos().y() - 250);
    const int nearestIndex = findNearestNode(dotPos);

    // Iterates through the nodesPlaced
    for (int i = 0; i < nodesPlaced.size(); ++i) {
        std::string roomQString = nodesPlaced[i];
        // Checks if the clicked item matches the current room
        if (clickedText == roomQString){
            qDebug() << "Index trying to be found:" << i << "and name " << roomQString;
            qDebug() << "Nearest node is" << nodesPlaced[nearestIndex] << " in place: " << nearestIndex;
            startAt(nearestIndex, i);
            break;
        }
    }
}
int MainWindow::findNearestNode(QPoint dotPos){
    int minDistance = std::numeric_limits<int>::max();
    int nearestNodeIndex = -1;

    for (int i = 0; i < coords.size(); ++i) {
        int distance = std::hypot(coords[i][0] - dotPos.x(), coords[i][1] - dotPos.y());

        if (distance < minDistance) {
            minDistance = distance;
            nearestNodeIndex = i;
        }
    }

    if (nearestNodeIndex != -1) {
        return nearestNodeIndex;
    } else{
        qDebug() << "ERROR: CANT FIND NEARESTNODE";
    }
}
void MainWindow::startAt(int nearestIndex, int findingIndex) {
    bool found = false;
    int currentIndex = nearestIndex;
    int adjacentIndex;
    QPoint position(nodesList[currentIndex].label->pos().x(), nodesList[currentIndex].label->pos().y());
    const QPoint endingPosition(nodesList[findingIndex].label->pos().x(), nodesList[findingIndex].label->pos().y());

    qDebug() << "Start pos: " << position << "End pos: " << endingPosition;

    while (!found) {
        adjacentIndex = findAdjacentLabelIndex(currentIndex, findingIndex);
        qDebug() << "Current index: " << currentIndex << "finding index: " << findingIndex;
        qDebug() << "Adjacent Index: " << adjacentIndex << nodesPlaced[adjacentIndex];

        if (adjacentIndex != -1) {
            addLines(nodesList[currentIndex].label, nodesList[adjacentIndex].label);
            currentIndex = adjacentIndex;
        } else{
            found = true;  // No adjacent label found, exit the loop
        }

        // Check if the current position is the ending position
        if (position == endingPosition) {
            found = true;
        }

        qDebug() << "-----------------------------------------------";
    }
}

int MainWindow::findAdjacentLabelIndex(int currentIndex, int findingIndex) {
    const QPoint& currentPosition = nodesList[currentIndex].label->pos();
    double distance = calculateDistance(nodesList[currentIndex].label->pos(), nodesList[findingIndex].label->pos());
    qDebug() << "Distance is: " << distance;

    int closestAdjacentIndex = -1;
    double closestAdjacentDistance = std::numeric_limits<double>::max();

    for (int i = 0; i < nodesList.size(); ++i) {
        bool indexAlreadyAccessed = std::find(accessedAlready.begin(), accessedAlready.end(), i) != accessedAlready.end();

        qDebug() << "Index: " << i << nodesPlaced[i] << " Already accessed? " << indexAlreadyAccessed;

        if (i != currentIndex && !indexAlreadyAccessed) {
            const QPoint& otherPosition = nodesList[i].label->pos();

            if (!((nodesPlaced[currentIndex] == "Elev 1" && nodesPlaced[i] == "Elev 2") ||
                  (nodesPlaced[currentIndex] == "Elev 2" && nodesPlaced[i] == "Elev 1"))) {

                double newDistance = calculateDistance(otherPosition, nodesList[findingIndex].label->pos());
                qDebug() << "Distance for not accessed with: " << nodesPlaced[currentIndex] << " and " << nodesPlaced[i] << ":" << newDistance;

                if (isAdjacent(currentPosition, otherPosition) && newDistance < closestAdjacentDistance) {
                    qDebug() << "Being compared: " << nodesPlaced[currentIndex] << " and " << nodesPlaced[i];
                    qDebug() << "Being compared pos:" << currentPosition << " and " << otherPosition;
                    closestAdjacentDistance = newDistance;
                    closestAdjacentIndex = i;
                }
            }
        }
    }

    if (closestAdjacentIndex != -1) {
        accessedAlready.push_back(closestAdjacentIndex);
    }

    return closestAdjacentIndex;
}

double MainWindow::calculateDistance(const QPoint& pos1, const QPoint& pos2){
    int dx = pos1.x() - pos2.x();
    int dy = pos1.y() - pos2.y();
    return std::sqrt(dx * dx + dy * dy);
}

// Function to check if two QPoint positions are adjacent
bool MainWindow::isAdjacent(const QPoint& pos1, const QPoint& pos2) {
    return pos1.x() == pos2.x() || pos1.y() == pos2.y();
}

void MainWindow::addLines(QLabel *label1, QLabel *label2){
    QLabel *lines = new QLabel(this);
    int height = 0, width = 0;

    if (label1->pos().x() == label2->pos().x()) {
        height = abs(label1->pos().y() - label2->pos().y());
        width = 3;
    } else if (label1->pos().y() == label2->pos().y()) {
        width = abs(label1->pos().x() - label2->pos().x());
        height = 3;
    } else {
        qDebug() << "Error!! Labels don't match at the same x or y";
    }

    lines->setGeometry(0, 0, width, height);
    lines->setStyleSheet("background-color: rgb(18, 140, 181)");

    // Calculate the position of the line based on the centers of the labels
    QPoint center1(label1->pos().x() + label1->width() / 2, label1->pos().y() + label1->height() / 2);
    QPoint center2(label2->pos().x() + label2->width() / 2, label2->pos().y() + label2->height() / 2);

    // Calculate the position to start the line from the center of label1
    QPoint linePos((center1.x() + center2.x()) / 2, (center1.y() + center2.y()) / 2);
    linePos.setX(linePos.x() - lines->width() / 2);
    linePos.setY(linePos.y() - lines->height() / 2);

    lines->move(linePos);
    lines->show();
    connectionLines.append(lines);
}
void MainWindow::clearSolidLines(){
    // Iterates through the solidLines list and deletes each allocation
    for (QLabel *lines : connectionLines) {
        delete lines;
    }
    // Clear the solidLines list
    connectionLines.clear();
}
