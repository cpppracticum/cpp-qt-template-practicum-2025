#include <QtTest>
#include <QCoreApplication>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QLabel>
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

    template<class T>
    T* findElementByName(const QString& name) {
        for (auto label : window->findChildren<T*>())
        {
            if (label->objectName() == name)
            {
                return label;
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
    void TestMelborpButton_1();
    void TestMelborpButton_2();

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


    auto line_edits = window->findChildren<QLineEdit*>();
    PRAC_COMPARE2(line_edits.size(), 1, "В приложении должно быть ровно одно поле QLineEdit");
    textView = line_edits.first();

}

void TestYourApp::TestMelborpButton_1()
{

    QString q_original_text = "This is my awesome string!";
    QString q_reversed_text = "!gnirts emosewa ym si sihT";

    textView->setText(q_original_text);
    PRAC_COMPARE2(textView->text(), q_original_text, "Выведенный текст не совпадает с исходным");

    QTest::mouseClick(yourButton, Qt::LeftButton);

    PRAC_COMPARE2(textView->text(), q_reversed_text, "Выведенный текст не совпадает с перевёрнутым");

    QTest::mouseClick(yourButton, Qt::LeftButton);
    PRAC_COMPARE2(textView->text(), q_original_text, "Повторно перевёрнутый текст не совпадает с исходным");

}

void TestYourApp::TestMelborpButton_2()
{

    QString original_text = "Привет!";
    QString reversed_text = "!тевирП";

    textView->setText(original_text);
    PRAC_COMPARE2(textView->text(), original_text, "Выведенный текст не совпадает с исходным");

    QTest::mouseClick(yourButton, Qt::LeftButton);

    PRAC_COMPARE2(textView->text(), reversed_text, "Выведенный текст не совпадает с перевёрнутым");

    QTest::mouseClick(yourButton, Qt::LeftButton);
    PRAC_COMPARE2(textView->text(), original_text, "Повторно перевёрнутый текст не совпадает с исходным");

}

void TestYourApp::cleanupTestCase()
{
}

QTEST_MAIN(TestYourApp)

#include "tests.moc"
