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
#include <prac/QTimer>

#include <mainwindow.h>

#include <string>
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
    void init();
    void cleanup();

    void TestClick();
    void TestUpAllWindows();
    void TestChooseEmptyDir();
    void TestChooseDir();

private:
    void matchFiles(size_t index, string message = "Файл не соответствует ожидаемому"){
        QVERIFY2(
            image_dir.filePath(window->GetCurrentFile()) == image_dir.filePath(images[index]),
            message.c_str());
    };

    MainWindow* window;
    QPushButton* btn_left;
    QPushButton* btn_right;
    QAction* action_up_windows;
    QAction* action_use_resources;
    QAction* action_choose_dir;
    QMenu* menu;
    QString dir_path = ":/cats/images/";
    QDir image_dir;
    QStringList images;
    const prac::QTimer* timer;
};



void TestYourApp::initTestCase()
{
    QDir im_dir(dir_path);
    image_dir = im_dir; // Ask me what is going on, I won't tell you the answer...
    images = im_dir.entryList();
    QVERIFY2(images.size() > 1, "В заданной директории из файла ресурсов находится меньше двух файлов");

}

void TestYourApp::init()
{
    window = new MainWindow();
    QVERIFY2(window != nullptr, "Окно приложения не создано");
    window->show();
    QVERIFY2(window->isVisible(), "Окно приложения не активируется");

    btn_left = FindElementByName<QPushButton>(window, "btn_left");
    QVERIFY2(btn_left, "Не найден элемент btn_left класса QPushButton");

    btn_right = FindElementByName<QPushButton>(window, "btn_right");
    QVERIFY2(btn_right, "Не найден элемент btn_right класса QPushButton");

    action_up_windows = FindElementByName<QAction>(window, "action_up_windows");
    QVERIFY2(action_up_windows, "Не найден элемент action_up_windows класса QAction");

    action_use_resources = FindElementByName<QAction>(window, "action_use_resources");
    QVERIFY2(action_use_resources, "Не найден элемент action_use_resources класса QAction");

    action_choose_dir = FindElementByName<QAction>(window, "action_choose_dir");
    QVERIFY2(action_choose_dir, "Не найден элемент action_choose_dir класса QAction");

    menu = FindElementByName<QMenu>(window, "menu");
    QVERIFY2(menu, "Не найден элемент menu класса QMenu");
    QVERIFY2(!menu->isEmpty(), "menu пустое");

    timer = prac::QTimer::getLastCreated();

    window->SetFolder(dir_path);
}

void TestYourApp::TestClick(){
    matchFiles(0, "Файл по умолчанию не соответствует первому файлу в директории");
    for (auto i = 0; i < images.size() - 2; ++i){

        QTest::mouseClick(btn_right, Qt::LeftButton);
        matchFiles(i + 1);
    }
}

void TestYourApp::TestUpAllWindows() {
    QVERIFY2(window->windowFlags().testAnyFlag(Qt::WindowStaysOnTopHint), "Не выставлен флаг Qt::WindowStaysOnTopHint по умолчанию");
    action_up_windows->trigger();
    QVERIFY2(!(window->windowFlags().testAnyFlag(Qt::WindowStaysOnTopHint)), "Не убран флаг Qt::WindowStaysOnTopHint");
    action_up_windows->trigger();
    QVERIFY2(window->windowFlags().testAnyFlag(Qt::WindowStaysOnTopHint), "Не выставлен флаг Qt::WindowStaysOnTopHint");
}

void TestYourApp::TestChooseEmptyDir() {
    // меняем директорию на фальшивую из мока - там нет файлов
    // проверяем, что кнопки неактивны
    prac::QFileDialog::setMockedExistingDirectory(QString("there/is/no/such/directory"));
    action_choose_dir->trigger();
    QVERIFY2(!btn_left->isEnabled(), "Кнопа btn_left должна быть неактивна для пустой директории");
    QVERIFY2(!btn_right->isEnabled(), "Кнопа btn_right должна быть неактивна для пустой директории");
}

void TestYourApp::TestChooseDir() {
    // меняем директорию на фальшивую из мока - там нет файлов
    // при попытке сматчить файлы здесь окно рухнет, т.к. не обрабатывает пустые директории
    prac::QFileDialog::setMockedExistingDirectory(QString("there/is/no/such/directory"));
    action_choose_dir->trigger();

    // возвращаем директорию к нормальной (из ресурсов)
    prac::QFileDialog::setMockedExistingDirectory(dir_path);
    action_choose_dir->trigger();

    // проверяем, что всё в порядке с файлами
    matchFiles(0, "Файл по умолчанию не соответствует первому файлу в директории после смены директории на ложную и обратно");
    for (auto i = 0; i < images.size() - 2; ++i){

        QTest::mouseClick(btn_right, Qt::LeftButton);
        matchFiles(i + 1, "Файл не соответствует ожидаемому после смены директории на ложную и обратно");
    }
}

void TestYourApp::cleanup()
{
    delete window;
}

QTEST_MAIN(TestYourApp)

#include "tests.moc"
