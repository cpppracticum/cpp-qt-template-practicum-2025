
// Core headers
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QWidget>

#include <QtWidgets/QPushButton>
#include <QtWidgets/QComboBox>
#include <QtTest>

#include "prac/QInputDialog"

#include <vector>

#include "mainwindow.h"

#include "utils.h"

#include "test_utils/practicum_assert.hpp"

extern QString custom_url;
extern QString custom_dir;

class TestYourApp : public QObject
{
    Q_OBJECT
public:
    TestYourApp()=default;
    ~TestYourApp()=default;

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    void TestCustomUrl();
    void TestCustomDir();

    void getAction(int, QString, std::function<void()> new_action)
    {
        action = new_action;
    }

private:

    Setting* setting;
    QComboBox* cb_action;
    QPushButton* pb_save;
    std::function<void()> action;

};


void TestYourApp::initTestCase()
{

}



void TestYourApp::init()
{
    setting = new Setting();
    QVERIFY2(setting != nullptr, "Окно настроек не создано");

    cb_action = getChild<QComboBox>(setting, "cb_action", "QComboBox");
    pb_save = getChild<QPushButton>(setting, "pb_save", "QPushButton");

    connect(setting, &Setting::signalSetAction, this, &TestYourApp::getAction);

}

void TestYourApp::TestCustomUrl()
{
    QString url = "https://super.duper.custom.url.org";
    prac::QInputDialog::setInput(url);

    cb_action->setCurrentIndex(5);
    clickWidget(pb_save);
    action();

    PRAC_COMPARE2(custom_url, url, "Открытый url не соответствует ожидаемому");
}

void TestYourApp::TestCustomDir()
{
    QString dir = "/home/student/practicum/best_project_ever/";
    prac::QInputDialog::setInput(dir);

    cb_action->setCurrentIndex(6);
    clickWidget(pb_save);
    action();

    PRAC_COMPARE2(custom_dir, dir, "Открытая директория не соответствует ожидаемой");
}

void TestYourApp::cleanupTestCase()
{
}

void TestYourApp::cleanup()
{
    delete setting;
}

QTEST_MAIN(TestYourApp)

#include "tests.moc"
