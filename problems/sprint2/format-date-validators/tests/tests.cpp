#include <QtTest>

#include <QCoreApplication>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>
#include <QtWidgets/QLabel>
#include <QIntValidator>

#include <mainwindow.h>

#include "test_utils/practicum_assert.hpp"

int getDayNumber(int day, int month, int year)
{

    auto DaysPerMonth = [&](int month, int year)
    {
        if (month == 2) {
            if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0) {
                return 29;
            } else {
                return 28;
            }
        } else if (month == 4 || month == 6 || month == 9 || month == 11) {
            return 30;
        } else {
            return 31;
        }
    };

    if(month == 0) {
        return 0;
    }
    int num_days = 0;
    for(size_t num_month = 1; num_month < static_cast<size_t>(month); ++num_month) {
        num_days += DaysPerMonth(num_month, year);
    }
    num_days += day;
    return num_days;
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

    void TestDateMask();
    void TestValidators();
    void TestCheckValidity();
    void TestCheckValidityOther();

private:
    MainWindow* window;

    QLineEdit* le_date;
    QLineEdit* le_day;
    QLineEdit* le_month;
    QLineEdit* le_year;
};


template<typename T>
void getChild(T*& dst, QObject* parent, const QString& object_name, const QString& type_name)
{

    dst = parent->findChild<T *>(object_name);
    QString message;
    if (type_name != "")
    {
        message = QString("В %3 не найден %1 типа %2").arg(object_name).arg(type_name).arg(parent->objectName());
    }
    else
    {
        message = QString("В %2 не найден %1").arg(object_name).arg(parent->objectName());;
    }

    QVERIFY2(dst, qPrintable(message));
}

template<typename T>
T* getChild(QObject* parent, const QString& object_name, const QString& type_name = {})
{
    T* dst = nullptr;
    getChild<T>(dst, parent, object_name, type_name);

    return dst;

}

void TestYourApp::initTestCase()
{

}

void TestYourApp::init()
{
    window = new MainWindow();
    QVERIFY2(window != nullptr, "Окно приложения не создано");
    window->show();
    QVERIFY2(window->isVisible(), "Окно приложения не активируется");

    le_date = getChild<QLineEdit>(window, "le_date", "QLineEdit");

    le_day = getChild<QLineEdit>(window, "le_day", "QLineEdit");
    le_month = getChild<QLineEdit>(window, "le_month", "QLineEdit");
    le_year = getChild<QLineEdit>(window, "le_year", "QLineEdit");
}

void TestYourApp::TestDateMask() {
    PRAC_COMPARE2(le_date->inputMask(), QString("00.00.0000"), "Неправильно установлена маска для поля ввода даты");
}

void TestYourApp::TestValidators()
{

    auto checkValidity = [this] (int day, int month, int year){
        auto day_str = QString("%1").arg(day, 2, 10, QChar('0'));
        auto month_str = QString("%1").arg(month, 2, 10, QChar('0'));
        auto year_str = QString("%1").arg(year);

        le_day->setText(day_str);;
        le_month->setText(month_str);
        le_year->setText(year_str);

        if ((day >= 1) && (day <= 31)) {
            QVERIFY2(le_day->hasAcceptableInput(), "Введенный день определен как не валидный, но должен быть таковым");
        } else {
            QVERIFY2(!(le_day->hasAcceptableInput()), "Введенный день определен как валидный, но не должен быть таковым");
        }

        if ((month >= 1) && (month <= 12)) {
            QVERIFY2(le_month->hasAcceptableInput(), "Введенный месяц определен как не валидный, но должен быть таковым");
        } else {
            QVERIFY2(!(le_month->hasAcceptableInput()), "Введенный месяц определен как валидный, но не должен быть таковым");
        }

        if ((year >= 1) && (year <= 9999)) {
            QVERIFY2(le_year->hasAcceptableInput(), "Введенный год определен как не валидный, но должен быть таковым");
        } else {
            QVERIFY2(!(le_year->hasAcceptableInput()), "Введенный год определен как валидный, но не должен быть таковым");
        }
    };

    checkValidity(22, 4, 2015);
    checkValidity(12, 5, 9999);
    checkValidity(5, 12, 1978);
    checkValidity(31, 7, 8727);
    checkValidity(2, 6, 115);
    checkValidity(32, 4, 2015);
    checkValidity(19, 13, 2015);
    checkValidity(2, 3, 10000);
    checkValidity(123, 3, 73315);
    checkValidity(99, 57, 2235);
    checkValidity(13, 39, 30015);
    checkValidity(222, 444, 12015);

}

void TestYourApp::TestCheckValidity()
{

    le_day->setText("32");
    window->CheckValidity(le_day);
    QCOMPARE(le_day->styleSheet(), QString("border:1px solid red"));

    le_month->setText("13");
    window->CheckValidity(le_month);
    QCOMPARE(le_day->styleSheet(), QString("border:1px solid red"));

    le_year->setText("10000");
    window->CheckValidity(le_year);
    QCOMPARE(le_day->styleSheet(), QString("border:1px solid red"));

    le_day->setText("12");
    window->CheckValidity(le_day);
    QCOMPARE(le_day->styleSheet(), QString("border:1px solid transparent"));

    le_month->setText("11");
    window->CheckValidity(le_month);
    QCOMPARE(le_day->styleSheet(), QString("border:1px solid transparent"));

    le_year->setText("1897");
    window->CheckValidity(le_year);
    QCOMPARE(le_day->styleSheet(), QString("border:1px solid transparent"));
}

void TestYourApp::TestCheckValidityOther()
{
    QLineEdit test;
    test.setValidator(new QIntValidator(22, 77));

    test.setText(QString("77"));
    window->CheckValidity(&test);
    QCOMPARE(test.styleSheet(), QString("border:1px solid transparent"));

    test.setText(QString("97"));
    window->CheckValidity(&test);
    QCOMPARE(test.styleSheet(), QString("border:1px solid red"));

    test.setText(QString("23"));
    window->CheckValidity(&test);
    QCOMPARE(test.styleSheet(), QString("border:1px solid transparent"));
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
