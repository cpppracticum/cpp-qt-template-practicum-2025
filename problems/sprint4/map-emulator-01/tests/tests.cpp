
// Core headers
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QWidget>

#include <QtWidgets/QPushButton>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>

#include <QtTest>

#include <iostream>

#include <algorithm>
#include <iterator>
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

    void TestSetByKey();
    void TestErase();
    void TestClear();
    void TestBegin();
    void TestEnd();
    void TestInc();
    void TestDec();
    void TestFind();
    void TestChangeByKey();

    void TestWords();
    void TestBooks();

private:
    [[nodiscard]] std::vector<QString> getList() const;
    void checkExpectedMap(const std::map<QString, QString>& expectedMap) const;
    void setCurrentElement(int index);
    void fill(const std::map<QString, QString>& values);

    MainWindow *window;

    QListWidget *list_widget;
    QLineEdit *txt_elem_key;
    QLineEdit *txt_elem_value;
    QLineEdit *txt_size;

    QPushButton *btn_set_by_key;
    QPushButton *btn_erase;
    QPushButton *btn_clear;
    QPushButton *btn_find;
    QPushButton *btn_dec_iterator;
    QPushButton *btn_inc_iterator;
    QPushButton *btn_begin;
    QPushButton *btn_end;
    QPushButton *btn_books;
    QPushButton *btn_words;

    using container = std::map<QString, QString>;
    container dict{};
    container::iterator it = dict.begin();

    static inline const std::map<QString, QString> words {
        {"Book", "Книга"},
        {"Cinnamon", "Корица"},
        {"Cup", "Чашка"},
        {"Kettle", "Чайник"},
        {"Saucer", "Блюдце"},
        {"Sugar", "Сахар"},
        {"Tea", "Чай"},
        {"Teaspoon", "Чайная ложка"},
    };

    static inline const std::map<QString, QString> authors_and_books {
        {"Т. Пратчетт", "Плоский мир"},
        {"Р. Брэдбери", "Лёд и пламя"},
        {"Д. Адамс", "В основном безвредна"},
        {"Ф. Дик", "Солнечная лотерея"},
        {"А. и Б. Стругацкие", "Жук в муравейнике"},
        {"Ф. Кафка", "Превращение"},
    };
};


void TestYourApp::init()
{
    window = new MainWindow();

    QVERIFY2(window != nullptr, "Главное окно не создано");
    window->show();
    QVERIFY2(window->isVisible(), "Главное окно не активируется");

    list_widget = getChild<QListWidget>(window, "list_widget", "QListWidget");
    txt_elem_key = getChild<QLineEdit>(window, "txt_elem_key", "QLineEdit");
    txt_elem_value = getChild<QLineEdit>(window, "txt_elem_value", "QLineEdit");
    txt_size = getChild<QLineEdit>(window, "txt_size", "QLineEdit");
    btn_set_by_key = getChildByText<QPushButton>(window, "Изменить", "QPushButton");
    btn_erase = getChildByText<QPushButton>(window, "erase", "QPushButton");
    btn_clear = getChildByText<QPushButton>(window, "clear", "QPushButton");
    btn_find = getChildByText<QPushButton>(window, "find", "QPushButton");
    btn_dec_iterator = getChildByText<QPushButton>(window, "--", "QPushButton");
    btn_inc_iterator = getChildByText<QPushButton>(window, "++", "QPushButton");
    btn_begin = getChildByText<QPushButton>(window, "begin", "QPushButton");
    btn_end = getChildByText<QPushButton>(window, "end", "QPushButton");
    btn_books = getChildByText<QPushButton>(window, "= книги", "QPushButton");
    btn_words = getChildByText<QPushButton>(window, "= словарь", "QPushButton");

    dict = {};
    it = dict.begin();
}

std::vector<QString> TestYourApp::getList() const {
    std::vector<QString> lines;
    const auto size = list_widget->count();
    lines.reserve(size);
    for (int i = 0; i < size - 1; ++i) {
        const auto& item = list_widget->item(i);
        lines.push_back(item->text());
    }
    return lines;
}

void TestYourApp::checkExpectedMap(const std::map<QString, QString>& expectedMap) const {
    std::vector<QString> expectedVector;
    for (const auto& iter : expectedMap) {
        expectedVector.push_back(QString("%1: %2").arg(iter.first).arg(iter.second));
    }
    const auto actual_vector = getList();

    std::vector<std::string> actual;
    for (int i = 0; i < actual_vector.size(); ++i) {
        actual.push_back(actual_vector[i].toStdString());
    }
    std::vector<std::string> expected;
    for (int i = 0; i < expectedVector.size(); ++i) {
        expected.push_back(expectedVector[i].toStdString());
    }

    PRAC_COMPARE2(expected.size(), actual.size(), "Размер словаря не совпадает с ожидаемым");
    const auto size_from_string = txt_size->text().toUInt();
    PRAC_COMPARE2(actual.size(), size_from_string, "Размер в поле txt_size не совпадает с реальным");

    std::sort(expected.begin(), expected.end());
    std::sort(actual.begin(), actual.end());

    for (size_t i = 0; i < expectedVector.size(); ++i) {
        const auto& expected_string = expectedVector.at(i);
        const auto& actual_string = actual_vector.at(i);
        PRAC_COMPARE2(expected_string, actual_string, "Запись в словаре не совпадает с ожидаемой");
    }

    const auto actual_pos = list_widget->currentRow();
    const auto expected_pos = std::distance<std::map<QString, QString>::const_iterator>(dict.begin(), it);
    PRAC_COMPARE2(actual_pos, expected_pos, "Индекс выбранного элемента не совпадает с ожидаемым");
}

void TestYourApp::setCurrentElement(int index) {
    const int clamped_index = std::clamp(index, 0, static_cast<int>(dict.size()));
    it = dict.begin();
    std::advance(it, clamped_index);
    list_widget->setCurrentRow(clamped_index);
}

void TestYourApp::fill(const std::map<QString, QString>& values) {
    for (const auto& iter : values) {
        txt_elem_key->setText(iter.first);
        txt_elem_value->setText(iter.second);
        clickWidget(btn_set_by_key);
        dict[iter.first] = iter.second;
    }
}

void TestYourApp::TestSetByKey() {
    const std::map<QString, QString> values = {{"key1", "value1"} , {"key2", "value2"}};
    fill(values);

    checkExpectedMap(values);
}

void TestYourApp::TestErase() {
    QVERIFY2(getList().empty(), "Словарь объектов должен быть пустым при старте программы");
    QVERIFY2(!btn_erase->isEnabled(), "Кнопка erase должна быть выключена для пустого cловаря");

    const std::map<QString, QString> values = {{"key1", "value1"} , {"key2", "value2"}};
    fill(values);

    setCurrentElement(dict.size() - 1);
    clickWidget(btn_erase);
    dict.erase(it);
    it = dict.begin();
    checkExpectedMap(dict);

    setCurrentElement(dict.size() - 1);
    clickWidget(btn_erase);
    dict.erase(it);
    it = dict.begin();
    checkExpectedMap(dict);
    QVERIFY2(!btn_erase->isEnabled(), "Кнопка erase должна быть выключена для пустого cловаря");
}

void TestYourApp::TestClear() {
    const std::map<QString, QString> values = {{"key1", "value1"} , {"key2", "value2"}};
    fill(values);

    clickWidget(btn_clear);
    dict.clear();
    it = dict.begin();
    checkExpectedMap(dict);
}

void TestYourApp::TestBegin() {
    const std::map<QString, QString> values = {{"key1", "value1"} , {"key2", "value2"}};
    fill(values);

    setCurrentElement(dict.size());
    checkExpectedMap(dict);
    clickWidget(btn_begin);
    it = dict.begin();
    checkExpectedMap(dict);

    setCurrentElement(dict.size() - 1);
    clickWidget(btn_begin);
    it = dict.begin();
    checkExpectedMap(dict);
    QVERIFY2(!btn_dec_iterator->isEnabled(), "Кнопка -- должна быть выключена в начале словаря");
}

void TestYourApp::TestEnd() {
    const std::map<QString, QString> values = {{"key1", "value1"} , {"key2", "value2"}};
    fill(values);

    clickWidget(btn_end);
    it = dict.end();
    checkExpectedMap(dict);

    setCurrentElement(dict.size());
    clickWidget(btn_end);
    it = dict.end();
    checkExpectedMap(dict);
    QVERIFY2(!btn_inc_iterator->isEnabled(), "Кнопка ++ должна быть выключена в конце словаря");
}

void TestYourApp::TestInc() {
    const std::map<QString, QString> values = {{"key1", "value1"} , {"key2", "value2"}};
    fill(values);

    clickWidget(btn_begin);
    it = dict.begin();
    while(it != dict.end()) {
        clickWidget(btn_inc_iterator);
        ++it;
        checkExpectedMap(dict);
    }
    QVERIFY2(!btn_inc_iterator->isEnabled(), "Кнопка ++ должна быть выключена в конце словаря");
}

void TestYourApp::TestDec() {
    const std::map<QString, QString> values = {{"key1", "value1"} , {"key2", "value2"}};
    fill(values);

    clickWidget(btn_end);
    it = dict.end();
    while(it != dict.begin()) {
        clickWidget(btn_dec_iterator);
        --it;
        checkExpectedMap(dict);
    }
    QVERIFY2(!btn_dec_iterator->isEnabled(), "Кнопка -- должна быть выключена в начале словаря");
}

void TestYourApp::TestFind() {
    const std::map<QString, QString> values = {{"key1", "value1"} , {"key2", "value2"}};
    fill(values);

    const QString existing = "key2";
    txt_elem_key->setText(existing);
    clickWidget(btn_find);
    it = dict.find(existing);
    checkExpectedMap(dict);

    const QString non_existing = "key3";
    txt_elem_key->setText(non_existing);
    clickWidget(btn_find);
    it = dict.find(non_existing);
    checkExpectedMap(dict);
}

void TestYourApp::TestChangeByKey() {
    const std::map<QString, QString> values = {{"key1", "value1"} , {"key2", "value2"}};
    fill(values);

    checkExpectedMap(dict);

    txt_elem_key->setText("key2");
    txt_elem_value->setText("new_value2");
    clickWidget(btn_set_by_key);
    dict["key2"] = "new_value2";

    checkExpectedMap(dict);
}

void TestYourApp::TestWords() {
    clickWidget(btn_words);
    dict = words;
    it = dict.begin();
    checkExpectedMap(words);
}

void TestYourApp::TestBooks() {
    clickWidget(btn_books);
    dict = words;
    it = dict.begin();
    checkExpectedMap(authors_and_books);
}

void TestYourApp::cleanup()
{
    delete window;
}

QTEST_MAIN(TestYourApp)

#include "tests.moc"
