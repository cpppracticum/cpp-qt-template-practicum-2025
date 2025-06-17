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

// add necessary includes here

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
    void TestInputText_1();
    void TestInputText_2();

private:
    MainWindow* window;
    QLineEdit *textView;
    QLabel *label;
};


void TestYourApp::initTestCase()
{


    window = new MainWindow();

    QVERIFY2(window != nullptr, "Окно приложения не создано");

    window->show();

    QVERIFY2(window->isVisible(), "Окно приложения не активируется");

    auto line_edits = window->findChildren<QLineEdit*>();

    PRAC_COMPARE2(line_edits.size(), 1, "В приложении должно быть ровно одно поле QLineEdit");
    textView = line_edits.first();

    label = findElementByName<QLabel>("lbl_result");
    QVERIFY2(label != nullptr, "Метка с именем lbl_result отсутствует в приложении");
    PRAC_COMPARE2(label->text(), "Введите имя", "Текст по умолчанию в метке lbl_result не совпадает с ожидаемым");

}

void TestYourApp::TestInputText_1()
{

    QString name = "SuperName";
    textView->setText(name);

    QString expected_text = "Привет из Qt, " + name + "!";
    PRAC_COMPARE2(label->text(), expected_text, "Выведенный текст не совпадает с ожидаемым");

    textView->setText("");
    PRAC_COMPARE2(label->text(), "Введите имя", "Выведенный при очищении поля ввода текст не совпадает с ожидаемым");

}

void TestYourApp::TestInputText_2()
{

    QString name = "Another super duper cool name";
    textView->setText(name);

    QString expected_text = "Привет из Qt, " + name + "!";
    PRAC_COMPARE2(label->text(), expected_text, "Выведенный текст не совпадает с ожидаемым");

    textView->setText("");
    PRAC_COMPARE2(label->text(), "Введите имя", "Выведенный при очищении поля ввода текст не совпадает с ожидаемым");

}

void TestYourApp::cleanupTestCase()
{
}

QTEST_MAIN(TestYourApp)

#include "tests.moc"
