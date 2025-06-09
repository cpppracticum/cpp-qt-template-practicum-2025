#include <QtTest>

#include <QDebug>
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

#include <mainwindow.h>

#include "utils.h"

#include "test_utils/practicum_assert.hpp"

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
    TestYourApp() = default;
    ~TestYourApp() = default;

private slots:
    void initTestCase();
    void cleanupTestCase();
    void TestSort();

private:
    MainWindow* window;
    QLineEdit *le_sorted, *le_unsorted;
};

void TestYourApp::initTestCase()
{


    window = new MainWindow();

    QVERIFY2(window != nullptr, "Окно приложения не создано");

    window->show();

    QVERIFY2(window->isVisible(), "Окно приложения не активируется");

    le_sorted = getChild<QLineEdit>(window, "le_sorted", "QLineEdit");
    le_unsorted = getChild<QLineEdit>(window, "le_unsorted", "QLineEdit");
}

void TestYourApp::TestSort()
{
    le_unsorted->setText("b c a");
    PRAC_COMPARE2(le_sorted->text().trimmed(), "a b c", "Неверный результат для строки 'b c a'");

    le_unsorted->setText("All good things");
    PRAC_COMPARE2(le_sorted->text().trimmed(), "All good things", "Неверный результат для строки 'All good things'");

    le_unsorted->setText("No one lives forever");
    PRAC_COMPARE2(le_sorted->text().trimmed(), "forever lives No one", "Неверный результат для строки 'No one lives forever'");
}

void TestYourApp::cleanupTestCase()
{
}

QTEST_MAIN(TestYourApp)

#include "tests.moc"
