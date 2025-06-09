#include <QtTest>
#include <QCoreApplication>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSlider>
#include <QVideoWidget>

#include <mainwindow.h>
#include <prac/QMediaPlayer>
#include <prac/QFileDialog>

#include "test_utils/practicum_assert.hpp"

#include <iostream>

using namespace std;

namespace {

template<class Widget>
Widget* FindElementByName(QMainWindow* window, const QString& name) {
    auto elements = window->findChildren<Widget*>();

    auto iter = std::find_if(elements.begin(), elements.end(), [&](Widget* elem){
        return elem->objectName() == name;
    });

    return iter == elements.end() ? nullptr : *iter;
}

}

template<typename T>
void getChild(T*& child, QObject* parent, const QString& object_name, const QString& type_name)
{

    child = parent->findChild<T *>(object_name);
    QString message;
    if (type_name != "")
    {
        message = QString("В %3 не найден %1 типа %2").arg(object_name).arg(type_name).arg(parent->objectName());
    }
    else
    {
        message = QString("В %2 не найден %1").arg(object_name).arg(parent->objectName());;
    }

    QVERIFY2(child, qPrintable(message));
}

template<typename T>
T* getChild(QObject* parent, const QString& object_name, const QString& type_name = {})
{
    T* child = nullptr;
    getChild<T>(child, parent, object_name, type_name);

    return child;
}


template<class T>
void findElementByText(T*& child, QObject* parent, const QString& label, const QString& type_name) {
    auto children = parent->findChildren<T*>();

    for (const auto& child_ : children)
    {
        if (child_->text() == label)
        {
            child = child_;
            return;
        }
    }

    QString message = QString("В %3 не найден %2 c текстом %1").arg(label).arg(type_name).arg(parent->objectName());
    QVERIFY2(child, qPrintable(message));
}


template<class T>
T* findElementByText(QObject* parent, const QString& label, const QString& type_name) {
    T* child = nullptr;
    findElementByText<T>(child, parent, label, type_name);
    return child;
}


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

    void TestConnectOutputs();
    void TestBtnChoose();
    void TestMediaStatusChanged();
    void TestPositionChanged();
    void TestBtnPause();
    void TestVolume();

private:
    MainWindow* window;

    QPushButton* btn_pause;
    QPushButton* btn_choose;

    QSlider* sld_pos;
    QSlider* sld_volume;

    QVideoWidget* video_output;
};

void TestYourApp::initTestCase() {
}

void TestYourApp::init()
{
    window = new MainWindow();
    QVERIFY2(window != nullptr, "Окно приложения не создано");
    window->show();
    QVERIFY2(window->isVisible(), "Окно приложения не активируется");

    btn_pause = getChild<QPushButton>(window, "btn_pause", "QPushButton");
    btn_choose = getChild<QPushButton>(window, "btn_choose", "QPushButton");

    sld_pos = getChild<QSlider>(window, "sld_pos", "QSlider");
    sld_volume = getChild<QSlider>(window, "sld_volume", "QSlider");

    video_output = getChild<QVideoWidget>(window, "video_output", "QVideoWidget");
}

void TestYourApp::TestConnectOutputs() {
    QVERIFY2(prac::QMediaPlayer::getAudioConnected(), "audio_output не подключен");
    QVERIFY2(prac::QMediaPlayer::getVideoConnected(), "video_output не подключен");
}

void TestYourApp::TestBtnChoose() {
    prac::QFileDialog::setMockedOpenFileName(QString("test_filename_for_test"));
    QTest::mouseClick(btn_choose, Qt::LeftButton);
    QVERIFY2(prac::QMediaPlayer::sourceIsSet(), "Источник медиа через setSource не установлен");
    PRAC_COMPARE2(prac::QMediaPlayer::getSource(), QUrl::fromLocalFile("test_filename_for_test"), "Источник медиа установлен неправильно");
    PRAC_COMPARE2(prac::QMediaPlayer::getState(), prac::QMediaPlayer::PlaybackState::PlayingState, "Не начато воспроизведение через play привыборе источника медиа");
}

void TestYourApp::TestMediaStatusChanged() {
    prac::QMediaPlayer::getLastCreated()->emitMediaStatusChanged(prac::QMediaPlayer::LoadedMedia);
    PRAC_COMPARE2(sld_pos->maximum(), 123, "После сигнала mediaStatusChanged установлена неправильная длительность");

    prac::QMediaPlayer::getLastCreated()->emitPositionChanged(50);
    PRAC_COMPARE2(sld_pos->value(), 50, "После сдвига изменения позиции воспроизведения должна измениться позиция слайдера");

    prac::QMediaPlayer::getLastCreated()->emitPositionChanged(100);
    PRAC_COMPARE2(sld_pos->value(), 100, "После сдвига изменения позиции воспроизведения должна измениться позиция слайдера");
}

void TestYourApp::TestPositionChanged() {
    sld_pos->setSliderPosition(33);
    PRAC_COMPARE2(prac::QMediaPlayer::getPosition(), 33, "Позиция в плеере не изменилась после сдвига слайдера sld_pos");
    sld_pos->setSliderPosition(47);
    PRAC_COMPARE2(prac::QMediaPlayer::getPosition(), 47, "Позиция в плеере не изменилась после сдвига слайдера sld_pos");
}

void TestYourApp::TestBtnPause() {
    prac::QMediaPlayer::getState() = prac::QMediaPlayer::PlaybackState::StoppedState;
    QTest::mouseClick(btn_pause, Qt::LeftButton);
    PRAC_COMPARE2(prac::QMediaPlayer::getPosition(), 0, "Позиция воспроизведения не установлена в 0 перед началом воспроизведения");
    PRAC_COMPARE2(prac::QMediaPlayer::getState(), prac::QMediaPlayer::PlaybackState::PlayingState, "Воспроизведение не начато");
    QTest::mouseClick(btn_pause, Qt::LeftButton);
    PRAC_COMPARE2(prac::QMediaPlayer::getState(), prac::QMediaPlayer::PlaybackState::PausedState, "Воспроизведение не поставлено на паузу");
    QTest::mouseClick(btn_pause, Qt::LeftButton);
    PRAC_COMPARE2(prac::QMediaPlayer::getState(), prac::QMediaPlayer::PlaybackState::PlayingState, "Воспроизведение не снято с паузы");
}

void TestYourApp::TestVolume() {
    sld_volume->setSliderPosition(25);
    PRAC_COMPARE2(prac::QMediaPlayer::getLastCreated()->audioOutput()->volume(), 0.25, "При установке слайдера громкости в позицию 25 нужно установить громкость 0.25");

    sld_volume->setSliderPosition(50);
    PRAC_COMPARE2(prac::QMediaPlayer::getLastCreated()->audioOutput()->volume(), 0.5, "При установке слайдера громкости в позицию 50 нужно установить громкость 0.5");
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
