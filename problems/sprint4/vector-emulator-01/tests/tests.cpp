
// Core headers
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QWidget>

#include <QtWidgets/QPushButton>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>

#include <QtTest>
#include <iostream>

#include <vector>
#include <string>

#include "mainwindow.h"

#include "utils.h"

#include "test_utils/practicum_assert.hpp"

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

    void TestPushBack();
    void TestPopBack();
    void TestClear();

    void TestDays();
    void TestMonths();

private:
    [[nodiscard]] std::vector<QString> getVector() const;
    void checkExpectedVector(const std::vector<QString>& expectedVector) const;

    void pushBack(const QString& str) const;
    void pushBackVector(const std::vector<QString>& vector) const;
    void popBack() const;
    void clear() const;


    MainWindow *window;

    QListWidget *list_widget;
    QLineEdit *txt_elem_content;
    QLineEdit *txt_size;
    QPushButton *btn_push_back;
    QPushButton *btn_pop_back;
    QPushButton *btn_clear;
    QPushButton *btn_days;
    QPushButton *btn_months;


    static inline const std::vector<QString> days_of_week = {
        "Понедельник",
        "Вторник",
        "Среда",
        "Четверг",
        "Пятница",
        "Суббота",
        "Воскресенье"
    };

    static inline const std::vector<QString> months_of_year = {
        "Январь",
        "Февраль",
        "Март",
        "Апрель",
        "Май",
        "Июнь",
        "Июль",
        "Август",
        "Сентябрь",
        "Октябрь",
        "Ноябрь",
        "Декабрь"
    };
};


void TestYourApp::initTestCase()
{

}



void TestYourApp::init()
{
    window = new MainWindow();

    QVERIFY2(window != nullptr, "Главное окно не создано");
    window->show();
    QVERIFY2(window->isVisible(), "Главное окно не активируется");

    list_widget = getChild<QListWidget>(window, "list_widget", "QListWidget");
    txt_elem_content = getChild<QLineEdit>(window, "txt_elem_content", "QLineEdit");
    txt_size = getChild<QLineEdit>(window, "txt_size", "QLineEdit");
    btn_push_back = getChildByText<QPushButton>(window, "push_back", "QPushButton");
    btn_pop_back = getChildByText<QPushButton>(window, "pop_back", "QPushButton");
    btn_clear = getChildByText<QPushButton>(window, "clear", "QPushButton");
    btn_days = getChildByText<QPushButton>(window, "= дни недели", "QPushButton");
    btn_months = getChildByText<QPushButton>(window, "= месяцы", "QPushButton");
}

std::vector<QString> TestYourApp::getVector() const {

    std::vector<QString> lines;
    const auto size = list_widget->count();
    lines.reserve(size);
    for (int i = 0; i < size; ++i) {
        const auto& item = list_widget->item(i);
        lines.push_back(item->text());
    }
    return lines;
}

void TestYourApp::checkExpectedVector(const std::vector<QString>& expectedVector) const {
    const auto actual_vector = getVector();

    PRAC_COMPARE2(expectedVector.size(), actual_vector.size(), "Размер вектора не совпадает с ожидаемым");
    const auto size_from_string = txt_size->text().toUInt();
    PRAC_COMPARE2(actual_vector.size(), size_from_string, "Размер в поле txt_size не совпадает с реальным");

    for (size_t i = 0; i < expectedVector.size(); ++i) {
        const QString expected_string = QString("%1: %2").arg(i).arg(expectedVector.at(i));
        const auto& actual_string = actual_vector.at(i);
        PRAC_COMPARE2(expected_string, actual_string, "Строка вектора не совпадает с ожидаемой");
    }
}

void TestYourApp::pushBack(const QString& str) const {
    txt_elem_content->setText(str);
    clickWidget(btn_push_back);
}

void TestYourApp::pushBackVector(const std::vector<QString>& vector) const {
    for (const auto& item : vector) {
        pushBack(item);
    }
}

void TestYourApp::popBack() const {
    clickWidget(btn_pop_back);
}

void TestYourApp::clear() const {
    clickWidget(btn_clear);
}

void TestYourApp::TestPushBack() {
    const std::vector<QString> lines{
        "first row",
        "second row"
    };
    pushBackVector(lines);
    checkExpectedVector(lines);
}

void TestYourApp::TestPopBack() {

    QVERIFY2(getVector().empty(), "Вектор объектов должен быть пустым при старте программы");
    QVERIFY2(!btn_pop_back->isEnabled(), "Кнопка pop_back должна быть выключена для пустого вектора");

    std::vector<QString> lines{
        "first row",
        "second row",
        "third row"
    };
    pushBackVector(lines);
    checkExpectedVector(lines);

    lines.pop_back();
    popBack();
    checkExpectedVector(lines);

    while(!lines.empty()) {
        lines.pop_back();
        popBack();
    }

    checkExpectedVector(lines);
    QVERIFY2(!btn_pop_back->isEnabled(), "Кнопка pop_back должна быть выключена для пустого вектора");

}

void TestYourApp::TestClear() {
    std::vector<QString> lines{
        "first row",
        "second row",
        "third row"
    };

    pushBackVector(lines);
    checkExpectedVector(lines);

    lines.clear();
    clear();
    checkExpectedVector(lines);
}

void TestYourApp::TestDays() {
    clickWidget(btn_days);
    checkExpectedVector(days_of_week);
}

void TestYourApp::TestMonths() {
    clickWidget(btn_months);
    checkExpectedVector(months_of_year);
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
