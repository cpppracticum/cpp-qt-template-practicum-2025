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

#include <mainwindow.h>

#include "test_utils/practicum_assert.hpp"

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
    void TestCloseButton();

private:
    MainWindow* window;
    QPushButton *yourButton;
    QLineEdit *textView;
};


void TestYourApp::initTestCase()
{


    window = new MainWindow();

    QVERIFY2(window != nullptr, "Окно приложения не создано");
    window->show();
    QVERIFY2(window->isVisible(), "Окно приложения не активируется");


    auto buttons = window->findChildren<QPushButton*>();
    PRAC_COMPARE2(buttons.size(), 1, "В приложении должна быть ровно одна копка QPushButton");
    yourButton = buttons.first();

}

void TestYourApp::TestCloseButton()
{

    QTest::mouseClick(yourButton, Qt::LeftButton);
    QVERIFY2(!window->isVisible(), "Окно приложения не было закрыто после нажатия на кнопку");

}

void TestYourApp::cleanupTestCase()
{
}

QTEST_MAIN(TestYourApp)

#include "tests.moc"
