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

#include <mywindowwithimage.h>

#include "test_utils/practicum_assert.hpp"

#define MACRO_TO_STR_T_INNER(x) #x
#define MACRO_TO_STR_T(x) MACRO_TO_STR_T_INNER(x)

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
    void TestDefault();


private:
    MyWindowWithImage* window;
    QLabel* lbl_pixmap;
};

void TestYourApp::initTestCase()
{

}

void TestYourApp::init()
{
    window = new MyWindowWithImage();
    QVERIFY2(window != nullptr, "Окно приложения не создано");
    window->show();
    QVERIFY2(window->isVisible(), "Окно приложения не активируется");
    lbl_pixmap = FindElementByName<QLabel>(window, "lbl_pixmap");
    QVERIFY2(lbl_pixmap, "Не найден элемент lbl_pixmap класса QLabel");
}


void check_lbl_pixmap(QMainWindow* parent, QLabel* child)
{

    PRAC_COMPARE2(child->size(), parent->size(), "Размер виджета не равен размеру окна");
    PRAC_COMPARE2(child->pos(), QPoint(0, 0), "Виджет не находится по координатам (0,0)");

    QString image_path = MACRO_TO_STR_T(ILOVEQT_IMAGE_FULL_PATH);
    QPixmap* reference_pixmap = new QPixmap(image_path);
    PRAC_COMPARE2(child->pixmap().toImage(), reference_pixmap->toImage(), "В QLabel нет нужной картинки");
}

void TestYourApp::TestDefault()
{
    check_lbl_pixmap(window, lbl_pixmap);
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
