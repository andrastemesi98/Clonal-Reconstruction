#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QListWidget>
#include <QProcess>
#include <QGroupBox>
#include <QRadioButton>
#include <QCheckBox>

#include "drawwidget.h"

class unittest; //to be able to test private functions

class MainWindow : public QMainWindow
{
    Q_OBJECT
    friend class unittest;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void clickedOne();
    void clickedTwo();
    void clickedThree();
    void clickedFour();
    void clickedFive();

    void clickedAlgo();
    void exampleGraph();
    void getFilePath();
    void FileOutName();
    void displayClicked();
    void runPython();
    void downloadFile();
    void setColors(int index);

    QGroupBox* pickType();
    QGroupBox* pickAlgo();
    QGroupBox* pickFile();
    QGroupBox* pickTarget();

private:
    QHBoxLayout *mainLayout;
    QVBoxLayout *buttonsLayout;
    QVBoxLayout *buttons2Layout;
    QVBoxLayout *contentLayout;
    QWidget *center;
    QVector<QPushButton*> buttonVector;
    QVector<QPushButton*> algoButtons;
    QVector<QPushButton*> testButtons;
    QLabel *welcomeText;
    QLabel *study1Label;
    QLabel *study2Label;
    QLabel *study3Label;
    QLabel *study4Label;
    QLabel *study5Label;
    QLabel *study6Label;
    QListWidget *studyList;
    QVBoxLayout *exampleOptions;
    QHBoxLayout *optionsOne;
    QHBoxLayout *optionsTwo;
    QVector<QVector<QPushButton*>> optionButtons;
    QPushButton *getInFile;
    QPushButton *getOutFile;
    QGridLayout *myGrid;
    QPushButton *sendData;

    QVector<QGroupBox*> groupBox;
    QVector<QRadioButton*> radio;
    QVector<QCheckBox*> checkBox;
    QVector<QVBoxLayout*> vbox;
    QGroupBox *generalBox;
    QString inPath;
    QString outFile;


    DrawWidget *myDraw = nullptr;

    void clearLayout(QLayout *layout);

private slots:
    void resizeEvent(QResizeEvent* event);

};

#endif // MAINWINDOW_H
