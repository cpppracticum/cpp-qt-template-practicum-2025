
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
    void init();
    void cleanup();

    void TestPushBack();
    void TestPopBack();
    void TestClear();

    void TestDays();
    void TestMonths();

    void TestEdit();
    void TestDec();
    void TestInc();
    void TestBegin();
    void TestEnd();

    void TestInsert();
    void TestErase();

    void TestIteratorInvalidation();

    void TestReserve();
    void TestResizeUp();
    void TestResizeDown();
    void TestBigVec();


private:
    [[nodiscard]] std::vector<QString> getVector() const;
    [[nodiscard]] int getIndex() const;
    void checkExpectedVector() const;
    void checkIndex() const;
    void checkEnd() const;
    void checkButtons() const;
    void checkContent() const;
    void checkCapacity() const;
    void checkSize() const;

    void checkModel() const;

    void pushBack(const QString& str);
    void pushBackVector(const std::vector<QString>& vector);
    void popBack();
    void clear();

    void insert();
    void insert(const QString& str);
    void erase();

    void begin();
    void end();
    void inc();
    void dec();

    void edit();
    void edit(const QString& str);

    void reserve() const;
    void reserve(size_t new_capacity);
    void resize() const;
    void resize(size_t new_size);

    void setCurrentElement(int index);

    MainWindow *window;

    QListWidget *list_widget{};
    QLineEdit *txt_elem_content{};
    QLineEdit *txt_size{};
    QLineEdit *txt_capacity{};

    QPushButton *btn_push_back{};
    QPushButton *btn_pop_back{};
    QPushButton *btn_clear{};
    QPushButton *btn_days{};
    QPushButton *btn_months{};

    QPushButton *btn_erase;
    QPushButton *btn_insert;

    QPushButton *btn_begin;
    QPushButton *btn_end;
    QPushButton *btn_inc;
    QPushButton *btn_dec;

    QPushButton *btn_edit;

    QPushButton *btn_reserve;
    QPushButton *btn_resize;

    using container = std::vector<QString>;
    container vec{};
    container::iterator it = vec.begin();
    QString content;


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


    static inline const container default_lines{
        "first row",
        "second row",
        "third row",
        "fourth row",
        "fifth row"
    };

};



void TestYourApp::init()
{
    window = new MainWindow();

    QVERIFY2(window != nullptr, "Главное окно не создано");
    window->show();
    QVERIFY2(window->isVisible(), "Главное окно не активируется");
    list_widget = getChild<QListWidget>(window, "list_widget", "QListWidget");
    txt_elem_content = getChild<QLineEdit>(window, "txt_elem_content", "QLineEdit");
    txt_size = getChild<QLineEdit>(window, "txt_size", "QLineEdit");
    txt_capacity = getChild<QLineEdit>(window, "txt_capacity", "QLineEdit");

    btn_push_back = getChildByText<QPushButton>(window, "push_back", "QPushButton");
    btn_pop_back = getChildByText<QPushButton>(window, "pop_back", "QPushButton");
    btn_clear = getChildByText<QPushButton>(window, "clear", "QPushButton");
    btn_days = getChildByText<QPushButton>(window, "= дни недели", "QPushButton");
    btn_months = getChildByText<QPushButton>(window, "= месяцы", "QPushButton");

    btn_erase = getChildByText<QPushButton>(window, "erase", "QPushButton");
    btn_insert = getChildByText<QPushButton>(window, "insert", "QPushButton");

    btn_begin = getChildByText<QPushButton>(window, "begin", "QPushButton");
    btn_end = getChildByText<QPushButton>(window, "end", "QPushButton");
    btn_inc = getChildByText<QPushButton>(window, "++", "QPushButton");
    btn_dec = getChildByText<QPushButton>(window, "--", "QPushButton");

    btn_edit = getChildByText<QPushButton>(window, "Edit", "QPushButton");

    btn_reserve = getChildByText<QPushButton>(window, "reserve", "QPushButton");
    btn_resize = getChildByText<QPushButton>(window, "resize", "QPushButton");


    vec = {};
    it = vec.begin();
    content = {};
    vec.shrink_to_fit();

}

std::vector<QString> TestYourApp::getVector() const {
    std::vector<QString> lines;
    const auto size = list_widget->count();
    lines.reserve(size);
    for (int i = 0; i < size - 1; ++i) {
        const auto& item = list_widget->item(i);
        lines.push_back(item->text());
    }
    return lines;
}

int TestYourApp::getIndex() const {
    return list_widget->currentRow();
}

void TestYourApp::checkExpectedVector() const {
    const auto actual_vector = getVector();

    PRAC_COMPARE2(vec.size(), actual_vector.size(), "Размер вектора не совпадает с ожидаемым");
    const auto size_from_string = txt_size->text().toUInt();
    PRAC_COMPARE2(actual_vector.size(), size_from_string, "Размер в поле txt_size не совпадает с реальным");
    for (size_t i = 0; i < vec.size(); ++i) {
        const QString expected_string = QString("%1: %2").arg(i).arg(vec.at(i));
        const auto& actual_string = actual_vector.at(i);
        PRAC_COMPARE2(expected_string, actual_string, "Строка вектора не совпадает с ожидаемой");
    }
}

void TestYourApp::checkIndex() const {
    const auto actual = getIndex();
    const auto expected = it - vec.begin();
    PRAC_COMPARE2(actual, expected, "Индекс выбранного элемента не совпадает с ожидаемым");
}

void TestYourApp::checkEnd() const {
    const auto size = list_widget->count();
    QVERIFY2(size, "В векторе отстствуют элементы, в том числе фиктивный элемент end");
    const auto *const end_element = list_widget->item(size - 1);
    PRAC_COMPARE2(end_element->text(), "end", "Последний элемент вектора всегда должен быть end");
}

void TestYourApp::checkButtons() const {
    // Можно проверять по внутреннему итератору,
    // поскольку уже выполнены проверки совпадения векторов и итераторов
    const auto should_be_enabled = it != vec.end();

    PRAC_COMPARE2(btn_edit->isEnabled(), should_be_enabled, "Кнопка Edit должна быть неактивна, когда итератор указывает на фиктивный элемент end, и активна в иных случаях");
    PRAC_COMPARE2(btn_inc->isEnabled(), should_be_enabled, "Кнопка ++ должна быть неактивна, когда итератор указывает на фиктивный элемент end, и активна в иных случаях");
    PRAC_COMPARE2(btn_erase->isEnabled(), should_be_enabled, "Кнопка erase должна быть неактивна, когда итератор указывает на фиктивный элемент end, и активна в иных случаях");

    const auto is_it_begin = it == vec.begin();
    QVERIFY2(btn_dec->isEnabled() != is_it_begin, "Кнопка -- должна быть неактивна, когда итератор указывает на начало вектора, и активна в иных случаях");
}

void TestYourApp::checkContent() const {
    // Аналогично checkButtons, можно проверять по внутреннему итератору,
    // поскольку уже выполнены проверки совпадения векторов и итераторов
    const auto& actual_text = txt_elem_content->text();
    if (it == vec.end()) {
        PRAC_COMPARE2(actual_text, "", "Текст в txt_elem_content должен отсутствовать, если итератор указывает на конец массива");
        return;
    };
    const auto& expected_text = *it;
    PRAC_COMPARE2(actual_text, expected_text, "Текст в txt_elem_content не совпадает с ожидаемым");
}

void TestYourApp::checkCapacity() const {
    const auto capacity_str = txt_capacity->text();
    const size_t capacity = capacity_str.toUInt();
    const size_t expected_capacity = vec.capacity();
    QVERIFY2(capacity >= expected_capacity, "Вместимость вектора меньше ожидаемой");
}

void TestYourApp::checkSize() const {
    const auto size_str = txt_size->text();
    const size_t size = size_str.toUInt();
    const size_t expected_size = vec.size();
    PRAC_COMPARE2(size, expected_size, "Размер вектора в txt_size не совпадает с ожидаемым");
}

void TestYourApp::checkModel() const {
    checkEnd();
    checkExpectedVector();
    checkIndex();
    checkButtons();
    checkContent();
    checkCapacity();
    checkSize();
}

void TestYourApp::pushBack(const QString& str) {
    txt_elem_content->setText(str);
    clickWidget(btn_push_back);

    vec.push_back(str);
    it = vec.begin();
}

void TestYourApp::pushBackVector(const std::vector<QString>& vector) {
    for (const auto& item : vector) {
        pushBack(item);
    }
}

void TestYourApp::popBack() {
    clickWidget(btn_pop_back);
    vec.pop_back();
    it = vec.begin();
}

void TestYourApp::clear() {
    clickWidget(btn_clear);
    vec.clear();
    it = vec.begin();
}

void TestYourApp::insert() {
    clickWidget(btn_insert);
    vec.insert(it, content);
    it = vec.begin();
}

void TestYourApp::insert(const QString& str) {
    txt_elem_content->setText(str);
    content = str;
    insert();
}

void TestYourApp::erase() {
    clickWidget(btn_erase);
    vec.erase(it);
    it = vec.begin();
}

void TestYourApp::begin() {
    clickWidget(btn_begin);
    it = vec.begin();
}

void TestYourApp::end() {
    clickWidget(btn_end);
    it = vec.end();
}

void TestYourApp::inc() {
    clickWidget(btn_inc);
    ++it;
}

void TestYourApp::dec() {
    clickWidget(btn_dec);
    --it;
}

void TestYourApp::edit() {
    clickWidget(btn_edit);
    *it = content;
}

void TestYourApp::edit(const QString& str) {
    txt_elem_content->setText(str);
    content = str;
    edit();
}

void TestYourApp::reserve() const {
    clickWidget(btn_reserve);
}

void TestYourApp::reserve(const size_t new_capacity) {
    txt_capacity->setText(QString::number(new_capacity));
    reserve();

    vec.reserve(new_capacity);
    it = vec.begin();
}

void TestYourApp::resize() const {
    clickWidget(btn_resize);
}

void TestYourApp::resize(const size_t new_size) {
    txt_size->setText(QString::number(new_size));
    resize();

    vec.resize(new_size);
    it = vec.begin();
}

void TestYourApp::setCurrentElement(int index) {
    // С разрешением указывать на end
    const int clamped_index = std::clamp(index, 0, static_cast<int>(vec.size()));
    it = vec.begin() + clamped_index;
    list_widget->setCurrentRow(clamped_index);
}

void TestYourApp::TestPushBack() {
    const std::vector<QString> lines{
        "first row",
        "second row"
    };
    pushBackVector(lines);
    checkModel();
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
    checkModel();
    while(!vec.empty()) {
        popBack();
    }
    checkModel();
    QVERIFY2(!btn_pop_back->isEnabled(), "Кнопка pop_back должна быть выключена для пустого вектора");
}

void TestYourApp::TestClear() {
    std::vector<QString> lines{
        "first row",
        "second row",
        "third row"
    };

    pushBackVector(lines);
    checkModel();

    lines.clear();
    clear();
    checkModel();
}

void TestYourApp::TestDays() {
    clickWidget(btn_days);
    vec = days_of_week;
    it = vec.begin();
    checkModel();
}

void TestYourApp::TestMonths() {
    clickWidget(btn_months);
    vec = months_of_year;
    it = vec.begin();
    checkModel();
}

void TestYourApp::TestEdit() {
    pushBackVector(default_lines);

    checkModel();
    edit("new first row");

    setCurrentElement(2);
    edit("new third row");
    checkModel();

    setCurrentElement(3);
    edit("new fourth row");
    checkModel();
}

void TestYourApp::TestBegin() {
    pushBackVector(default_lines);

    setCurrentElement(3);
    checkModel();
    begin();
    checkModel();

    setCurrentElement(2);
    begin();
    checkModel();
}

void TestYourApp::TestEnd() {
    pushBackVector(default_lines);

    end();
    checkModel();
    setCurrentElement(3);
    end();
    checkModel();
}

void TestYourApp::TestDec() {
    pushBackVector(default_lines);
    end();
    while(it != vec.end()) {
        dec();
        checkModel();
    }
}

void TestYourApp::TestInc() {
    pushBackVector(default_lines);
    begin();
    while(it != vec.end()) {
        inc();
        checkModel();
    }
}

void TestYourApp::TestErase() {
    pushBackVector(default_lines);
    setCurrentElement(2);
    erase();
    checkModel();
    setCurrentElement(static_cast<int>(vec.size()) - 1);
    erase();
    checkModel();
}

void TestYourApp::TestInsert() {
    pushBackVector(default_lines);
    setCurrentElement(3);
    insert("New string");
    checkModel();
    setCurrentElement(3);
    insert("New new string");
    checkModel();

    setCurrentElement(static_cast<int>(vec.size()));
    insert("New string at the end");
    checkModel();
}

void TestYourApp::TestIteratorInvalidation() {
    pushBackVector(default_lines);

    setCurrentElement(3);
    pushBack("New string at the back");
    checkModel();

    setCurrentElement(5);
    popBack();
    checkModel();

    setCurrentElement(1);
    insert("Inserted string");
    checkModel();

    setCurrentElement(2);
    erase();
    checkModel();

    setCurrentElement(4);
    clear();
    checkModel();
}

void TestYourApp::TestReserve() {
    pushBackVector(default_lines);
    reserve(25);
    checkModel();

    pushBackVector(default_lines);
    checkModel();
}

void TestYourApp::TestResizeUp() {
    pushBackVector(default_lines);
    checkModel();
    resize(default_lines.size() + 15);
    checkModel();
    setCurrentElement(static_cast<int>(default_lines.size() + 3));
    edit("New string");
    checkModel();
}

void TestYourApp::TestResizeDown() {
    pushBackVector(default_lines);
    pushBackVector(default_lines);
    checkModel();
    resize(default_lines.size() + 1);
    checkModel();
    setCurrentElement(static_cast<int>(default_lines.size() - 1));
    edit("New string");
    checkModel();
}

void TestYourApp::TestBigVec() {
    reserve(1000);
    checkModel();
    for(int i = 0; i < 1000; ++i) {
        pushBack(QString::number(i));
    }
    checkModel();
    resize(100);
    checkModel();
}

void TestYourApp::cleanup()
{
    delete window;
}

QTEST_MAIN(TestYourApp)

#include "tests.moc"
