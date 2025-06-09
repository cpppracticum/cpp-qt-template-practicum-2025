#include <mainwindow.h>

#include <QAction>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMenu>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QWidget>

#include <prac/QDateTime>
#include <prac/QFileDialog>
#include <prac/QMediaPlayer>
#include <prac/QTime>
#include <prac/QTimer>
#include <test_utils/core.hpp>


using namespace std;

namespace {

void UpdateTime(const QDateTime& datetime) {
    prac::QDateTime::currentDateTime() = datetime;
    prac::QTime::currentTime() = datetime.time();
}

QString TimeToStringFromPrecode(const QTime& time) {
    return QString("%1:%2:%3").arg(time.hour(), 2, 10, QChar('0')).arg(time.minute(), 2, 10, QChar('0')).arg(time.second(), 2, 10, QChar('0'));
}

QString TimeToStringFromPrecode(const std::chrono::milliseconds& ms) {
    return TimeToStringFromPrecode(QTime::fromMSecsSinceStartOfDay(ms.count() + 500));
}

}


class TestYourApp : public QObject
{
    Q_OBJECT

public:
    TestYourApp()=default;
    ~TestYourApp()=default;

private slots:
    void init();
    void cleanup();

    void TestTimer();
    void TestTimerNext();

    void TestTimeDisplaying();
    void TestTimeDisplayingNext();

    void TestTodayAlarmMoment();
    void TestTomorrowAlarmMoment();

    void TestRemainingTimeDisplayingToday();
    void TestRemainingTimeDisplayingTodayNext();
    void TestRemainingTimeDisplayingTomorrow();

    void TestAlarmStartStop();

    void TestSetMelody();

    void TestAlarm();

private:

    void SetAlarmTime(const QTime& alarm_time);

    MainWindow* window;
    prac::QTimer* timer;
    prac::QMediaPlayer* player;


    QLabel* lbl_now;
    QLabel* lbl_timeout;
    QLabel* lbl_melody;
    QPushButton* pb_start_stop;
    QSpinBox* sb_hour;
    QSpinBox* sb_min;
    QSpinBox* sb_sec;
    QAction* action_load_file;

    const QDateTime initial_time{QDate{2025, 1, 19}, QTime{12, 9, 46, 275}};

};

void TestYourApp::SetAlarmTime(const QTime& alarm_time) {
    const auto& hours = alarm_time.hour();
    const auto& minutes = alarm_time.minute();
    const auto& seconds = alarm_time.second();
    sb_hour->setValue(hours);
    sb_min->setValue(minutes);
    sb_sec->setValue(seconds);
}


void TestYourApp::init()
{
    window = new MainWindow();
    QVERIFY2(window != nullptr, "Окно приложения не создано");
    window->show();
    QVERIFY2(window->isVisible(), "Окно приложения не активируется");

    timer = prac::QTimer::getLastCreated();
    QVERIFY2(timer, "Не найден таймер");

    player = prac::QMediaPlayer::getLastCreated();
    QVERIFY2(player, "Не найден плеер");


    lbl_now = getChild<QLabel>(window, "lbl_now", "QLabel");
    lbl_timeout = getChild<QLabel>(window, "lbl_timeout", "QLabel");
    lbl_melody = getChild<QLabel>(window, "lbl_melody", "QLabel");

    pb_start_stop = getChild<QPushButton>(window, "pb_start_stop", "QPushButton");

    sb_hour = getChild<QSpinBox>(window, "sb_hour", "QSpinBox");
    sb_min = getChild<QSpinBox>(window, "sb_min", "QSpinBox");
    sb_sec = getChild<QSpinBox>(window, "sb_sec", "QSpinBox");

    action_load_file = getChild<QAction>(window, "action_load_file", "QAction");


    UpdateTime(initial_time);

    // Should update timer with current time
    timer->emitTimeout();
}

void TestYourApp::TestTimer() {
    const auto interval = timer->getInterval();
    const auto expected_interval = 1000 - initial_time.time().msec();
    PRAC_COMPARE(interval, expected_interval);
}

void TestYourApp::TestTimerNext() {
    auto actual_interval = timer->getInterval();
    const auto expected_interval_1 = 1000 - initial_time.time().msec();
    PRAC_COMPARE(actual_interval, expected_interval_1);

    // Set new time and emit timeout to initiate update
    const auto new_time = initial_time.addMSecs(576).addSecs(12);
    UpdateTime(new_time);
    timer->emitTimeout();

    actual_interval = timer->getInterval();
    const auto expected_interval_2 = 1000 - new_time.time().msec();
    PRAC_COMPARE(actual_interval, expected_interval_2);
}


void TestYourApp::TestTimeDisplaying() {
    const QString displaying_time = lbl_now->text();
    const auto expected_time = TimeToStringFromPrecode(initial_time.time());

    PRAC_COMPARE(displaying_time, expected_time);
}

void TestYourApp::TestTimeDisplayingNext() {
    QString displaying_time = lbl_now->text();
    const auto expected_time_1 = TimeToStringFromPrecode(initial_time.time());
    PRAC_COMPARE(displaying_time, expected_time_1);

    // Set new time and emit timeout to initiate update
    const auto new_time = prac::QTime{15, 27, 14, 976};
    UpdateTime(prac::QDateTime{initial_time.date(), new_time});
    timer->emitTimeout();

    displaying_time = lbl_now->text();
    const auto expected_time_2 = TimeToStringFromPrecode(new_time);
    PRAC_COMPARE(displaying_time, expected_time_2);
}


void TestYourApp::TestTodayAlarmMoment() {

    const QTime alarm_time{12, 10, 12};

    SetAlarmTime(alarm_time);
    clickWidget(pb_start_stop);

    const auto alarm_moment = window->GetAlarmMoment();

    const QDateTime expected_time{initial_time.date(), alarm_time};

    PRAC_COMPARE(alarm_moment, expected_time);
}

void TestYourApp::TestTomorrowAlarmMoment() {

    const QTime alarm_time{9, 10, 12};

    SetAlarmTime(alarm_time);
    clickWidget(pb_start_stop);

    const auto alarm_moment = window->GetAlarmMoment();
    qInfo() << alarm_moment;
    const QDateTime expected_time = QDateTime{initial_time.date(), alarm_time}.addDays(1);

    PRAC_COMPARE(alarm_moment, expected_time);
}


void TestYourApp::TestRemainingTimeDisplayingToday() {

    const QTime alarm_time{12, 10, 12};

    SetAlarmTime(alarm_time);
    clickWidget(pb_start_stop);

    const auto alarm_moment = window->GetAlarmMoment();
    const auto elapsed_time = alarm_moment - initial_time;

    const auto expected_elapsed_string = TimeToStringFromPrecode(elapsed_time);

    const auto actual_elapsed_string = lbl_timeout->text();

    PRAC_COMPARE(actual_elapsed_string, expected_elapsed_string);
}

void TestYourApp::TestRemainingTimeDisplayingTodayNext() {

    const QTime alarm_time{21, 19, 7};
    SetAlarmTime(alarm_time);
    clickWidget(pb_start_stop);
    const auto alarm_moment = window->GetAlarmMoment();
    const auto elapsed_time_1 = alarm_moment - initial_time;
    const auto expected_elapsed_string_1 = TimeToStringFromPrecode(elapsed_time_1);
    auto actual_elapsed_string = lbl_timeout->text();
    PRAC_COMPARE(actual_elapsed_string, expected_elapsed_string_1);

    // Set new time and emit timeout to initiate update
    const auto new_time = prac::QTime{15, 27, 14, 976};
    const auto new_datetime = prac::QDateTime{initial_time.date(), new_time};
    UpdateTime(new_datetime);
    timer->emitTimeout();

    const auto elapsed_time_2 = alarm_moment - new_datetime;
    const auto expected_elapsed_string_2 = TimeToStringFromPrecode(elapsed_time_2);
    actual_elapsed_string = lbl_timeout->text();
    PRAC_COMPARE(actual_elapsed_string, expected_elapsed_string_2);
}

void TestYourApp::TestRemainingTimeDisplayingTomorrow() {

    const QTime alarm_time{9, 10, 12};

    SetAlarmTime(alarm_time);
    clickWidget(pb_start_stop);

    const auto alarm_moment = window->GetAlarmMoment();
    const auto elapsed_time = alarm_moment - initial_time;

    const auto expected_elapsed_string = TimeToStringFromPrecode(elapsed_time);

    const auto actual_elapsed_string = lbl_timeout->text();

    PRAC_COMPARE(actual_elapsed_string, expected_elapsed_string);
}


void TestYourApp::TestAlarmStartStop() {

    const QTime alarm_time{12, 10, 12};
    SetAlarmTime(alarm_time);

    auto pb_text = pb_start_stop->text();
    auto lbl_text = lbl_timeout->text();
    auto alarm_moment = window->GetAlarmMoment();

    PRAC_COMPARE(pb_text, "Старт");
    PRAC_COMPARE(lbl_text, "Установите будильник");
    PRAC_COMPARE(alarm_moment, QDateTime{});

    clickWidget(pb_start_stop);

    pb_text = pb_start_stop->text();

    PRAC_COMPARE(pb_text, "Стоп");

    clickWidget(pb_start_stop);

    pb_text = pb_start_stop->text();
    lbl_text = lbl_timeout->text();
    alarm_moment = window->GetAlarmMoment();

    PRAC_COMPARE(pb_text, "Старт");
    PRAC_COMPARE(lbl_text, "Установите будильник");
    PRAC_COMPARE(alarm_moment, QDateTime{});
}


void TestYourApp::TestSetMelody() {

    const QString melody_name = "alarm_melody.mp3";
    prac::QFileDialog::setMockedOpenFileName(melody_name);

    action_load_file->trigger();

    PRAC_COMPARE(lbl_melody->text(), melody_name);
    PRAC_COMPARE2(
        player->getSource(),
        "file:" + melody_name,
        "Если у вас отсутствует префикс \"file:\", используйте QUrl::fromLocalFile в методе setSource"
    );
}


void TestYourApp::TestAlarm() {

    const QTime alarm_time{12, 10, 12};
    SetAlarmTime(alarm_time);
    clickWidget(pb_start_stop);


    auto player_state = player->getState();
    QVERIFY(player_state != QMediaPlayer::PlayingState);

    const auto alarm_moment = window->GetAlarmMoment().addMSecs(10);

    UpdateTime(alarm_moment);

    timer->emitTimeout();

    player_state = player->getState();

    PRAC_COMPARE(lbl_timeout->text(), "Пора вставать");
    PRAC_COMPARE(player_state, QMediaPlayer::PlayingState);
}


void TestYourApp::cleanup()
{
    delete window;
}

QTEST_MAIN(TestYourApp)

#include "tests.moc"
