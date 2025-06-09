
// Core headers
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QWidget>

#include <QtWidgets/QPushButton>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QLabel>

#include <QtTest>

#include <deque>
#include <string>

#include "algo.h"
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

    void TestMerge();
    void TestMergeSort();

    void TestPushFront();
    void TestPopFront();

    void TestPushBack();
    void TestPopBack();
    void TestClear();

    void TestEdit();
    void TestDec();
    void TestInc();
    void TestBegin();
    void TestEnd();

    void TestInsert();
    void TestErase();

    void TestIteratorInvalidation();

    void TestResizeUp();
    void TestResizeDown();

    void TestFind();
    void TestCount();

    void TestMinElement();
    void TestMaxElement();

    void TestSort();
    void TestsOrT();
    void TestShuffle();
    void TestUnique();
    void TestUniqueUnsorted();
    void TestReverse();

    void TestLowerBound();
    void TestUpperBound();

    void TestTeaTime();


private:
    using container = std::deque<QString>;
    [[nodiscard]] container getDeque() const;
    [[nodiscard]] int getIndex() const;
    void checkExpectedDeque() const;
    void checkIndex() const;
    void checkEnd() const;
    void checkButtons() const;
    void checkContent() const;
    void checkSize() const;

    void checkModel() const;

    void pushFront(const QString& str);
    void pushFrontLines(const container& lines);
    void popFront();

    void pushBack(const QString& str);
    void pushBackLines(const container& lines);
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

    void resize() const;
    void resize(size_t new_size);

    void find(const QString& str);

    [[nodiscard]] size_t count(const QString& str) const;

    void min_element();
    void max_element();

    void sort();
    void sOrT();
    void shuffle();
    void shuffle(size_t seed);
    void unique();
    void reverse();

    void lowerBound() const;
    void lowerBound(const QString& bound);

    void upperBound() const;
    void upperBound(const QString& bound);

    void setCurrentElement(int index);

    MainWindow *window;

    QListWidget *list_widget{};
    QLineEdit *txt_elem_content{};
    QLineEdit *txt_size{};
    QLineEdit *le_count{};

    QLabel *lbl_count{};

    QPushButton *btn_push_front{};
    QPushButton *btn_pop_front{};

    QPushButton *btn_push_back{};
    QPushButton *btn_pop_back{};
    QPushButton *btn_clear{};

    QPushButton *btn_tea{};
    QPushButton *btn_cakes{};

    QPushButton *btn_erase{};
    QPushButton *btn_insert{};

    QPushButton *btn_begin{};
    QPushButton *btn_end{};
    QPushButton *btn_inc{};
    QPushButton *btn_dec{};

    QPushButton *btn_edit{};

    QPushButton *btn_resize{};

    QPushButton *btn_find{};
    QPushButton *btn_count{};

    QPushButton *btn_min_element{};
    QPushButton *btn_max_element{};

    QPushButton *btn_sort{};
    QPushButton *btn_sOrT{};
    QPushButton *btn_shuffle{};
    QPushButton *btn_unique{};
    QPushButton *btn_reverse{};

    QPushButton *btn_lower_bound{};
    QPushButton *btn_upper_bound{};

    container deq{};
    container::iterator it = deq.begin();
    QString content;
    QString expected_content;

    std::mt19937 random_gen;

    static inline const container tea = {
        "Чай Лунцзин",
        "Эрл Грей",
        "Сенча",
        "Пуэр",
        "Дарджилинг",
        "Ассам",
        "Матча",
        "Ганпаудер",
        "Оолонг",
        "Лапсанг Сушонг"
        };

    static inline const container cakes = {
        "Красный бархат",
        "Наполеон",
        "Медовик",
        "Тирамису",
        "Прага",
        "Чизкейк",
        "Захер",
        "Эстерхази",
        "Морковный торт",
        "Чёрный лес",
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
    le_count = getChild<QLineEdit>(window, "le_count", "QLineEdit");

    lbl_count = getChild<QLabel>(window, "lbl_count", "QLabel");

    btn_push_front = getChildByText<QPushButton>(window, "push_front", "QPushButton");
    btn_pop_front = getChildByText<QPushButton>(window, "pop_front", "QPushButton");

    btn_push_back = getChildByText<QPushButton>(window, "push_back", "QPushButton");
    btn_pop_back = getChildByText<QPushButton>(window, "pop_back", "QPushButton");
    btn_clear = getChildByText<QPushButton>(window, "clear", "QPushButton");
    btn_tea = getChildByText<QPushButton>(window, "= чай", "QPushButton");
    btn_cakes = getChildByText<QPushButton>(window, "= торты", "QPushButton");

    btn_erase = getChildByText<QPushButton>(window, "erase", "QPushButton");
    btn_insert = getChildByText<QPushButton>(window, "insert", "QPushButton");

    btn_begin = getChildByText<QPushButton>(window, "begin", "QPushButton");
    btn_end = getChildByText<QPushButton>(window, "end", "QPushButton");
    btn_inc = getChildByText<QPushButton>(window, "++", "QPushButton");
    btn_dec = getChildByText<QPushButton>(window, "--", "QPushButton");

    btn_edit = getChildByText<QPushButton>(window, "Edit", "QPushButton");

    btn_resize = getChildByText<QPushButton>(window, "resize", "QPushButton");

    btn_find = getChildByText<QPushButton>(window, "find", "QPushButton");
    btn_count = getChildByText<QPushButton>(window, "count", "QPushButton");

    btn_min_element = getChildByText<QPushButton>(window, "min_element", "QPushButton");
    btn_max_element = getChildByText<QPushButton>(window, "max_element", "QPushButton");

    btn_sort = getChildByText<QPushButton>(window, "merge sort", "QPushButton");
    btn_sOrT = getChildByText<QPushButton>(window, "merge sOrT", "QPushButton");
    btn_shuffle = getChildByText<QPushButton>(window, "shuffle", "QPushButton");
    btn_unique = getChildByText<QPushButton>(window, "unique", "QPushButton");
    btn_reverse = getChildByText<QPushButton>(window, "reverse", "QPushButton");

    btn_lower_bound = getChildByText<QPushButton>(window, "lower_bound", "QPushButton");
    btn_upper_bound = getChildByText<QPushButton>(window, "upper_bound", "QPushButton");


    deq = {};
    it = deq.begin();
    content = {};
    deq.shrink_to_fit();

    random_gen.seed(12);
}

TestYourApp::container TestYourApp::getDeque() const {
    container lines;
    const auto size = list_widget->count();
    for (int i = 0; i < size - 1; ++i) {
        const auto& item = list_widget->item(i);
        lines.push_back(item->text());
    }
    return lines;
}

int TestYourApp::getIndex() const {
    return list_widget->currentRow();
}

void TestYourApp::checkExpectedDeque() const {
    const auto actual_vector = getDeque();

    PRAC_COMPARE2(deq.size(), actual_vector.size(), "Размер вектора не совпадает с ожидаемым");
    const auto size_from_string = txt_size->text().toUInt();
    PRAC_COMPARE2(actual_vector.size(), size_from_string, "Размер в поле txt_size не совпадает с реальным");
    for (size_t i = 0; i < deq.size(); ++i) {
        const QString expected_string = QString("%1: %2").arg(i).arg(deq.at(i));
        const auto& actual_string = actual_vector.at(i);
        PRAC_COMPARE2(expected_string, actual_string, "Строка вектора не совпадает с ожидаемой");
    }
}

void TestYourApp::checkIndex() const {
    const auto actual = getIndex();
    const auto expected = it - deq.begin();
    QCOMPARE(actual, (int) expected);
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
    const auto should_be_enabled = it != deq.end();

    PRAC_COMPARE2(btn_edit->isEnabled(), should_be_enabled, "Кнопка Edit должна быть неактивна, когда итератор указывает на фиктивный элемент end, и активна в иных случаях");
    PRAC_COMPARE2(btn_inc->isEnabled(), should_be_enabled, "Кнопка ++ должна быть неактивна, когда итератор указывает на фиктивный элемент end, и активна в иных случаях");
    PRAC_COMPARE2(btn_erase->isEnabled(), should_be_enabled, "Кнопка erase должна быть неактивна, когда итератор указывает на фиктивный элемент end, и активна в иных случаях");

    const auto is_it_begin = it == deq.begin();
    QVERIFY2(btn_dec->isEnabled() != is_it_begin, "Кнопка -- должна быть неактивна, когда итератор указывает на начало вектора, и активна в иных случаях");

    const auto is_not_empty = !deq.empty();
    PRAC_COMPARE2(btn_pop_front->isEnabled(), is_not_empty, "Кнопка pop_front должна быть выключена, когда список пуст");
    PRAC_COMPARE2(btn_pop_back->isEnabled(), is_not_empty, "Кнопка pop_front должна быть выключена, когда список пуст");
}

void TestYourApp::checkContent() const {
    // Аналогично checkButtons, можно проверять по внутреннему итератору,
    // поскольку уже выполнены проверки совпадения векторов и итераторов
    const auto& actual_text = txt_elem_content->text();
    if (it == deq.end()) {
        PRAC_COMPARE2(actual_text, "", "Текст в txt_elem_content должен отсутствовать, если итератор указывает на конец массива");
        return;
    }
    const auto& expected_text = expected_content.isEmpty() ? *it : expected_content;
    QCOMPARE(actual_text, expected_text);
}

void TestYourApp::checkSize() const {
    const auto size_str = txt_size->text();
    const size_t size = size_str.toUInt();
    const size_t expected_size = deq.size();
    PRAC_COMPARE2(size, expected_size, "Размер вектора в txt_size не совпадает с ожидаемым");
}

void TestYourApp::checkModel() const {
    checkEnd();
    checkExpectedDeque();
    checkIndex();
    checkButtons();
    checkContent();
    checkSize();
}

void TestYourApp::pushFront(const QString& str) {
    txt_elem_content->setText(str);
    clickWidget(btn_push_front);
    deq.push_front(txt_elem_content->text());
    it = deq.begin();
}

void TestYourApp::pushFrontLines(const TestYourApp::container& lines) {
    for (const auto& line : lines) {
        pushFront(line);
    }
}

void TestYourApp::popFront() {
    clickWidget(btn_pop_front);
    deq.pop_front();
    it = deq.begin();
}

void TestYourApp::pushBack(const QString& str) {
    txt_elem_content->setText(str);
    clickWidget(btn_push_back);

    deq.push_back(str);
    it = deq.begin();
}

void TestYourApp::pushBackLines(const TestYourApp::container& lines) {
    for (const auto& item : lines) {
        pushBack(item);
    }
}

void TestYourApp::popBack() {
    clickWidget(btn_pop_back);
    deq.pop_back();
    it = deq.begin();
}

void TestYourApp::clear() {
    clickWidget(btn_clear);
    deq.clear();
    it = deq.begin();
}

void TestYourApp::insert() {
    clickWidget(btn_insert);
    deq.insert(it, content);
    it = deq.begin();
}

void TestYourApp::insert(const QString& str) {
    txt_elem_content->setText(str);
    content = str;
    insert();
}

void TestYourApp::erase() {
    clickWidget(btn_erase);
    deq.erase(it);
    it = deq.begin();
}

void TestYourApp::begin() {
    clickWidget(btn_begin);
    it = deq.begin();
}

void TestYourApp::end() {
    clickWidget(btn_end);
    it = deq.end();
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

void TestYourApp::resize() const {
    clickWidget(btn_resize);
}

void TestYourApp::resize(const size_t new_size) {
    txt_size->setText(QString::number(new_size));
    resize();

    deq.resize(new_size);
    it = deq.begin();
}

void TestYourApp::find(const QString& str) {
    txt_elem_content->setText(str);
    clickWidget(btn_find);
    it = std::find(deq.begin(), deq.end(), str);
}

size_t TestYourApp::count(const QString& str) const {
    le_count->setText(str);
    clickWidget(btn_count);
    const auto count_str = lbl_count->text();
    return count_str.toUInt();
}

void TestYourApp::min_element() {
    clickWidget(btn_min_element);
    it = std::min_element(deq.begin(), deq.end());
}

void TestYourApp::max_element() {
    clickWidget(btn_max_element);
    it = std::max_element(deq.begin(), deq.end());
}

void TestYourApp::sort() {
    clickWidget(btn_sort);
    std::sort(deq.begin(), deq.end());
    it = deq.begin();
}

void TestYourApp::sOrT() {
    clickWidget(btn_sOrT);
    std::sort(deq.begin(), deq.end(), [](const QString& left, const QString& right) { return QString::compare(left, right, Qt::CaseInsensitive) < 0;});
    it = deq.begin();
}

void TestYourApp::shuffle() {
    window->SetRandomGen(random_gen);
    clickWidget(btn_shuffle);
    std::shuffle(deq.begin(), deq.end(), random_gen);
}

void TestYourApp::shuffle(size_t seed) {
    random_gen.seed(seed);
    shuffle();
}

void TestYourApp::unique() {
    clickWidget(btn_unique);

    if (!std::is_sorted(deq.begin(), deq.end())) {
        return;
    }
    const auto to_del = std::unique(deq.begin(), deq.end());
    deq.erase(to_del, deq.end());
    it = deq.begin();
}

void TestYourApp::reverse() {
    clickWidget(btn_reverse);
    std::reverse(deq.begin(), deq.end());
}

void TestYourApp::lowerBound() const {
    clickWidget(btn_lower_bound);
}

void TestYourApp::lowerBound(const QString& bound) {
    txt_elem_content->setText(bound);
    lowerBound();
    if (std::is_sorted(deq.begin(), deq.end())) {
        it = std::lower_bound(deq.begin(), deq.end(), bound);
    }

}

void TestYourApp::upperBound() const {
    clickWidget(btn_upper_bound);
}

void TestYourApp::upperBound(const QString& bound) {
    txt_elem_content->setText(bound);
    upperBound();
    if(std::is_sorted(deq.begin(), deq.end())) {
        it = std::upper_bound(deq.begin(), deq.end(), bound);
    }
}

void TestYourApp::setCurrentElement(int index) {
    // С разрешением указывать на end
    const int clamped_index = std::clamp(index, 0, static_cast<int>(deq.size()));
    it = deq.begin();
    std::advance(it, clamped_index);
    list_widget->setCurrentRow(clamped_index);
}

void TestYourApp::TestMerge() {

    const container half {
        "aaa",
        "bbb",
        "ccc",
        "ddd",
        "eee"
    };

    Comparator comparator;
    const auto result = Merge<QString>(half, half, comparator);

    const container expected_result{
        "aaa",
        "aaa",
        "bbb",
        "bbb",
        "ccc",
        "ccc",
        "ddd",
        "ddd",
        "eee",
        "eee"
    };

    QCOMPARE(result.size(), expected_result.size());

    for (auto it = result.begin(), exp_it = expected_result.begin(); it != result.end() and exp_it != expected_result.end(); ++it, ++exp_it) {
        QCOMPARE(*it, *exp_it);
    }

    QCOMPARE(comparator.getCompares(), 9);

}

void TestYourApp::TestMergeSort() {

    std::deque<int> data;

    for(int i = 0; i < 1000; ++i) {
        data.push_back(i);
    }

    Comparator comparator;
    random_gen.seed(150);

    std::shuffle(data.begin(), data.end(), random_gen);

    auto sorted = MergeSort(data, comparator);

    const int merge_sort_compares = comparator.getCompares();
    constexpr int expected_compares = 8731;

    QCOMPARE_LT(std::abs(expected_compares - merge_sort_compares), 50);

    std::sort(data.begin(), data.end());

    QCOMPARE(sorted.size(), data.size());

    for(auto it = sorted.begin(), exp_it = data.begin(); it != sorted.end() && exp_it != data.end(); ++it, ++exp_it) {
        QCOMPARE(*it, *exp_it);
    }
}

void TestYourApp::TestPushFront() {
    pushBackLines(default_lines);
    checkModel();
}

void TestYourApp::TestPopFront() {
    pushBackLines(default_lines);
    checkModel();
    while(deq.empty()) {
        popFront();
        checkModel();
    }
}

void TestYourApp::TestPushBack() {
    const container lines{
        "first row",
        "second row"
    };
    pushBackLines(lines);
    checkModel();
}

void TestYourApp::TestPopBack() {
    QVERIFY2(getDeque().empty(), "Вектор объектов должен быть пустым при старте программы");
    QVERIFY2(!btn_pop_back->isEnabled(), "Кнопка pop_back должна быть выключена для пустого вектора");

    container lines{
        "first row",
        "second row",
        "third row"
    };
    pushBackLines(lines);
    checkModel();
    while(!deq.empty()) {
        popBack();
    }
    checkModel();
    QVERIFY2(!btn_pop_back->isEnabled(), "Кнопка pop_back должна быть выключена для пустого вектора");
}

void TestYourApp::TestClear() {
    container lines{
        "first row",
        "second row",
        "third row"
    };

    pushBackLines(lines);
    checkModel();

    lines.clear();
    clear();
    checkModel();
}

void TestYourApp::TestEdit() {
    pushBackLines(default_lines);

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
    pushBackLines(default_lines);

    setCurrentElement(3);
    checkModel();
    begin();
    checkModel();

    setCurrentElement(2);
    begin();
    checkModel();
}

void TestYourApp::TestEnd() {
    pushBackLines(default_lines);

    end();
    checkModel();
    setCurrentElement(3);
    end();
    checkModel();
}

void TestYourApp::TestDec() {
    pushBackLines(default_lines);
    end();
    while(it != deq.end()) {
        dec();
        checkModel();
    }
}

void TestYourApp::TestInc() {
    pushBackLines(default_lines);
    begin();
    while(it != deq.end()) {
        inc();
        checkModel();
    }
}

void TestYourApp::TestErase() {
    pushBackLines(default_lines);
    setCurrentElement(2);
    erase();
    checkModel();
    setCurrentElement(static_cast<int>(deq.size()) - 1);
    erase();
    checkModel();
}

void TestYourApp::TestInsert() {
    pushBackLines(default_lines);
    setCurrentElement(3);
    insert("New string");
    checkModel();
    setCurrentElement(3);
    insert("New new string");
    checkModel();

    setCurrentElement(static_cast<int>(deq.size()));
    insert("New string at the end");
    checkModel();
}


void TestYourApp::TestIteratorInvalidation() {
    pushBackLines(default_lines);

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

void TestYourApp::TestResizeUp() {
    pushBackLines(default_lines);
    checkModel();
    resize(default_lines.size() + 15);
    checkModel();
    setCurrentElement(static_cast<int>(default_lines.size() + 3));
    edit("New string");
    checkModel();
}

void TestYourApp::TestResizeDown() {
    pushBackLines(default_lines);
    pushBackLines(default_lines);
    checkModel();
    resize(default_lines.size() + 1);
    checkModel();
    setCurrentElement(static_cast<int>(default_lines.size() - 1));
    edit("New string");
    checkModel();
}

void TestYourApp::TestFind() {
    pushBackLines(default_lines);
    const QString& string_to_find = default_lines.at(3);
    find(string_to_find);
    checkModel();

    const QString string_to_not_find = "That line is not from there, at least it should not be";
    find(string_to_not_find);
    checkModel();
}

void TestYourApp::TestCount() {
    const container lines = {
        "First unique line",
        "Not so unique line!",
        "Not so unique line!",
        "Not so unique line!",
        "Not so unique line!",
        "Not so unique line!",
        "Second totally unique line",
        "That line is also not actually unique",
        "That line is also not actually unique",
        "That line is also not actually unique",
        "Last unique line",
        "Just kidding, this is the last one ;)"
    };
    pushBackLines(lines);
    checkModel();
    const auto first_non_unique_line_quantity = count(lines.at(1));
    PRAC_COMPARE2(first_non_unique_line_quantity, 5, "Количество вхождений строки не совпадает с ожидаемым");

    const auto second_non_unique_line_quantity = count(lines.at(7));
    PRAC_COMPARE2(second_non_unique_line_quantity, 3, "Количество вхождений строки не совпадает с ожидаемым");

    for (const auto& line : {lines[0], lines[6], lines[10], lines[11]}) {
        PRAC_COMPARE2(count(line), 1, "Количество вхождений уникальной строки не совпадает с ожидаемым");
    }

    PRAC_COMPARE2(count("Line is not from vector"), 0, "Количество вхождений строки, отсутствующей в векторе, отлично от нуля");
}

void TestYourApp::TestMinElement() {
    const container lines = {
        "AAA",
        "bbb",
        "ddd",
        "Some normal line",
    };
    pushBackLines(lines);
    checkModel();
    min_element();
    checkModel();

    pushBack("0 as the lowest number here, will it be the lowest character?");
    checkModel();
    min_element();
    checkModel();

    pushBack("a normal line starting with letter a");
    checkModel();
    min_element();
    checkModel();
}

void TestYourApp::TestMaxElement() {
    const container lines = {
        "AAA",
        "bbb",
        "ddd",
        "Some normal line",
    };
    pushBackLines(lines);
    checkModel();
    max_element();
    checkModel();

    pushBack("9 as the highest number here, will it be the highest character?");
    checkModel();
    max_element();
    checkModel();

    pushBack("You are going to be a great programmer, aren't you?");
    checkModel();
    max_element();
    checkModel();
}


void TestYourApp::TestSort() {
    const container lines = {
        "BBB",
        "AAbA",
        "bbA",
        "a",
        "ddD",
        "DAA"
        };
    pushBackLines(lines);
    setCurrentElement(3);
    checkModel();
    sort();
    checkModel();
}


void TestYourApp::TestsOrT() {
    const container lines = {
        "BBB",
        "AAbA",
        "bbA",
        "a",
        "ddD",
        "DAA"
        };
    pushBackLines(lines);
    setCurrentElement(1);
    checkModel();
    sOrT();
    checkModel();
}

void TestYourApp::TestShuffle() {
    const container lines = {
        "BBB",
        "AAbA",
        "bbA",
        "a",
        "ddD",
        "DAA"
        };
    pushBackLines(lines);
    setCurrentElement(5);
    checkModel();
    shuffle(157);
    checkModel();
    shuffle(574);
    checkModel();
}

void TestYourApp::TestUnique() {
    const container lines = {
        "BBB",
        "AAbA",
        "bbA",
        "BBB",
        "a",
        "ddD",
        "a",
        "a",
        "AAD",
        "BBB",
        "DAA"
        };
    pushBackLines(lines);
    setCurrentElement(7);
    checkModel();
    sort();
    checkModel();
    unique();
    checkModel();
}

void TestYourApp::TestUniqueUnsorted() {
    const container lines = {
        "BBB",
        "AAbA",
        "bbA",
        "BBB",
        "a",
        "ddD",
        "a",
        "a",
        "AAD",
        "BBB",
        "DAA"
        };
    pushBackLines(lines);
    setCurrentElement(6);
    checkModel();
    unique();
    checkModel();
}

void TestYourApp::TestReverse() {
    const container lines = {
        "BBB",
        "AAbA",
        "bbA",
        "BBB",
        "a",
        "ddD",
        "a",
        "a",
        "AAD",
        "BBB",
        "DAA"
        };
    pushBackLines(lines);
    setCurrentElement(2);

    checkModel();
    reverse();
    checkModel();
    sort();
    checkModel();
    reverse();
    checkModel();
}

void TestYourApp::TestLowerBound() {
    const container unsorted_lines = {
        "BBB",
        "AAbA",
        "bbA",
        "BBB",
        "a",
        "ddD",
        "a",
        "a",
        "AAD",
        "BBB",
        "DAA"
    };

    pushBackLines(unsorted_lines);
    setCurrentElement(2);
    checkModel();
    lowerBound("dd");
    expected_content = "dd";
    checkModel();
    expected_content.clear();

    sort();
    checkModel();
    lowerBound("dd");
    checkModel();

    lowerBound("C");
    checkModel();
}

void TestYourApp::TestUpperBound() {
    const container unsorted_lines = {
        "BBB",
        "AAbA",
        "bbA",
        "BBB",
        "a",
        "ddD",
        "a",
        "a",
        "AAD",
        "BBB",
        "DAA"
    };

    pushBackLines(unsorted_lines);
    setCurrentElement(5);
    checkModel();
    upperBound("AAB");
    expected_content = "AAB";
    checkModel();
    expected_content.clear();

    sort();
    checkModel();
    upperBound("a");
    checkModel();
    upperBound("f");
    checkModel();
}



void TestYourApp::TestTeaTime() {
    clickWidget(btn_tea);
    deq = tea;
    it = deq.begin();
    checkModel();

    clickWidget(btn_cakes);
    deq = cakes;
    it = deq.begin();
    checkModel();
}


void TestYourApp::cleanup()
{
    delete window;
}

QTEST_MAIN(TestYourApp)

#include "tests.moc"
