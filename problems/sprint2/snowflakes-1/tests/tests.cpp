#include <winterwindow.h>

#include <QCoreApplication>
#include <QtTest>
#include <QThread>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QWidget>
#include <QtWidgets/QSlider>
#include <cmath>

#include "prac/QPainter"

#include "test_utils/practicum_assert.hpp"

using namespace std;


template <typename T>
void getChild(T*& child, QObject* parent, const QString& object_name, const QString& type_name) {
    child = parent->findChild<T*>(object_name);
    QString message;
    if (type_name != "") {
        message = QString("В %3 не найден %1 типа %2")
                      .arg(object_name)
                      .arg(type_name)
                      .arg(parent->objectName());
    } else {
        message = QString("В %2 не найден %1").arg(object_name).arg(parent->objectName());
    }

    QVERIFY2(child, qPrintable(message));
}

template <typename T>
T* getChild(QObject* parent, const QString& object_name, const QString& type_name = {}) {
    T* child = nullptr;
    getChild<T>(child, parent, object_name, type_name);

    return child;
}

class TestYourApp : public QObject {
    Q_OBJECT

public:
    TestYourApp() = default;
    ~TestYourApp() = default;

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();
    void TestDefault();
    void TestRotated();
    void TestResized();
    void TestThickened();
    void TestComplex();

private:
    void ValidateSnowflake();
    WinterWindow* window;
    QSlider* sld_size;
    QSlider* sld_rotation;
    QSlider* sld_line;
};

void TestYourApp::initTestCase() {

}


void TestYourApp::init() {
    window = new WinterWindow();
    QVERIFY2(window != nullptr, "Окно приложения не создано");
    window->show();
    QVERIFY2(window->isVisible(), "Окно приожения не активируется");
    QApplication::instance()->processEvents(); // Запуск обработки событий в фоновом режиме для тестирования

    sld_size = getChild<QSlider>(window, "sld_size", "QSlider");
    sld_rotation = getChild<QSlider>(window, "sld_rotation", "QSlider");
    sld_line = getChild<QSlider>(window, "sld_line", "QSlider");

}

QPointF RotatePoint(const QPointF& point, double angle)
{
    double angle_radian = angle / 180. * M_PI;
    double sin_ = std::sin(angle_radian);
    double cos_ = std::cos(angle_radian);

    double x = point.x() * cos_ + point.y() * sin_;
    double y = point.x() * (-sin_) + point.y() * cos_;

    return {x, y};
}

bool findSameLine(std::vector<prac::Line> lines, prac::Line line)
{
    for (const auto& candidate : lines)
    {
        if (candidate == line){
            return true;
        }
    }
    return false;
}

void checkPens(std::vector<QPen> pens, double line_width, double threshold = 1e-5)
{
    for(const auto& pen : pens)
    {
        bool sameness = std::abs(pen.widthF() - line_width) < threshold;
        QVERIFY2(sameness, "Толщина кисти не равна ожидаемой");
    }
}

void TestYourApp::ValidateSnowflake()
{
    QPointF center = {window->width() / 2., window->height() / 2.};
    double line_length = sld_size->value();
    double rotation = sld_rotation->value();
    double line_width = sld_line->value() / 15.;

    checkPens(prac::QPainter::pens, line_width);

    PRAC_COMPARE2(prac::QPainter::lines.size(), 6, "Количество линий, которые были нарисованы, отлично от шести");
    for (auto i = 0; i < 6; ++i)
    {
        QPointF const rotated = RotatePoint({line_length, 0}, rotation + i * 60);
        QPointF const end = center + rotated;

        bool line_is_found = findSameLine(prac::QPainter::lines, {center, end});
        QVERIFY2(line_is_found, "Как минимум один из ожидаемых лучей не был нарисован");
    }
}

void TestYourApp::TestDefault()
{
    ValidateSnowflake();
}

void TestYourApp::TestRotated()
{
    sld_rotation->setValue(12);
    ValidateSnowflake();

    sld_rotation->setValue(69);
    ValidateSnowflake();
}

void TestYourApp::TestResized()
{
    sld_size->setValue(57);
    ValidateSnowflake();

    sld_size->setValue(97);
    ValidateSnowflake();
}

void TestYourApp::TestThickened()
{
    sld_line->setValue(5);
    ValidateSnowflake();

    sld_line->setValue(15);
    ValidateSnowflake();
}

void TestYourApp::TestComplex()
{
    sld_rotation->setValue(5);
    sld_line->setValue(50);
    sld_size->setValue(70);
    ValidateSnowflake();

    sld_rotation->setValue(99);
    sld_line->setValue(1);
    sld_size->setValue(54);
    ValidateSnowflake();
}


void TestYourApp::cleanupTestCase() {
}

void TestYourApp::cleanup() {
    delete window;
}

QTEST_MAIN(TestYourApp)

#include "tests.moc"
