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

    void TestValidWithDots();
    void TestValidWithoutDots();
    void TestViaParts();
    void TestIncorrectDate();
    void TestNonExistingDate();


private:

    void CheckValidDate(int day, int month, int year, const QString& month_name);
    void CheckInvalidDate();

    MainWindow* window;

    QLineEdit* le_date;
    QLineEdit* le_day;
    QLineEdit* le_month;
    QLineEdit* le_year;

    QLabel* lbl_format_type1;
    QLabel* lbl_format_type2;
    QLabel* lbl_format_type3;
    QLabel* lbl_message;

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

    lbl_format_type1 = getChild<QLabel>(window, "lbl_format_type1", "QLabel");
    lbl_format_type2 = getChild<QLabel>(window, "lbl_format_type2", "QLabel");
    lbl_format_type3 = getChild<QLabel>(window, "lbl_format_type3", "QLabel");
    lbl_message = getChild<QLabel>(window, "lbl_message", "QLabel");
}


void TestYourApp::CheckValidDate(int day, int month, int year, const QString& month_name)
{
    QString date("%1.%2.%3");
    date = date.arg(day, 2, 10, QChar('0')).arg(month, 2, 10, QChar('0')).arg(year);

    PRAC_COMPARE2(le_day->text().toInt(), day, "Выведенный день не совпадает с ожидаемым");
    PRAC_COMPARE2(le_month->text().toInt(), month, "Выведенный месяц не совпадает с ожидаемым");
    PRAC_COMPARE2(le_year->text().toInt(), year, "Выведенный год не совпадает с ожидаемым");

    PRAC_COMPARE2(le_date->text(), date, "Выведенная дата не совпадает с ожидаемой");

    QString format1 = QString("%1.%2.%3").arg(day, 2, 10, QChar('0')).arg(month, 2, 10, QChar('0')).arg(year);
    QString format2 = QString("%2/%1/%3").arg(day, 2, 10, QChar('0')).arg(month, 2, 10, QChar('0')).arg(year);
    QString format3 = QString("%1 %2 %3 года").arg(day, 2, 10, QChar('0')).arg(month_name).arg(year);

    PRAC_COMPARE2(lbl_format_type1->text(), format1, "Выведенный текст в формате 1 не совпадает с ожидаемым");
    PRAC_COMPARE2(lbl_format_type2->text(), format2, "Выведенный текст в формате 2 не совпадает с ожидаемым");
    PRAC_COMPARE2(lbl_format_type3->text(), format3, "Выведенный текст в формате 3 не совпадает с ожидаемым");

    QString message = QString("Это %1 день в %2 году").arg(getDayNumber(day, month, year)).arg(year);
    PRAC_COMPARE2(lbl_message->text(), message, "Выведенное сообщение не совпадает с ожидаемым");

}


void TestYourApp::CheckInvalidDate()
{

    PRAC_COMPARE2(lbl_format_type1->text(), "", "В случае невалидной даты поле lbl_format_type1 должно быть очищено");
    PRAC_COMPARE2(lbl_format_type2->text(), "", "В случае невалидной даты поле lbl_format_type2 должно быть очищено");
    PRAC_COMPARE2(lbl_format_type3->text(), "", "В случае невалидной даты поле lbl_format_type3 должно быть очищено");

}


void TestYourApp::TestValidWithDots()
{

    auto testCase = [this] (int day, int month, int year, const QString& month_name){
        QString date("%1.%2.%3");
        date = date.arg(day, 2, 10, QChar('0')).arg(month, 2, 10, QChar('0')).arg(year);
        le_date->setText(date);
        CheckValidDate(day, month, year, month_name);
    };

    testCase(2, 3, 2015, "марта");
    testCase(12, 5, 2025, "мая");
    testCase(5, 11, 1978, "ноября");
    testCase(30, 7, 2027, "июля");
    testCase(2, 6, 115, "июня");
    testCase(22, 2, 97, "февраля");
    testCase(2, 8, 8764, "августа");
    testCase(25, 12, 876, "декабря");
    testCase(31, 1, 2024, "января");
    testCase(15, 4, 1875, "апреля");
    testCase(3, 9, 1993, "сентября");
    testCase(5, 10, 1966, "октября");
}

void TestYourApp::TestValidWithoutDots()
{

    auto testCase = [this] (int day, int month, int year, const QString& month_name){
        QString date("%1%2%3");
        date = date.arg(day, 2, 10, QChar('0')).arg(month, 2, 10, QChar('0')).arg(year);
        le_date->setText(date);
        CheckValidDate(day, month, year, month_name);
    };

    testCase(2, 3, 2015, "марта");
    testCase(12, 5, 2025, "мая");
    testCase(5, 11, 1978, "ноября");
    testCase(30, 7, 2027, "июля");
    testCase(2, 6, 115, "июня");
    testCase(22, 2, 97, "февраля");
    testCase(2, 8, 8764, "августа");
    testCase(25, 12, 876, "декабря");
    testCase(31, 1, 2024, "января");
    testCase(15, 4, 1875, "апреля");
    testCase(3, 9, 1993, "сентября");
    testCase(5, 10, 1966, "октября");
}


void TestYourApp::TestViaParts()
{

    auto testCase = [this] (int day, int month, int year, const QString& month_name){
        auto day_str = QString("%1").arg(day, 2, 10, QChar('0'));
        auto month_str = QString("%1").arg(month, 2, 10, QChar('0'));
        auto year_str = QString("%1").arg(year);

        le_day->setText("01");
        le_month->setText("01");
        le_year->setText("2000");
        CheckValidDate(1, 1, 2000, "января");

        le_day->setText(day_str);
        CheckValidDate(day, 1, 2000, "января");
        le_month->setText(month_str);
        CheckValidDate(day, month, 2000, month_name);
        le_year->setText(year_str);
        CheckValidDate(day, month, year, month_name);

    };

    testCase(2, 3, 2015, "марта");
    testCase(12, 5, 2025, "мая");
    testCase(5, 11, 1978, "ноября");
    testCase(30, 7, 2027, "июля");
    testCase(2, 6, 115, "июня");
    testCase(22, 2, 97, "февраля");
    testCase(2, 8, 8764, "августа");
    testCase(25, 12, 876, "декабря");
    testCase(31, 1, 2024, "января");
    testCase(15, 4, 1875, "апреля");
    testCase(3, 9, 1993, "сентября");
    testCase(5, 10, 1966, "октября");

}

void TestYourApp::TestIncorrectDate()
{

    auto CheckIncorrectDate = [this] (){
        CheckInvalidDate();
        PRAC_COMPARE2(lbl_message->text(), "Некорректная дата", "В случае невалидной даты в поле lbl_message должна быть надпись \"Некорректная дата\"");
    };
    CheckIncorrectDate();
    le_date->setText("01.0.");
    CheckIncorrectDate();
    le_day->setText("12");
    CheckIncorrectDate();
    le_month->setText("5");
    CheckIncorrectDate();
    le_date->setText("");
    le_year->setText("2022");
    CheckIncorrectDate();
}

void TestYourApp::TestNonExistingDate()
{

    auto CheckNonExistingDate = [this] (){
        CheckInvalidDate();
        PRAC_COMPARE2(lbl_message->text(), "Такой даты не существует", "В случае несуществующей даты в поле lbl_message должна быть надпись \"Такой даты не существует\"");
    };

    le_date->setText("30.02.2020");
    CheckNonExistingDate();
    le_date->setText("31.06.2020");
    CheckNonExistingDate();

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
