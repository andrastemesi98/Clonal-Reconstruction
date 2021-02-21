#include <QtTest>
#include <QCoreApplication>

#include <QLineEdit>

#include "/home/andras/Desktop/thesis/thesisfront/mainwindow.h"

class unittest : public QObject
{
    Q_OBJECT

public:
    unittest();
    ~unittest();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void test_case1();
    void mainWindowSize();
    void mainWindowFirst();
    void mainWindowSecond();
    void mainWindowThird();
    void mainWindowFourt();
    void mainWindowFifth();
    void simulateUse();
    void testDrawWidget();

};

unittest::unittest()
{

}

unittest::~unittest()
{

}

//Check if system is in correct state
void unittest::initTestCase()
{
    qDebug("Called before everything else.");
}

//Check if finished correctly
void unittest::cleanupTestCase()
{
    qDebug("Called after everything else.");
}

//test of test
void unittest::test_case1()
{
    QLineEdit lineEdit;
    QTest::keyClicks(&lineEdit, "hello world");
    QCOMPARE(lineEdit.text(), QString("hello world"));
}

//check if initial sizing is correct
void unittest::mainWindowSize()
{
    MainWindow myWindow;
    QCOMPARE(myWindow.minimumWidth(), 600);
    QCOMPARE(myWindow.minimumHeight(), 400);
    QVERIFY(!myWindow.isFullScreen());
}

//click on first menubutton check if objects exist
void unittest::mainWindowFirst()
{
    MainWindow myWindow;
    QTest::mouseClick(myWindow.buttonVector[0], Qt::MouseButton::LeftButton);
    QVERIFY(myWindow.studyList);
    QCOMPARE(myWindow.optionButtons.size(), 2);
    QCOMPARE(myWindow.optionButtons[0].size(), 3);
}

//click on second menubutton check if objects exist
void unittest::mainWindowSecond()
{
    MainWindow myWindow;
    QTest::mouseClick(myWindow.buttonVector[1], Qt::MouseButton::LeftButton);
    QVERIFY(myWindow.study1Label);
    QCOMPARE(myWindow.algoButtons.size(), 5);
    QVERIFY(!myWindow.myDraw); //should not
}

//click on third menubutton check if objects exist
void unittest::mainWindowThird()
{
    MainWindow myWindow;
    QTest::mouseClick(myWindow.buttonVector[2], Qt::MouseButton::RightButton);
    QCOMPARE(myWindow.testButtons.size(), 0); //wrong click
    QTest::mouseClick(myWindow.buttonVector[2], Qt::MouseButton::LeftButton);
    QCOMPARE(myWindow.testButtons.size(), 4);
    QCOMPARE(myWindow.study3Label->text(), "Általános eset: 100 darab szimulált 10 klónos minta esetén:");
}

//click on fourth menubutton check if objects exist
void unittest::mainWindowFourt()
{
    MainWindow myWindow;
    QTest::mouseClick(myWindow.buttonVector[3], Qt::MouseButton::LeftButton);
    QCOMPARE(myWindow.testButtons.size(), 0); //other menubutton click
    QCOMPARE(myWindow.groupBox.size(), 4);
    QCOMPARE(myWindow.checkBox.size(), 7);
}

//click on fifth menubutton check if objects exist
void unittest::mainWindowFifth()
{
    MainWindow myWindow;
    QTest::mouseClick(myWindow.buttonVector[4], Qt::MouseButton::LeftButton);
    QVERIFY(myWindow.study1Label);
    QCOMPARE(myWindow.studyList->height(), 100);
}

//click many things
void unittest::simulateUse()
{
    MainWindow myWindow;
    QTest::mouseClick(myWindow.buttonVector[0], Qt::MouseButton::LeftButton);
    QTest::mouseClick(myWindow.buttonVector[1], Qt::MouseButton::LeftButton);
    QTest::mouseClick(myWindow.buttonVector[2], Qt::MouseButton::LeftButton);
    QTest::mouseClick(myWindow.buttonVector[0], Qt::MouseButton::LeftButton);
    QTest::mouseClick(myWindow.optionButtons[0][0], Qt::MouseButton::LeftButton);
    QVERIFY(myWindow.myDraw);
    QTest::mouseClick(myWindow.buttonVector[4], Qt::MouseButton::LeftButton);
    QVERIFY(!myWindow.myDraw);
}

//test drawWidget
void unittest::testDrawWidget()
{
    DrawWidget myDraw;
    std::vector<int> nums({8, 4, 2, 3, 1});
    std::vector<int> pars({0, 0, 1, 2});
    myDraw.calculations(nums, pars);
    QCOMPARE(myDraw.solution->toPlainText(), "8 * 4 * 4 * 2 = 256");
    std::vector<int> nums2({14, 2, 5, 4, 1, 3, 4});
    std::vector<int> pars2({0, 0, 2, 1, 3, 0});
    myDraw.calculations(nums2, pars2);
    QCOMPARE(myDraw.solution->toPlainText(), "14 * 12 * 5 * 2 * 4 * 7 = 47040");
    std::vector<int> nums3({10, 9, 8, 7, 6, 5, 4, 1, 1, 1});
    std::vector<int> pars3({0, 1, 2, 3, 4, 5, 6, 6, 6});
    myDraw.calculations(nums3, pars3);
    QCOMPARE(myDraw.solution->toPlainText(), "10 * 9 * 8 * 7 * 6 * 5 * 4 * 3 * 2 = 3628800");
}


QTEST_MAIN(unittest)

#include "tst_unittest.moc"
