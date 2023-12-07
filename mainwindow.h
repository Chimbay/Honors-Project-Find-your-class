#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include <QListWidgetItem>
#include <QLineEdit>
#include <QLabel>
#include <QVector>
#include <vector>

#include "animations.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

struct NodeInfo{
    QLabel* label;
    QString courseName;

    NodeInfo(QLabel* l, const QString& name) : label(l), courseName(name) {}
};

class MainWindow : public QMainWindow{
    Q_OBJECT

    private:
        Ui::MainWindow *ui;

        // Check to see if bottombars are up or not
        bool bottomBarAtTopForAlerts = false;
        bool bottomBarAtTopForBookmarks = false;

        // Animation object to access objects of bottom bar
        Animations programAnimations;

        // Values that are assigned and customizable in the initializer
        int step;
        int leftSideOfTheMap;
        int stopWhereForX;
        int stopWhereForY;
        int borderOfBottom;
        int rightSideOfTheMap;
        int topSideOfTheMap;

        // Object to store nodes
        QListWidget *suggestionList;
        QVector<NodeInfo> nodesList;

        // Vectors to store names of rooms/locations
        std::vector<std::string> nodesPlaced;
        std::vector<std::vector<int>> coords;
        // Vector to store which nodes have been accessed
        std::vector<int> accessedAlready;

        std::vector<std::string> locations;
        // List to store lines from roooms/locations
        QList<QLabel *> connectionLines;


    protected:
        void keyPressEvent(QKeyEvent *event) override;
        bool eventFilter(QObject *obj, QEvent *event) override;

    private slots:


    // Functions
        // Buttons
        void on_bookmarksButton_clicked();
        void on_alertsButton_clicked();
        void on_exploreButton_clicked();

        // Out of bounds checking for the map
        bool outOfBoundsForPositiveX();
        bool outOfBoundsForNegativeX();
        bool outOfBoundsForPositiveY();
        bool outOfBoundsForNegativeY();

        // Nodes
        void pathFinding(QListWidgetItem *item);
        void alignNodes(QLabel *nodeLabel, int i);
        int findNearestNode(QPoint dotPos);
        void startAt(int nearestIndex, int findingIndex);
        double calculateDistance(const QPoint& pos1, const QPoint& pos2);
        // Node lines
        void addLines(QLabel *label1, QLabel *label2);
        void clearSolidLines();

        bool isAdjacent(const QPoint& pos1, const QPoint& pos2);
        int findAdjacentLabelIndex(int currentIndex, int findingIndex);

    // Methods for widgets
        // Searching
        void handleTextChanged(const QString &text);
        QStringList getMatchingSuggestions(const QString &text);
        void updateSuggestionList(const QStringList &suggestions);
        void handleSuggestionClicked(QListWidgetItem *item);

        // Initializers
        void valueInitializer();
        void searchBarInitializer();
        void nodeDotInitializer();
        void mappingInitializer();

        void suggestionListInitializer();
        void assignmentOfNodes();


    public:
        MainWindow(QWidget *parent = nullptr);

        // Destructor
        ~MainWindow();
};

#endif
