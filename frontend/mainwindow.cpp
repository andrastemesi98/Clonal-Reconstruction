#include "mainwindow.h"

#include <QInputDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QCoreApplication>
#include <QFileDialog>
#include <QDesktopServices>

#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    //Create basic layout
    //center Widget will have mainLayout
    //left side of mainLayout->buttonLayout, right side->contentLayout
    setWindowTitle(tr("Speciális Gráfalgoritmusok"));
    resize(600, 400);
    setMinimumWidth(600);
    setMinimumHeight(400);
    center = new QWidget();
    mainLayout = new QHBoxLayout();
    buttonsLayout = new QVBoxLayout();
    contentLayout = new QVBoxLayout();
    mainLayout->addLayout(buttonsLayout);
    mainLayout->addLayout(contentLayout);
    center->setLayout(mainLayout);

    //Add menu buttons
    //Set buttoncolor yellow
    buttonVector.resize(5);
    for (int i=0; i<5; ++i)
    {
        buttonVector[i] = new QPushButton();
        buttonsLayout->addWidget(buttonVector[i]);
        buttonVector[i]->setStyleSheet("background-color: #FFFFB7; color: black; border: 4px solid black; font: bold");
    }

    //If menubutton clicked then call specific slot
    buttonVector[0]->setText(tr("Feladat\n bemutatása"));
    connect(buttonVector[0], SIGNAL(clicked()), this, SLOT(clickedOne()));
    buttonVector[1]->setText(tr("Megoldás\n adott\n számsorra"));
    connect(buttonVector[1], SIGNAL(clicked()), this, SLOT(clickedTwo()));
    buttonVector[2]->setText(tr("Tesztelési\n eredmények"));
    connect(buttonVector[2], SIGNAL(clicked()), this, SLOT(clickedThree()));
    buttonVector[3]->setText(tr("Haladó\n funkciók"));
    connect(buttonVector[3], SIGNAL(clicked()), this, SLOT(clickedFour()));
    buttonVector[4]->setText(tr("Súgó és\n információk"));
    connect(buttonVector[4], SIGNAL(clicked()), this, SLOT(clickedFive()));

    //Display all
    setCentralWidget(center);

    //First page should be the information section
    clickedFive();
}

//Set the size of menubuttons according to window size
//will be about 1/5th of the window dimensions
void MainWindow::resizeEvent(QResizeEvent* event)
{
   QMainWindow::resizeEvent(event);
   for (int i=0; i<5; ++i)
   {
       buttonVector[i]->setFixedWidth(size().rwidth()/5);
       buttonVector[i]->setFixedHeight((size().rheight()*9)/50);
   }
}

//Delete every item from a Layout
//free up the dynamically allocated memory
void MainWindow::clearLayout(QLayout *layout)
{
    QLayoutItem *item;
    while((item = layout->takeAt(0)))
    {
        if (item->layout())
        {
            clearLayout(item->layout()); //first we have to delete objects in the layout
        }
        if (item->widget())
        {
           delete item->widget(); //widgets can be deleted directly
           item = nullptr;
        }
        delete item;
        item = nullptr;
    }
    if(myDraw!=nullptr){
        delete myDraw;
        myDraw = nullptr;
    }
}

//first menubutton clicked
void MainWindow::clickedOne()
{
    //Delete right side and make buttoncolor red
    clearLayout(contentLayout);
    setColors(0);

    //Create right side
    welcomeText = new QLabel();
    welcomeText->setText("Tanulás: Mi maga a probléma?");
    welcomeText->setAlignment(Qt::AlignTop);
    welcomeText->setStyleSheet("font: 15pt");

    study1Label = new QLabel();
    study1Label->setText("Adott egy pozitív számokat tartalmazó lista. "
                         "Egy 'helyes' gráfot szeretnénk építeni a következő feltételekkel: (egyszerű eset)");
    study1Label->setWordWrap(true);
    studyList = new QListWidget();
    studyList->addItem("*Lista elemei a csúcsok, a gráf egy irányított fa");
    studyList->addItem("*Első csúcs kivételével minden csúcsba pontosan egy bemenő él");
    studyList->addItem("*Minden élre a kiindulócsúcs listaindexe kisebb, mint a célcsúcsé");
    studyList->addItem("*Minden csúcs értéke nagyobb, mint a gyerekei értékeinek összege");
    studyList->addItem("*A 'megengedett' gráfokból szeretnénk megtalálni az optimálisat");
    studyList->setStyleSheet("background: #FFFFB7; font: italic; border: 2px solid black");

    //Create layout for buttons
    exampleOptions = new QVBoxLayout;
    optionsOne = new QHBoxLayout;
    optionsTwo = new QHBoxLayout;
    exampleOptions->addLayout(optionsOne);
    exampleOptions->addLayout(optionsTwo);

    //Add buttons
    optionButtons.resize(2);
    for(int i=0; i<2; ++i)
    {
        optionButtons[i].resize(3);
    }
    for(int j=0; j<3; ++j)
    {
        optionButtons[0][j] = new QPushButton();
        optionButtons[0][j]->setContentsMargins(0,0,0,0);
        optionsOne->addWidget(optionButtons[0][j]);
    }
    for(int j=0; j<3; ++j)
    {
        optionButtons[1][j] = new QPushButton();
        optionButtons[1][j]->setContentsMargins(0,0,0,0);
        optionsTwo->addWidget(optionButtons[1][j]);
    }

    //Set text on the button, if clicked call right slot
    optionButtons[0][0]->setText(tr("Hibás#1"));
    optionButtons[0][1]->setText(tr("Hibás#2"));
    optionButtons[0][2]->setText(tr("Hibás#3"));
    optionButtons[1][0]->setText(tr("Helyes#1"));
    optionButtons[1][1]->setText(tr("Helyes#2"));
    optionButtons[1][2]->setText(tr("Helyes#3"));
    connect(optionButtons[0][0], SIGNAL(clicked()), this, SLOT(exampleGraph()));
    connect(optionButtons[0][1], SIGNAL(clicked()), this, SLOT(exampleGraph()));
    connect(optionButtons[0][2], SIGNAL(clicked()), this, SLOT(exampleGraph()));
    connect(optionButtons[1][0], SIGNAL(clicked()), this, SLOT(exampleGraph()));
    connect(optionButtons[1][1], SIGNAL(clicked()), this, SLOT(exampleGraph()));
    connect(optionButtons[1][2], SIGNAL(clicked()), this, SLOT(exampleGraph()));

    //Add the objects on the right side
    contentLayout->addWidget(welcomeText);
    contentLayout->addWidget(study1Label);
    contentLayout->addWidget(studyList);
    contentLayout->addLayout(exampleOptions);

    //Create scene with simple text
    myDraw = new DrawWidget();
    contentLayout->addWidget(myDraw);
    myDraw->welcomeText();
}

//second menubutton clicked
void MainWindow::clickedTwo()
{
    //Delete right side and make buttoncolor red
    clearLayout(contentLayout);
    setColors(1);

    //Create right side
    welcomeText = new QLabel();
    welcomeText->setText(tr("Konkrét példa megoldása és szemléltetése"));
    welcomeText->setAlignment(Qt::AlignTop);
    welcomeText->setStyleSheet("font: 15pt");
    study1Label = new QLabel();
    study1Label->setText("Algoritmus választása:");

    //Add buttons
    buttons2Layout = new QVBoxLayout();
    algoButtons.resize(5);
    for(int i=0; i<5; ++i)
    {
        algoButtons[i] = new QPushButton();
        algoButtons[i]->setStyleSheet("background-color: #FFFFB7; color: black; border: 1px solid black");
        buttons2Layout->addWidget(algoButtons[i]);
        buttons2Layout->setAlignment(algoButtons[i], Qt::AlignTop);
    }
    algoButtons[0]->setText(tr("Precheck+rekurzív"));
    connect(algoButtons[0], SIGNAL(clicked()), this, SLOT(clickedAlgo()));
    algoButtons[1]->setText(tr("Precheck"));
    connect(algoButtons[1], SIGNAL(clicked()), this, SLOT(clickedAlgo()));
    algoButtons[2]->setText(tr("Naív-keresés"));
    connect(algoButtons[2], SIGNAL(clicked()), this, SLOT(clickedAlgo()));
    algoButtons[3]->setText(tr("Mohó-keresés"));
    connect(algoButtons[3], SIGNAL(clicked()), this, SLOT(clickedAlgo()));
    algoButtons[4]->setText(tr("Megengedettségi tipp"));
    connect(algoButtons[4], SIGNAL(clicked()), this, SLOT(clickedAlgo()));

    study2Label = new QLabel();
    study2Label->setText("Maximum 5mp-es futásidő! Kérem addig várjon!\nMegjelenítés négy számjegyre csonkol. Max. 2^31");
    buttons2Layout->addWidget(study2Label);
    buttons2Layout->addStretch(); //Align top

    //Add content on right side
    contentLayout->addWidget(welcomeText);
    contentLayout->addWidget(study1Label);
    contentLayout->addLayout(buttons2Layout);
}

//third menubutton clicked
void MainWindow::clickedThree()
{
    //Delete right side and make buttoncolor red
    clearLayout(contentLayout);
    setColors(2);

    //Create right side
    welcomeText = new QLabel();
    welcomeText->setText(tr("Algoritmus helyessége és előnyei\n(internetelérés szükséges)"));
    welcomeText->setAlignment(Qt::AlignTop);
    welcomeText->setStyleSheet("font: 14.5pt");
    study1Label = new QLabel();
    study1Label->setText(tr("Az algoritmusom helyessége: a kapott fa függvényértéke minden esetben "
                            "megegyezik a naív keresés esetén kapott függvényértékkel."));
    study1Label->setStyleSheet("background: #FFFFB7; font: italic; border: 1px solid black");
    study1Label->setWordWrap(true); //More lines
    study1Label->setAlignment(Qt::AlignTop);
    study2Label = new QLabel();
    study2Label->setText(tr("Egyszerű eset: 100 darab szimulált 8 klónos minta esetén:"));
    study2Label->setAlignment(Qt::AlignTop);
    study2Label->setStyleSheet("font: 10.5pt");
    testButtons.resize(4);
    testButtons[0] = new QPushButton();
    testButtons[0]->setText(tr("results1.xls letöltése"));
    study3Label = new QLabel();
    study3Label->setText(tr("Általános eset: 100 darab szimulált 10 klónos minta esetén:"));
    study3Label->setStyleSheet("font: 10.5pt");
    testButtons[1] = new QPushButton();
    testButtons[1]->setText(tr("results2.xls letöltése"));
    study4Label = new QLabel();
    study4Label->setText("Az algoritmusom előnye: mindig az optimális fát találja meg, és az élek egy részét "
                         "polinomiális költséggel határozza meg. ");
    study4Label->setWordWrap(true);
    study4Label->setStyleSheet("background: #FFFFB7; font: italic; border: 1px solid black");
    study6Label = new QLabel();
    study6Label->setText(tr("Egyszerű eset: 100 darab szimulált 10 klónos mintára az algoritmusom átlagosan 110%-kal nagyobb "
                            "függvényértékű fát talál, mint a mohó:"));
    study6Label->setWordWrap(true);
    study6Label->setStyleSheet("font: 10.5pt");
    testButtons[2] = new QPushButton();
    testButtons[2]->setText(tr("results5.xls letöltése"));
    study5Label = new QLabel();
    study5Label->setText("Általános eset: 100 darab szimulált 20 klónos mintára az élek kétharmadát helyesen "
                         "azonosítjuk polinomiális költséggel:");
    study5Label->setWordWrap(true);
    study5Label->setStyleSheet("font: 10.5pt");
    testButtons[3] = new QPushButton();
    testButtons[3]->setText(tr("results7.xls letöltése"));

    //If buttons clicked call necessary slot
    for(int i=0; i<testButtons.size(); ++i)
    {
        connect(testButtons[i], SIGNAL(clicked()), this, SLOT(downloadFile()));
    }

    //Add content on the right side in this order
    contentLayout->addWidget(welcomeText);
    contentLayout->addWidget(study1Label);
    contentLayout->addWidget(study2Label);
    contentLayout->addWidget(testButtons[0]);
    contentLayout->addWidget(study3Label);
    contentLayout->addWidget(testButtons[1]);
    contentLayout->addWidget(study4Label);
    contentLayout->addWidget(study6Label);
    contentLayout->addWidget(testButtons[2]);
    contentLayout->addWidget(study5Label);
    contentLayout->addWidget(testButtons[3]);
    contentLayout->setAlignment(Qt::AlignTop);
}

//fourth menubutton clicked
void MainWindow::clickedFour()
{
    //Delete right side and set color of button
    clearLayout(contentLayout);
    setColors(3);

    //Create new right side
    welcomeText = new QLabel();
    welcomeText->setText(tr("Haladó funkciók"));
    welcomeText->setAlignment(Qt::AlignTop);
    welcomeText->setStyleSheet("font: 15pt");
    QGridLayout *myGrid = new QGridLayout;

    //These will be set by the user
    inPath="";
    outFile="";
    groupBox.resize(4);
    checkBox.resize(7);
    radio.resize(4);
    vbox.resize(5);

    //Create blank form
    myGrid->addWidget(pickType(), 0, 0);
    myGrid->addWidget(pickAlgo(), 0, 1);
    myGrid->addWidget(pickFile(), 1, 0);
    myGrid->addWidget(pickTarget(), 1, 1);

    //Add buttun
    sendData = new QPushButton();
    sendData->setText("Futtatás");
    sendData->setDefault(true);
    connect(sendData, SIGNAL(clicked()), this, SLOT(runPython()));

    //Add right side
    contentLayout->addWidget(welcomeText);
    contentLayout->addLayout(myGrid);
    contentLayout->addWidget(sendData);
}

//fifth menubutton clicked
void MainWindow::clickedFive()
{
    //Delete right side and set color of button
    clearLayout(contentLayout);
    setColors(4);

    study1Label = new QLabel();
    study1Label->setText("Program információk");
    study1Label->setAlignment(Qt::AlignTop);
    study1Label->setStyleSheet("font: 15pt");

    welcomeText = new QLabel();
    welcomeText->setText("Az alkalmazás bioinformatikai számításoknál hasznosítható. Segítségével meghatározhatjuk a "
                          "legvalószínűbb öröklési mintát, az általunk választott algoritmussal. Előre elkészített tesztelési "
                          "eredményeket nézhetünk meg, és új teszteket futtathatunk.\n\nBaloldali menüpontok:");
    welcomeText->setWordWrap(true);

    studyList = new QListWidget();
    studyList->addItem("1: Rövid magyarázat a feladattal kapcsolatban és néhány példa");
    studyList->addItem("2: Algoritmusok kipróbálása és kirajzolása egyszerű esetben");
    studyList->addItem("3: Tesztelési eredmények értelmezése és letöltése");
    studyList->addItem("4: Teszt futtatása, tetszőleges bemenetre és algoritmusokra");
    studyList->addItem("5: Az itt látható súgó");
    studyList->setStyleSheet("background: #FFFFB7; font: italic; border: 2px solid black");

    study4Label = new QLabel();
    study4Label->setText("A program szakdolgozat részeként készült. Az alkalmazás használható felhasználói "
                         "felület nélkül is. A felhasznált források, támogatások és a részletes használati  "
                         "útmutató a dokumentációban érhető el.");
    study4Label->setWordWrap(true);
    study2Label = new QLabel();
    study2Label->setText("Készítő: Temesi András");
    study2Label->setStyleSheet("font: 12pt");
    study3Label = new QLabel();
    study3Label->setText("\nTémavezető: Szabó László Ferenc");
    study3Label->setStyleSheet("font: 12pt");

    studyList->setFixedHeight(100);


    contentLayout->addWidget(study1Label);
    contentLayout->addWidget(welcomeText);
    contentLayout->addWidget(studyList);
    contentLayout->addWidget(study4Label);
    contentLayout->addWidget(study3Label);
    contentLayout->addWidget(study2Label);
    contentLayout->addStretch(1);

}

QGroupBox* MainWindow::pickType()
{
    //First quarter of form
    groupBox[0] = new QGroupBox(tr("Típus"));
    radio[0] = new QRadioButton(tr("Egyszerű"));
    radio[0]->setChecked(true);

    vbox[0] = new QVBoxLayout();
    vbox[0]->addWidget(radio[0]);

    //Checkbox for general case
    generalBox = new QGroupBox(tr("Általános"));
    generalBox->setCheckable(true);
    generalBox->setChecked(false);
    connect(generalBox, SIGNAL(clicked()), this, SLOT(displayClicked()));

    vbox[1] = new QVBoxLayout();

    for(int i=1; i<4; ++i){
        radio[i] = new QRadioButton();
        vbox[1]->addWidget(radio[i]);
    }

    //General case additional options
    radio[1]->setText(tr("Alapértelmezett függvény"));
    radio[2]->setText(tr("Függvény #2"));
    radio[3]->setText(tr("Függvény #3"));

    vbox[1]->addStretch(1);
    generalBox->setLayout(vbox[1]);

    vbox[0]->addWidget(generalBox);
    vbox[0]->addStretch(1);
    groupBox[0]->setLayout(vbox[0]);

    return groupBox[0];
}

void MainWindow::displayClicked()
{
    //If general case
    //Set two algorithmboxes unavailable
    radio[0]->setEnabled(!generalBox->isChecked());
    checkBox[3]->setEnabled(!generalBox->isChecked());
    checkBox[2]->setEnabled(!generalBox->isChecked());
    if(!checkBox[3]->isEnabled())
    {
        checkBox[3]->setCheckState(Qt::CheckState::Unchecked);
        checkBox[2]->setCheckState(Qt::CheckState::Unchecked);
    }
    radio[1]->setChecked(generalBox->isChecked());
}


QGroupBox* MainWindow::pickAlgo()
{
    //Second quarter of form
    groupBox[1] = new QGroupBox(tr("Algoritmus"));

    vbox[2] = new QVBoxLayout;

    //Add buttons to the new layout
    for(int i=0; i<4; ++i)
    {
        checkBox[i] = new QCheckBox();
        vbox[2]->addWidget(checkBox[i]);
    }
    checkBox[0]->setText(tr("Precheck+Rekurzív"));
    checkBox[1]->setText(tr("Precheck"));
    checkBox[2]->setText(tr("Naív"));
    checkBox[3]->setText(tr("Mohó"));

    vbox[2]->addStretch(1);
    groupBox[1]->setLayout(vbox[2]);

    return groupBox[1];
}

QGroupBox* MainWindow::pickFile()
{
    //Third quarter of form
    groupBox[3] = new QGroupBox(tr("Fájlok"));
    vbox[4] = new QVBoxLayout();

    //Add two new buttons
    getInFile = new QPushButton();
    getInFile->setText("Válasszon bemeneti fájlt!");
    connect(getInFile, SIGNAL(clicked()), this, SLOT(getFilePath()));
    getOutFile = new QPushButton();
    getOutFile->setText("Adja meg a kimeneti fájl nevét!");
    connect(getOutFile, SIGNAL(clicked()), this, SLOT(FileOutName()));

    vbox[4]->addWidget(getInFile);
    vbox[4]->addWidget(getOutFile);

    groupBox[3]->setLayout(vbox[4]);

    return groupBox[3];
}

QGroupBox* MainWindow::pickTarget()
{
    //Fourth quarter of form
    groupBox[2] = new QGroupBox(tr("Adatok"));

    //Add buttons to the new layout
    vbox[3] = new QVBoxLayout;
    for(int i=4; i<7; ++i)
    {
        checkBox[i] = new QCheckBox();
        vbox[3]->addWidget(checkBox[i]);
    }

    checkBox[4]->setText(tr("Függvényérték"));
    checkBox[5]->setText(tr("Szülő vektor"));
    checkBox[6]->setText(tr("Futási idő"));

    vbox[3]->addStretch(1);
    groupBox[2]->setLayout(vbox[3]);

    return groupBox[2];
}

void MainWindow::exampleGraph()
{
    //If graph already present delete it
    if(myDraw != nullptr)
    {
        delete myDraw;
        myDraw=nullptr;
    }
    for(int i=0; i<2; ++i)
    {
        for(int j=0; j<3; ++j)
        {
            optionButtons[i][j]->setStyleSheet("");
        }
    }
    //Call drawGraph according to the button clicked
    //Set color of clicked button red
    QPushButton* buttonSender = qobject_cast<QPushButton*>(sender());
    if(buttonSender==optionButtons[0][0])
    {
        optionButtons[0][0]->setStyleSheet("background-color: #E55451");
        myDraw = new DrawWidget();
        contentLayout->addWidget(myDraw);
        std::vector<int> nums({8, 4, 2, 3, 1});
        std::vector<int> pars({0, 3, 4, 0});

        myDraw->drawGraph(nums, pars, "Visszafele menő él");
    }
    if(buttonSender==optionButtons[0][1])
    {
        optionButtons[0][1]->setStyleSheet("background-color: #E55451");
        myDraw = new DrawWidget();
        contentLayout->addWidget(myDraw);
        std::vector<int> nums({11, 5, 4, 1, 2});
        std::vector<int> pars({0, 0, -1, 2});

        myDraw->drawGraph(nums, pars, "Túl kevés él, nem fa");
    }
    if(buttonSender==optionButtons[0][2])
    {
        optionButtons[0][2]->setStyleSheet("background-color: #E55451");
        myDraw = new DrawWidget();
        contentLayout->addWidget(myDraw);
        std::vector<int> nums({9, 7, 2, 3, 5, 4});
        std::vector<int> pars({0, 1, 1, 0, 4});

        myDraw->drawGraph(nums, pars, "9-7-5 értéke negatív (-3)");
    }
    if(buttonSender==optionButtons[1][0])
    {
        optionButtons[1][0]->setStyleSheet("background-color: #E55451");
        myDraw = new DrawWidget();
        contentLayout->addWidget(myDraw);
        std::vector<int> nums({15, 3, 7, 2});
        std::vector<int> pars({0, 0, 2});

        myDraw->drawGraph(nums, pars);
    }
    if(buttonSender==optionButtons[1][1])
    {
        optionButtons[1][1]->setStyleSheet("background-color: #E55451");
        myDraw = new DrawWidget();
        contentLayout->addWidget(myDraw);
        std::vector<int> nums({23, 5, 12, 3, 6});
        std::vector<int> pars({0, 0, 1, 2});

        myDraw->drawGraph(nums, pars);
    }
    if(buttonSender==optionButtons[1][2])
    {
        optionButtons[1][2]->setStyleSheet("background-color: #E55451");
        myDraw = new DrawWidget();
        contentLayout->addWidget(myDraw);
        std::vector<int> nums({14, 4, 2, 6, 4, 1});
        std::vector<int> pars({0, 0, 0, 3, 3});

        myDraw->drawGraph(nums, pars);
    }

}

void MainWindow::clickedAlgo()
{
    QInputDialog inp;
    inp.setLabelText(tr("Szóközzel elválasztott egészek:"));
    inp.setWindowTitle(tr("Számsorozat:"));
    inp.setTextEchoMode(QLineEdit::Normal);
    inp.adjustSize();
    inp.setModal(true);
    inp.show();
    inp.move(geometry().bottomRight()-QPoint(inp.geometry().width(), inp.geometry().height()));
    if(inp.exec() == QDialog::Accepted and inp.textValue()!="")
    {
        //Delete former drawing if present
        if(myDraw != nullptr)
        {
            delete myDraw;
            myDraw=nullptr;
        }
        QString text = inp.textValue();

        //Make argument list from input
        text.replace("\n", " ");
        QStringList stringList = text.split(" ",QString::SkipEmptyParts);

        //Determine sender button
        QString algParam;
        QPushButton* buttonSender = qobject_cast<QPushButton*>(sender());
        if(buttonSender==algoButtons[0])
        {
            algParam="1";
        }
        else if(buttonSender==algoButtons[1])
        {
            algParam="2";
        }
        else if(buttonSender==algoButtons[2])
        {
            algParam="3";
        }
        else if(buttonSender==algoButtons[3])
        {
            algParam="4";
        }
        else if(buttonSender==algoButtons[4])
        {
            algParam="5";
        }
        //Build the command and start process, backend file is in build directory
        QString path = QCoreApplication::applicationDirPath();
        QString  command("python");
        QStringList params = QStringList() << "algoritmusok.py" << algParam;
        params.append(stringList);

        QProcess *process = new QProcess();
        process->start(command, params);

        //Wait for max 5 seconds
        if(!process->waitForFinished(5000)){
            QMessageBox msgBox;
            msgBox.setWindowTitle(tr("Időtúllépés"));
            msgBox.setText(tr("Nem futott le 5mp alatt. Használd a haladó funkciókat!"));
            msgBox.show();
            msgBox.setModal(true);
            msgBox.move(geometry().bottomRight()-QPoint(msgBox.geometry().width(), msgBox.geometry().height()));
            msgBox.exec();
            process->close();
            return;
        }
        process->close();

        //Fill nums vector from input object
        std::vector<int> nums;
        for(int i=0; i<stringList.size(); ++i)
        {
            nums.push_back(stringList[i].toInt());
        }
        //Open file with the output
        QFile file(QCoreApplication::applicationDirPath() + "/output.txt");

        if(!file.open(QIODevice::ReadOnly))
        {
            QMessageBox::information(nullptr, "Internal error", file.errorString());
        }
        QStringList stringList2;
        QTextStream in(&file);

        //If only true or false print that and return
        if(algParam=="5")
        {
            QString line = in.readLine();
            if(line=="True")
            {
                myDraw = new DrawWidget();
                myDraw->welcomeText("Megoldható");
                contentLayout->addWidget(myDraw);
            }
            else if(line=="False")
            {
                myDraw = new DrawWidget();
                myDraw->welcomeText("Nem megoldható");
                contentLayout->addWidget(myDraw);
            } else
            {
                QMessageBox msgBox;
                msgBox.setWindowTitle(tr("Bemenet hiba"));
                msgBox.setText(tr("Csak pozitív egész számokat adjon meg!"));
                msgBox.show();
                msgBox.setModal(true);
                msgBox.move(geometry().bottomRight()-QPoint(msgBox.geometry().width(), msgBox.geometry().height()));
                msgBox.exec();
            }
            file.close();
            return;
        }
        //If other algorithm read in the results
        QString line = in.readLine();
        stringList2= line.split(" ",QString::SkipEmptyParts);

        //-1 means no solution
        if(stringList2[stringList2.size()-1].toInt()==-1)
        {
            QMessageBox msgBox;
            msgBox.setWindowTitle(tr("Nincs eredmény"));
            msgBox.setText(tr("Az algoritmus szerint nem megoldható."));
            msgBox.show();
            msgBox.setModal(true);
            msgBox.move(geometry().bottomRight()-QPoint(msgBox.geometry().width(), msgBox.geometry().height()));
            msgBox.exec();
            file.close();
            return;
        } //-2 means invalid input
        else if(stringList2[stringList2.size()-1].toInt()==-2)
        {
            QMessageBox msgBox;
            msgBox.setWindowTitle(tr("Bemenet"));
            msgBox.setText(tr("Csak pozitív egész számokat adjon meg!"));
            msgBox.show();
            msgBox.setModal(true);
            msgBox.move(geometry().bottomRight()-QPoint(msgBox.geometry().width(), msgBox.geometry().height()));
            msgBox.exec();
            file.close();
            return;
        }
        else
        {
            stringList2.pop_back();
            std::vector<int> pars;
            //Fill pars vector
            for(int i=0; i<stringList2.size(); ++i)
            {
                pars.push_back(stringList2[i].toInt());
            }

            //create myDraw and paint
            myDraw = new DrawWidget();
            myDraw->setStyleSheet("background-color: rgb(130, 130, 130)");
            myDraw->drawGraph(nums, pars);
            contentLayout->addWidget(myDraw);

            file.close();
        }
    }
}

//Open file dialog window to choose inputfile
void MainWindow::getFilePath()
{
    QString file1Name = QFileDialog::getOpenFileName(nullptr,
             tr("Open SIMS File 1"), QCoreApplication::applicationDirPath(), tr("SIMS Files (*.sims)"));
    if (!file1Name.isEmpty())
    {
        inPath=file1Name;
        getInFile->setStyleSheet("background: green");
    }
}

//Open input dialog window to type filename
//Set color green if successfull
void MainWindow::FileOutName()
{
    QInputDialog inp;
    inp.setLabelText(tr("Kimeneti fájl neve:"));
    inp.setWindowTitle(tr("Fájlnév:"));
    inp.setTextEchoMode(QLineEdit::Normal);
    inp.adjustSize();
    inp.setModal(true);
    inp.show();
    inp.move(geometry().bottomRight()-QPoint(inp.geometry().width(), inp.geometry().height()));
    if(inp.exec() == QDialog::Accepted and inp.textValue()!="")
    {

        outFile = inp.textValue();
        getOutFile->setStyleSheet("background: green");
    }
}

//Start the detached process
void MainWindow::runPython()
{
    //0 means advanced
    QStringList stringList = QStringList() << "0";
    //Get type
    if(generalBox->isChecked())
    {
        if(radio[1]->isChecked()){
            stringList.append("1");
        }
        else if(radio[2]->isChecked()){
            stringList.append("2");
        }
        else{
            stringList.append("3");
        }
    } else{
        stringList.append("0");
    }
    //Check input and output file
    if(inPath==""){
        QMessageBox msgBox;
        msgBox.setText("Válasszon bemeneti fájlt!");
        msgBox.setWindowTitle("Hiányos");
        msgBox.show();
        msgBox.setModal(true);
        msgBox.move(geometry().bottomRight()-QPoint(msgBox.geometry().width(), msgBox.geometry().height()));
        msgBox.exec();
        return;
    }
    if(outFile==""){
        QMessageBox msgBox;
        msgBox.setText("Adja meg a fájlnevet!");
        msgBox.setWindowTitle("Hiányos");
        msgBox.show();
        msgBox.setModal(true);
        msgBox.move(geometry().bottomRight()-QPoint(msgBox.geometry().width(), msgBox.geometry().height()));
        msgBox.exec();
        return;
    }
    //check output filename for special characters
    if (outFile.toStdString().find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890_") != std::string::npos)
    {
        QMessageBox msgBox;
        msgBox.setText("A kimeneti fájlnév nem tartalmazhat speciális karaktereket!");
        msgBox.setWindowTitle("Hiányos");
        msgBox.show();
        msgBox.setModal(true);
        msgBox.move(geometry().bottomRight()-QPoint(msgBox.geometry().width(), msgBox.geometry().height()));
        msgBox.exec();
        return;
    }
    bool algoSelected=false;
    bool targetSelected=false;
    for(int i=0; i<7; ++i)
    {
        if(i<4 && checkBox[i]->isChecked()){
            algoSelected=true;
        }
        if(i>=4 && checkBox[i]->isChecked()){
            targetSelected=true;
        }
    }
    //Check if at least one algorithm selected
    if(!algoSelected){
        QMessageBox msgBox;
        msgBox.setText("Válasszon legalább egy algoritmust!");
        msgBox.setWindowTitle("Hiányos");
        msgBox.show();
        msgBox.setModal(true);
        msgBox.move(geometry().bottomRight()-QPoint(msgBox.geometry().width(), msgBox.geometry().height()));
        msgBox.exec();
        return;
    }
    //Check if at least one target selected
    if(!targetSelected){
        QMessageBox msgBox;
        msgBox.setText("Válasszon legalább egy kimeneti értéket!");
        msgBox.setWindowTitle("Hiányos");
        msgBox.show();
        msgBox.setModal(true);
        msgBox.move(geometry().bottomRight()-QPoint(msgBox.geometry().width(), msgBox.geometry().height()));
        msgBox.exec();
        return;
    }
    //Create argument list
    for(int i=0; i<7; ++i)
    {
        if(checkBox[i]->isChecked()){
            stringList.append("1");
        } else{
            stringList.append("0");
        }
    }
    stringList.append(inPath);
    stringList.append(outFile);

    //Start the process in build directory
    QString path = QCoreApplication::applicationDirPath();
    QString  command("python");
    QStringList params = QStringList() << "algoritmusok.py";
    params.append(stringList);

    qint64 *processID = nullptr;
    QProcess *process = new QProcess();
    bool started = process->startDetached(command, params, path, processID);
    if(started){
        QMessageBox msgBox;
        msgBox.setText("Fut a program! Parancssorból ellenőrízhető.");
        msgBox.setWindowTitle("Elindult");
        msgBox.show();
        msgBox.setModal(true);
        msgBox.move(geometry().bottomRight()-QPoint(msgBox.geometry().width(), msgBox.geometry().height()));
        msgBox.exec();
    }
}

//Download testresults from the internet
void MainWindow::downloadFile()
{
    QString ExcelFileName="https://people.inf.elte.hu/b4zppx/data";
    QPushButton* buttonSender = qobject_cast<QPushButton*>(sender());
    if(buttonSender==testButtons[0]){
        ExcelFileName+="/results1.xls";
    } else if(buttonSender==testButtons[1]){
        ExcelFileName+="/results2.xls";
    } else if(buttonSender==testButtons[2]){
        ExcelFileName+="/results5.xls";
    } else {
        ExcelFileName+="/results7.xls";
    }
    QDesktopServices::openUrl(QUrl(ExcelFileName));
}

//Paint the current menubutton red
void MainWindow::setColors(int index)
{
    for(int i=0; i<5; ++i)
    {
        buttonVector[i]->setStyleSheet("background-color: #FFFFB7; color: black; border: 4px solid black; font: bold");
    }
    buttonVector[index]->setStyleSheet("background-color: #E55451; color: black; border: 4px solid black; font: bold");
}

MainWindow::~MainWindow()
{

}
