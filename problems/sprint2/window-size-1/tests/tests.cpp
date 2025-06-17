#include <QtTest>
#include <QCoreApplication>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>
#include <QtWidgets/QLabel>

#include <mainwindow.h>
#include <iostream>

#include "test_utils/practicum_assert.hpp"

using namespace std;

namespace {

template<class Widget>
Widget* FindElementByName(QMainWindow* window, const QString& name) {
    auto elements = window->findChildren<Widget*>();

    auto iter = std::find_if(elements.begin(), elements.end(), [&](Widget* elem){
        return elem->objectName() == name;
    });

    return iter == elements.end() ? nullptr : *iter;
}

}


class TestYourApp : public QObject
{
    Q_OBJECT

    template<class T>
    T* findElementByText(const QString& label) {
        for (T* button : window->findChildren<T*>())
        {
            if (button->text() == label)
            {
                return button;
            }
        }
        return nullptr;
    }

public:
    TestYourApp()=default;
    ~TestYourApp()=default;

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();
    void TestDefault();
    void TestResize1();
    void TestResize2();


private:
    MainWindow* window;
    QLabel* lbl_size;
};

void TestYourApp::initTestCase()
{

}

void TestYourApp::init()
{
    window = new MainWindow();
    QVERIFY2(window != nullptr, "Окно приложения не создано");
    window->show();
    QVERIFY2(window->isVisible(), "Окно приложения не активируется");
    lbl_size = FindElementByName<QLabel>(window, "lbl_size");
    QVERIFY2(lbl_size, "Не найден элемент lbl_size класса QLabel");
}


void check_lbl_alignment(QMainWindow* parent, QLabel* child)
{

    PRAC_COMPARE2(child->size(), parent->size(), "Размер виджета не равен размеру окна");
    PRAC_COMPARE2(child->pos(), QPoint(0, 0), "Размер виджета не равен размеру окна");

    auto alignment = child->property("alignment");
    QVERIFY2(alignment.isValid(), "Выравнивание текста в lbl_size должно быть задано полем alignement");
    auto value = alignment.value<Qt::AlignmentFlag>();
    PRAC_COMPARE2(value, Qt::AlignCenter, "Выравннивание текста в lbl_size должно быть по центру");
}

void TestYourApp::TestDefault()
{
    check_lbl_alignment(window, lbl_size);
}


void TestYourApp::TestResize1()
{
    window->resize(window->size() * 0.57);
    check_lbl_alignment(window, lbl_size);
}

void TestYourApp::TestResize2()
{
    window->resize(window->size() * 1.65);
    check_lbl_alignment(window, lbl_size);
}


void TestYourApp::cleanupTestCase()
{
}

void TestYourApp::cleanup()
{
    delete window;
}

QTEST_MAIN(TestYourApp)

#include "tests.moc"
