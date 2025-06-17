
// Core headers
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QWidget>

#include <QtWidgets/QPushButton>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>

#include <QtTest>
#include <QComboBox>
#include <QCheckBox>
#include <QDoubleSpinBox>
#include <iostream>

#include <algorithm>
#include <vector>
#include <set>
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

    void TestFill();
    void TestFillNotEmptyName();
    void TestWatched();
    void TestGenre();
    void TestRating();
    void TestUpdate();
    void TestUpdateFromWatched();
    void TestUpdateFromUnwatched();

private:
    void fill(const std::vector<QString>& values, bool watched = false, bool genre = false, bool rating = false);
    void checkLists(const std::vector<QString>& expected_wait, const std::vector<QString>& expected_done);

    MainWindow *window;

    QComboBox* cb_list_wait;
    QComboBox* cb_list_done;

    QLineEdit *le_name;

    QPushButton *pb_add;

    QCheckBox *chb_is_done;

    QComboBox* cb_genre;

    QDoubleSpinBox* dsb_rating;

    const std::vector<QString> test_values = {"Film1" , "Film2", "Film3", "Film4" , "Film5", "Film6"};
};


void TestYourApp::init()
{
    window = new MainWindow();

    QVERIFY2(window != nullptr, "Главное окно не создано");
    window->show();
    QVERIFY2(window->isVisible(), "Главное окно не активируется");

    cb_list_wait = getChild<QComboBox>(window, "cb_list_wait", "QComboBox");
    cb_list_done = getChild<QComboBox>(window, "cb_list_done", "QComboBox");

    le_name = getChild<QLineEdit>(window, "le_name", "QLineEdit");

    pb_add = getChildByText<QPushButton>(window, "Добавить / Редактировать", "QPushButton");

    chb_is_done = getChildByText<QCheckBox>(window, "Просмотрен", "QCheckBox");

    cb_genre = getChild<QComboBox>(window, "cb_genre", "QComboBox");

    dsb_rating = getChild<QDoubleSpinBox>(window, "dsb_rating", "QDoubleSpinBox");
}

void TestYourApp::fill(const std::vector<QString>& values, bool watched, bool genre, bool rating) {
    for (int i = 0; i < values.size(); ++i) {
        le_name->setText(values[i]);
        if (watched) {
            if (i % 2 == 0) {
                chb_is_done->setCheckState(Qt::Checked);
            } else {
                chb_is_done->setCheckState(Qt::Unchecked);
            }
        }
        if (genre) {
            cb_genre->setCurrentIndex(i);
        }
        if (rating) {
            dsb_rating->setValue(i + static_cast<double>(i % 3) / 10);
        }
        clickWidget(pb_add);
    }
}

void TestYourApp::checkLists(const std::vector<QString>& expected_wait, const std::vector<QString>& expected_done) {
    std::vector<QString> lines_wait;
    const auto size_wait = cb_list_wait->count();
    lines_wait.reserve(size_wait);
    for (int i = 0; i < size_wait; ++i) {
        cb_list_wait->setCurrentIndex(i);
        const auto& item = cb_list_wait->currentText();
        lines_wait.push_back(item);
    }
    std::vector<QString> lines_done;
    const auto size_done = cb_list_done->count();
    lines_done.reserve(size_done);
    for (int i = 0; i < size_done; ++i) {
        cb_list_done->setCurrentIndex(i);
        const auto& item = cb_list_done->currentText();
        lines_done.push_back(item);
    }

    PRAC_COMPARE2(lines_wait.size(), expected_wait.size(), "Размер списка непросмотренных фильмов не совпадает с ожидаемым!");
    PRAC_COMPARE2(lines_done.size(), expected_done.size(), "Размер списка просмотренных фильмов не совпадает с ожидаемым!");
    PRAC_COMPARE2(std::set<QString>(lines_wait.begin(), lines_wait.end()), std::set<QString>(expected_wait.begin(), expected_wait.end()), "Информация о непросмотренных фильмах не совпадает с ожидаемой!");
    PRAC_COMPARE2(std::set<QString>(lines_done.begin(), lines_done.end()), std::set<QString>(expected_done.begin(), expected_done.end()), "Информация о просмотренных фильмах не совпадает с ожидаемой!");
}

void TestYourApp::TestFill() {
    fill(test_values);

    std::vector<QString> expected_wait = {
        "Комедия 'Film1' рейтинг: 0",
        "Комедия 'Film2' рейтинг: 0",
        "Комедия 'Film3' рейтинг: 0",
        "Комедия 'Film4' рейтинг: 0",
        "Комедия 'Film5' рейтинг: 0",
        "Комедия 'Film6' рейтинг: 0"};
    std::vector<QString> expected_done = {};

    checkLists(expected_wait, expected_done);
}

void TestYourApp::TestFillNotEmptyName() {
    const std::vector<QString> values = {"Film1" , "", "Film3"};
    fill(values);

    std::vector<QString> expected_wait = {
        "Комедия 'Film1' рейтинг: 0",
        "Комедия 'Film3' рейтинг: 0"};
    std::vector<QString> expected_done = {};

    checkLists(expected_wait, expected_done);
}

void TestYourApp::TestWatched() {
    fill(test_values, true, false, false);

    std::vector<QString> expected_wait = {
        "Комедия 'Film2' рейтинг: 0",
        "Комедия 'Film4' рейтинг: 0",
        "Комедия 'Film6' рейтинг: 0"};
    std::vector<QString> expected_done = {
        "Комедия 'Film1' рейтинг: 0",
        "Комедия 'Film3' рейтинг: 0",
        "Комедия 'Film5' рейтинг: 0"};

    checkLists(expected_wait, expected_done);
}

void TestYourApp::TestGenre() {
    fill(test_values, false, true, false);

    std::vector<QString> expected_wait = {
        "Комедия 'Film1' рейтинг: 0",
        "Боевик 'Film2' рейтинг: 0",
        "Драма 'Film3' рейтинг: 0",
        "Сказка 'Film4' рейтинг: 0",
        "Мультфильм 'Film5' рейтинг: 0",
        "Ужасы 'Film6' рейтинг: 0"};
    std::vector<QString> expected_done = {};

    checkLists(expected_wait, expected_done);
}

void TestYourApp::TestRating() {
    fill(test_values, false, false, true);

    std::vector<QString> expected_wait = {
        "Комедия 'Film1' рейтинг: 0",
        "Комедия 'Film2' рейтинг: 1.1",
        "Комедия 'Film3' рейтинг: 2.2",
        "Комедия 'Film4' рейтинг: 3",
        "Комедия 'Film5' рейтинг: 4.1",
        "Комедия 'Film6' рейтинг: 5.2"};
    std::vector<QString> expected_done = {};

    checkLists(expected_wait, expected_done);
}

void TestYourApp::TestUpdate() {
    fill(test_values, true, true, true);

    std::vector<QString> expected_wait = {
        "Боевик 'Film2' рейтинг: 1.1",
        "Сказка 'Film4' рейтинг: 3",
        "Ужасы 'Film6' рейтинг: 5.2"};
    std::vector<QString> expected_done = {
        "Комедия 'Film1' рейтинг: 0",
        "Драма 'Film3' рейтинг: 2.2",
        "Мультфильм 'Film5' рейтинг: 4.1"};

    checkLists(expected_wait, expected_done);

    // this film will be updated
    le_name->setText("Film4");
    dsb_rating->setValue(7.6);
    cb_genre->setCurrentIndex(3);
    clickWidget(pb_add);

    // this film will be added as a new one
    le_name->setText("Film3");
    dsb_rating->setValue(9.5);
    clickWidget(chb_is_done);
    cb_genre->setCurrentIndex(4);
    clickWidget(pb_add);

    expected_wait = {
        "Боевик 'Film2' рейтинг: 1.1",
        "Ужасы 'Film6' рейтинг: 5.2",
        "Мультфильм 'Film3' рейтинг: 9.5"};
    expected_done = {
        "Комедия 'Film1' рейтинг: 0",
        "Драма 'Film3' рейтинг: 2.2",
        "Сказка 'Film4' рейтинг: 7.6",
        "Мультфильм 'Film5' рейтинг: 4.1"};

    checkLists(expected_wait, expected_done);
}

void TestYourApp::TestUpdateFromWatched() {
    fill(test_values, true, true, true);

    std::vector<QString> expected_wait = {
        "Боевик 'Film2' рейтинг: 1.1",
        "Сказка 'Film4' рейтинг: 3",
        "Ужасы 'Film6' рейтинг: 5.2"};
    std::vector<QString> expected_done = {
        "Комедия 'Film1' рейтинг: 0",
        "Драма 'Film3' рейтинг: 2.2",
        "Мультфильм 'Film5' рейтинг: 4.1"};

    checkLists(expected_wait, expected_done);

    cb_list_done->setCurrentIndex(1);
    dsb_rating->setValue(6.5);
    chb_is_done->setCheckState(Qt::Unchecked);
    clickWidget(pb_add);

    expected_wait = {
        "Боевик 'Film2' рейтинг: 1.1",
        "Сказка 'Film4' рейтинг: 3",
        "Драма 'Film3' рейтинг: 6.5",
        "Ужасы 'Film6' рейтинг: 5.2"};
    expected_done = {
        "Комедия 'Film1' рейтинг: 0",
        "Мультфильм 'Film5' рейтинг: 4.1"};
}

void TestYourApp::TestUpdateFromUnwatched() {
    fill(test_values, true, true, true);

    std::vector<QString> expected_wait = {
        "Боевик 'Film2' рейтинг: 1.1",
        "Сказка 'Film4' рейтинг: 3",
        "Ужасы 'Film6' рейтинг: 5.2"};
    std::vector<QString> expected_done = {
        "Комедия 'Film1' рейтинг: 0",
        "Драма 'Film3' рейтинг: 2.2",
        "Мультфильм 'Film5' рейтинг: 4.1"};

    checkLists(expected_wait, expected_done);

    cb_list_wait->setCurrentIndex(2);
    dsb_rating->setValue(7.9);
    chb_is_done->setCheckState(Qt::Checked);
    clickWidget(pb_add);

    expected_wait = {
        "Боевик 'Film2' рейтинг: 1.1",
        "Сказка 'Film4' рейтинг: 3"};
    expected_done = {
        "Комедия 'Film1' рейтинг: 0",
        "Драма 'Film3' рейтинг: 2.2",
        "Мультфильм 'Film5' рейтинг: 4.1",
        "Ужасы 'Film6' рейтинг: 7.9"};
}

void TestYourApp::cleanup()
{
    delete window;
}

QTEST_MAIN(TestYourApp)

#include "tests.moc"
