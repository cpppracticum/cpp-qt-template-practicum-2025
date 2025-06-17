#include <winterwindow.h>

#include <QtTest>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QWidget>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpinBox>
#include <cmath>

#include "prac/QPainter"

#include "test_utils/practicum_assert.hpp"

using namespace std;


class LineSet {
    template<class T>
    size_t CellsSum(const T& cells) const {
        size_t acc = 0;
        for (const auto& c : cells) {
            if (!c.has_value()) {
                continue;
            }
            auto iter = storage_.find(*c);
            if (iter == storage_.end()) {
                continue;
            }
            acc += iter->second.size();
        }
        return acc;
    }
public:
    LineSet(double eps, double step) : eps_(eps), step_(step) {}

    template<class T>
    void Set(T begin, T end) {
        for (;begin!=end; ++begin) {
            AddLine(*begin);
        }
    }

    using Cell = std::pair<int,int>;
    void AddLine(prac::Line l) {
        auto cells_to_add_start = GetCellsForPoint(l.start);
        auto cells_to_add_stop = GetCellsForPoint(l.start);

        auto cells_to_add = CellsSum(cells_to_add_start) < CellsSum(cells_to_add_stop) ? cells_to_add_start : cells_to_add_stop;

        for (const auto& cell : cells_to_add) {
            if (!cell.has_value()) {
                continue;
            }
            storage_[*cell].push_back(l);
        }
    }

    bool HasLine(prac::Line l) const {
        return HasLine(l, l.start) || HasLine(l, l.stop);
    }

    bool HasLine(prac::Line l, QPointF loockup_point) const {
        auto iter = storage_.find(GetCell(loockup_point));
        if (iter == storage_.end()) {
            return false;
        }
        const auto& vec = iter->second;
        return std::find(vec.begin(), vec.end(), l) != vec.end();
    }

    Cell GetCell(QPointF p) const {
        return {int(p.x() / step_), int(p.y() / step_)};
    }

    std::array<std::optional<Cell>, 4> GetCellsForPoint(QPointF p) const {
        std::array<Cell, 4> cells = {GetCell(p + QPointF(-eps_, -eps_)), GetCell(p + QPointF(-eps_, eps_)), GetCell(p + QPointF(eps_, -eps_)), GetCell(p + QPointF(eps_, eps_))};
        std::sort(cells.begin(), cells.end());
        auto real_end = std::unique(cells.begin(), cells.end());
        std::array<std::optional<Cell>, 4> result;
        std::copy(cells.begin(), real_end, result.begin());
        return result;
    }

private:
    double eps_;
    double step_;
    std::map<Cell, std::vector<prac::Line>> storage_;
};


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
        ;
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
    void TestFactor();
    void TestDepth();
    void TestComplex();

private:
    void ValidateSnowflake();
    WinterWindow* window;
    QSlider* sld_size;
    QSlider* sld_rotation;
    QSlider* sld_line;
    QSlider* sld_factor;
    QSpinBox* spin_depth;
};

void TestYourApp::initTestCase() {

}


void TestYourApp::init() {
    window = new WinterWindow();
    QVERIFY2(window != nullptr, "Окно приложения не создано");
    window->show();
    QVERIFY2(window->isVisible(), "Окно приложения не активируется");
    QApplication::instance()->processEvents(); // Запуск обработки событий в фоновом режиме для тестирования

    sld_size = getChild<QSlider>(window, "sld_size", "QSlider");
    sld_rotation = getChild<QSlider>(window, "sld_rotation", "QSlider");
    sld_line = getChild<QSlider>(window, "sld_line", "QSlider");
    sld_factor = getChild<QSlider>(window, "sld_factor", "QSlider");
    spin_depth = getChild<QSpinBox>(window, "spin_depth", "QSpinBox");
}

QPointF RotatePoint(const QPointF& point, double angle)
{
    const double angle_radian = angle / 180. * M_PI;
    const double sin_ = std::sin(angle_radian);
    const double cos_ = std::cos(angle_radian);

    double x = point.x() * cos_ + point.y() * sin_;
    double y = point.x() * (-sin_) + point.y() * cos_;

    return {x, y};
}


bool findSameLine(const std::vector<prac::Line>& lines, prac::Line line)
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

std::vector<prac::Line> getRecursiveSnowflake(
    const QPointF& center,
    double line_length,
    double rotation,
    double line_width,
    double factor,
    int depth)
{
    if (depth < 0)
    {
        return {};
    }
    std::vector<prac::Line> expected_lines;

    const double scaled_length = line_length * factor;
    const double next_depth = depth - 1;
    for (auto i = 0; i < 6; ++i)
    {
        QPointF const rotated = RotatePoint({line_length, 0}, rotation + i * 60);
        QPointF const end = center + rotated;
        expected_lines.emplace_back(center, end);

        std::vector<prac::Line> recursive_expected_lines = getRecursiveSnowflake(
            end, scaled_length, rotation, line_width, factor, next_depth);

        expected_lines.insert(expected_lines.end(), recursive_expected_lines.begin(), recursive_expected_lines.end());
    }
    return expected_lines;
}


void TestYourApp::ValidateSnowflake()
{
    QPointF center = {window->width() / 2., window->height() / 2.};
    const double line_length = sld_size->value();
    const double rotation = sld_rotation->value();
    const double line_width = sld_line->value() / 15.;
    const double factor = sld_factor->value() / double(sld_factor->maximum());
    const double depth = spin_depth->value();

    checkPens(prac::QPainter::pens, line_width);

    auto expected_lines = getRecursiveSnowflake(center, line_length, rotation, line_width, factor, depth);

    LineSet set(1e-5, 0.1);
    set.Set(prac::QPainter::lines.begin(), prac::QPainter::lines.end());

    PRAC_COMPARE2(prac::QPainter::lines.size(), expected_lines.size(), "Количество линий, которые были нарисовано, не совпадает с ожидаемым");
    for (const auto& line: expected_lines)
    {
        bool line_is_found = set.HasLine(line);
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

void TestYourApp::TestFactor()
{
    sld_factor->setValue(sld_factor->maximum() / 20);
    ValidateSnowflake();

    sld_factor->setValue(sld_factor->maximum() * 0.96);
    ValidateSnowflake();
}

void TestYourApp::TestDepth()
{
    spin_depth->setValue(2);
    sld_size->setValue(2);
    ValidateSnowflake();

    spin_depth->setValue(5);
    sld_size->setValue(3);
    ValidateSnowflake();
}

void TestYourApp::TestComplex()
{
    spin_depth->setValue(3);
    sld_factor->setValue(sld_factor->maximum() / 3);

    sld_rotation->setValue(5);
    sld_line->setValue(50);
    sld_size->setValue(70);

    ValidateSnowflake();

    spin_depth->setValue(4);
    sld_factor->setValue(sld_factor->maximum() / 12);
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
