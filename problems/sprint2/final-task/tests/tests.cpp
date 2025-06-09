#include <QtTest>

#include <QCoreApplication>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QComboBox>

#include <QtWidgets/QWidget>
#include <QtWidgets/QLabel>

#include <mainwindow.h>
#include <calculator.h>

#include <QtTest/QTest>

#include "test_utils/practicum_assert.hpp"

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

    void TestSimpleOperations();
    void TestAdvancedOperations();
    void TestMemory();


private:

    void parseKeyboard();
    void pushButton(const std::string& label);
    void inputNumber(const std::string& number);

    void checkLabels(const QString& expected_memory, const QString& expected_result, const QString& expected_formula);

    MainWindow* window;
    std::map<std::string, QPushButton*> keyboard;
    std::map<std::string, std::string> operations;
    QLabel* l_memory;
    QLabel* l_result;
    QLabel* l_formula;
};


void TestYourApp::parseKeyboard()
{
    auto buttons =
    {
        "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0",
        "+", "−", "÷", "×", "±", "=", "xʸ", "⌫", ".",
        "C", "MC", "MR", "MS"
    };
    operations = {
        {"+", "+"},
        {"-", "−"},
        {"/", "÷"},
        {"*", "×"},
        {"^", "xʸ"},
        {"+-", "±"}
    };

    for (const auto& label: buttons)
    {
        this->keyboard[label] = findElementByText<QPushButton>(window, label, "QPushButton");
    }
}

void TestYourApp::pushButton(const std::string& label)
{
    auto* button_ptr = keyboard.at(label);
    QTest::mouseClick(button_ptr, Qt::MouseButton::LeftButton);
}

void TestYourApp::inputNumber(const std::string& number)
{
    for( const auto & ch : number)
    {
        pushButton({ch});
    }
}


void TestYourApp::initTestCase()
{

}



void TestYourApp::init()
{
    window = new MainWindow();
    QVERIFY2(window != nullptr, "Окно приложения не создано");
    window->show();
    QVERIFY2(window->isVisible(), "Окно приложения не активируется");

    parseKeyboard();

    l_memory  = getChild<QLabel>(window, "l_memory",  "QLabel");
    l_result  = getChild<QLabel>(window, "l_result",  "QLabel");
    l_formula = getChild<QLabel>(window, "l_formula", "QLabel");


}

void TestYourApp::checkLabels(
    const QString& expected_memory,
    const QString& expected_result,
    const QString& expected_formula)
{
    auto memory = l_memory->text();
    auto result = l_result->text();
    auto formula = l_formula->text();

    PRAC_COMPARE2(memory, expected_memory, "Текст l_memory не совпадает с ожидаемым");
    PRAC_COMPARE2(result, expected_result, "Teкст l_result не совпадает с ожидаемым");
    PRAC_COMPARE2(formula, expected_formula, "Текст l_formula не совпадает с ожидаемым");
}


void TestYourApp::TestSimpleOperations()
{

    auto test =
        [&](const std::string& operand_a,
            const std::string& operation,
            const std::string& operand_b,
            const std::string& expected_result)
    {
        auto qt_operation = operations[operation];
        inputNumber(operand_a);
        pushButton(qt_operation);
        inputNumber(operand_b);
        pushButton("=");
        std::string expected_formula = operand_a + " " + qt_operation + " " + operand_b + " =";
        checkLabels("", expected_result.c_str(), expected_formula.c_str());
        pushButton("C");
    };

    test("156", "+", "12", "168");
    test("5", "*", "3", "15");
    test("36", "/", "12", "3");
    test("258", "-", "164", "94");

    {
        inputNumber("2");
        pushButton(operations["^"]);
        inputNumber("100");
        pushButton("=");
        pushButton(operations["/"]);
        pushButton("=");
        PRAC_COMPARE2(l_result->text(), "1", "Текст l_result не совпадает с ожидаемым");
    }
}


void TestYourApp::TestAdvancedOperations()
{
    {
        inputNumber("156");
        checkLabels("", "156", "");
        pushButton(operations["+-"]);
        checkLabels("", "-156", "");
    }

    pushButton("C");

    {
        inputNumber("2");
        checkLabels("", "2", "");
        pushButton(operations["^"]);
        inputNumber("8");
        pushButton("=");
        checkLabels("", "256", "2 ^ 8 =");
    }

    pushButton("C");

    {
        inputNumber("12");
        pushButton(operations["+-"]);
        pushButton(operations["*"]);
        inputNumber("12");
        pushButton("=");
        checkLabels("", "-144", "-12 × 12 =");
    }

    pushButton("C");
}


void TestYourApp::TestMemory()
{
    {
        inputNumber("254");
        pushButton("MS");
        checkLabels("M", "254", "");
        pushButton("C");
        checkLabels("M", "0", "");
        pushButton("MR");
        checkLabels("M", "254", "");
        pushButton("MC");
        checkLabels("", "254", "");
    }

    pushButton("C");

    {
        inputNumber("15");
        pushButton("+");
        inputNumber("12");
        pushButton("=");
        pushButton("MS");
        checkLabels("M", "27", "15 + 12 =");
        pushButton("C");
        pushButton("MR");
        pushButton(operations["-"]);
        inputNumber("15");
        pushButton("=");
        checkLabels("M", "12", ("27 " + operations["-"] + " 15 =").c_str());
        pushButton("MC");
    }

    pushButton("C");

    {
        inputNumber("2");
        pushButton(operations["^"]);
        inputNumber("100");
        pushButton("=");
        pushButton("MS");
        inputNumber("2");
        pushButton(operations["^"]);
        inputNumber("100");
        pushButton("=");
        pushButton(operations["/"]);
        pushButton("MR");
        pushButton("=");
        PRAC_COMPARE2(l_result->text(), "1", "Текст l_result не совпадает с ожидаемым");
    }
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
