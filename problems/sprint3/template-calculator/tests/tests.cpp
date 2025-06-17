
// Core headers
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLabel>
#include <QtWidgets/QComboBox>
#include <QtTest>

#include <vector>

#include "mainwindow.h"

#include "utils.h"
#include "calculator_tests.h"
#include "calculator.h"
#include "controller.h"

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

    static void TestCalculatorFloatingPoint();
    static void TestCalculatorInteger();
    static void TestCalculatorRational();

    static void TestCalculatorZeroDivisionError();
    static void TestCalculatorZeroPowerToZeroError();
    static void TestCalculatorFractionalPowerError();
    static void TestCalculatorIntegerNegativePower();


    void TestInterfaceIntegers();
    void TestInterfaceFloatingPoint();
    void TestInterfaceRationals();

    void TestInterfaceZeroDivisionError();
    void TestInterfaceZeroPowerToZeroError();
    void TestInterfaceFractionalPowerError();
    void TestInterfaceIntegerNegativePower();

private:
    void SetupControllers();

    void parseKeyboard();
    void pushButton(const std::string& label);
    void inputNumber(const std::string& number);

    void checkFormula(const std::string& operand_1, const std::string& operation, const std::string& operand_2, bool with_equals = true);
    void checkResult(const QString& expected_result) const;
    void checkExtraButton(const QString& type);

    void testOperation(const QString& type,
        const std::string& operand_1,
        const std::string& operation,
        const std::string& operand_2,
        const QString& expected_result);

    void testErrorCode(
        const QString& type,
        const std::string& operand_1,
        const std::string& operation,
        const std::string& operand_2,
        CalculatorTests::ERROR_CODES expected_error_code);

    void chooseType(const QString& type) const;

    MainWindow* window;

    std::map<std::string, QPushButton*> keyboard;
    std::map<std::string, std::string> operations;

    QComboBox* cmb_controller;
    QPushButton* tb_extra;
    QLabel* l_memory;
    QLabel* l_result;
    QLabel* l_formula;

    std::string initial_hex_color;

};

void TestYourApp::SetupControllers() {
    static Controller<double> double_controller;
    static Controller<int> int_controller;
    static Controller<float> float_controller;
    static Controller<size_t> size_t_controller;
    static Controller<int64_t> int64_t_controller;
    static Controller<std::uint8_t> byte_controller;
    static Controller<Rational> rational_controller;
    window->SetControllerCallback([&](ControllerType controller) {
        switch(controller) {
        case ControllerType::DOUBLE:
            double_controller.BindWithMainWindow(window);
            break;
        case ControllerType::FLOAT:
            float_controller.BindWithMainWindow(window);
            break;
        case ControllerType::INT:
            int_controller.BindWithMainWindow(window);
            break;
        case ControllerType::INT64_T:
            int64_t_controller.BindWithMainWindow(window);
            break;
        case ControllerType::SIZE_T:
            size_t_controller.BindWithMainWindow(window);
            break;
        case ControllerType::UINT8_T:
            byte_controller.BindWithMainWindow(window);
            break;
        case ControllerType::RATIONAL:
            rational_controller.BindWithMainWindow(window);
            break;
        }
    });
    double_controller.BindWithMainWindow(window);
}

void TestYourApp::parseKeyboard()
{
    auto buttons =
    {
        "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0",
        "+", "−", "÷", "×", "±", "=", "xʸ", "⌫",
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

    if (label == "." || label == "/") {
        clickWidget(tb_extra);
    } else
    if (operations.contains(label)) {
        auto* button_ptr = keyboard.at(operations[label]);
        clickWidget(button_ptr);
    }
    else if(label != " "){
        auto* button_ptr = keyboard.at(label);
        clickWidget(button_ptr);
    }

}

void TestYourApp::inputNumber(const std::string& number)
{
    auto input = [this](const std::string& number) {
        for( const auto & ch : number)
        {
            pushButton({ch});
        }
    };

    if (number.at(0) == '-') {
        input(number.substr(1, number.length() - 1));
        pushButton("+-");
    } else {
        input(number);
    }
}

void TestYourApp::checkExtraButton(const QString& type)
{
    if (type == "double" || type == "float") {
        QVERIFY2(tb_extra->isVisible(), "Extra button should be visible for floating point types");
        PRAC_COMPARE2(tb_extra->text(), ".",
            "Text on extra button for floating point types should be \".\"");
    } else if (type == "Rational") {
        QVERIFY2(tb_extra->isVisible(), "Extra button should be visible for Rational type");
        PRAC_COMPARE2(tb_extra->text(), "/",
            "Text on extra button for Rational type should be \"/\"");
    }
    else {
        QVERIFY2(!tb_extra->isVisible(), "Extra button should not be visible for integer types");
    }
}

void TestYourApp::checkFormula(const std::string& operand_1, const std::string& operation, const std::string& operand_2, bool with_equals)
{

    auto expected_formula =
        QString::fromStdString( operand_1 + " " +
        (operation == "^" ? operation : operations[operation]) + " " +
        operand_2);

    if (with_equals) {
        expected_formula +=" =";
    }
    PRAC_COMPARE(l_formula->text().trimmed(), expected_formula.trimmed());
}

void TestYourApp::checkResult(const QString& expected_result) const {
    PRAC_COMPARE(l_result->text().trimmed(), expected_result.trimmed());
}

void TestYourApp::chooseType(const QString& type) const {
    const int index = cmb_controller->findText(type);
    QVERIFY2(index >= 0, "Expected type not found");
    cmb_controller->setCurrentIndex(index);
}


void TestYourApp::init()
{
    window = new MainWindow();

    QVERIFY2(window != nullptr, "Окно настроек не создано");


    cmb_controller = getChild<QComboBox>(window, "cmb_controller", "QComboBox");
    tb_extra = getChild<QPushButton>(window, "tb_extra", "QPushButton");

    parseKeyboard();

    window->show();
    QVERIFY2(window->isVisible(), "Окно приложения не активируется");

    l_memory  = getChild<QLabel>(window, "l_memory",  "QLabel");
    l_result  = getChild<QLabel>(window, "l_result",  "QLabel");
    l_formula = getChild<QLabel>(window, "l_formula", "QLabel");

    SetupControllers();
    initial_hex_color = l_result->palette().text().color().name().toStdString();
}


void TestYourApp::TestCalculatorFloatingPoint()
{
    CalculatorTests::testFloatingPoint<float>();
    CalculatorTests::testFloatingPoint<double>();
}

void TestYourApp::TestCalculatorInteger()
{
    CalculatorTests::testIntegerPoint<uint8_t>({105, 55, 2, 3, 230, 2});
    CalculatorTests::testIntegerPoint<int>({105, -55, -2, 3, -230, 2});
    CalculatorTests::testIntegerPoint<int64_t>({1505, 125, -2, 33, 157, 3});
    CalculatorTests::testIntegerPoint<size_t>({1000, 250, 5, 15, 30, 4});
}

void TestYourApp::TestCalculatorRational()
{
    CalculatorTests::testRational();
}


void TestYourApp::TestCalculatorZeroDivisionError()
{
    CalculatorTests::testZeroDivisionError<int>(-541453314);
    CalculatorTests::testZeroDivisionError<uint8_t>(15);
    CalculatorTests::testZeroDivisionError<size_t>(31230952350998);
    CalculatorTests::testZeroDivisionError<int64_t>(-43298578763487265);
    CalculatorTests::testZeroDivisionError<Rational>({15, 8});
}


void TestYourApp::TestCalculatorZeroPowerToZeroError() {
    CalculatorTests::testZeroPowerToZero<int>();
    CalculatorTests::testZeroPowerToZero<int64_t>();
    CalculatorTests::testZeroPowerToZero<int8_t>();
    CalculatorTests::testZeroPowerToZero<size_t>();
    CalculatorTests::testZeroPowerToZero<Rational>();
}


void TestYourApp::TestCalculatorFractionalPowerError()
{
    CalculatorTests::testFractionalPower<Rational>();
}


void TestYourApp::TestCalculatorIntegerNegativePower()
{
    CalculatorTests::testIntegerNegativePowerError<int>();
    CalculatorTests::testIntegerNegativePowerError<int64_t>();
}


void TestYourApp::testOperation(const QString& type,
    const std::string& operand_1,
    const std::string& operation,
    const std::string& operand_2,
    const QString& expected_result) {
    chooseType(type);
    checkExtraButton(type);
    pushButton("C");
    inputNumber(operand_1);
    pushButton(operations[operation]);
    inputNumber(operand_2);
    pushButton("=");
    checkFormula(operand_1, operation, operand_2, true);
    checkResult(expected_result);
}

void TestYourApp::TestInterfaceIntegers()
{
    testOperation("int",  "1000", "-", "8000", "-7000");
    testOperation("uint8_t", "12", "+", "5", "17");
    testOperation("size_t", "12409098124", "*", "157", "1948228405468");
    testOperation("int64_t", "8764861823467", "/", "-357", "-24551433679");
    testOperation("int64_t", "2", "^", "57", "144115188075855872");
}

void TestYourApp::TestInterfaceFloatingPoint() {
    testOperation("double", "12.5", "+", "3.2", "15.7");
    testOperation("float", "2.51", "-", "765.27", "-762.76");
    testOperation("double", "10", "^", "10", "1e+10");
    testOperation("float", "10", "/", "100", "0.1");
    testOperation("double", "-15.04", "*", "-875.3", "13164.5");
}


void TestYourApp::TestInterfaceRationals() {
    testOperation("Rational", "1 / 2", "*", "3", "3 / 2");
    testOperation("Rational", "1 / 2", "/", "3", "1 / 6");
    testOperation("Rational", "6 / 5", "^", "3", "216 / 125");
    testOperation("Rational", "3", "/", "27", "1 / 9");
    testOperation("Rational", "4 / 757", "+", "21347 / 5874", "16183175 / 4446618");

    testOperation("Rational", "5 / 2", "-", "1 / 2", "2");

    {
        chooseType("Rational");
        checkExtraButton("Rational");
        pushButton("C");
        inputNumber("2 / 2");
        pushButton("-");
        inputNumber("3 / 3");
        pushButton("=");
        checkFormula("1", "-", "1");
        checkResult("0");
    }

    testOperation("Rational", "5 / 17", "^", "-1", "17 / 5");
}


void TestYourApp::testErrorCode(
        const QString& type,
        const std::string& operand_1,
        const std::string& operation,
        const std::string& operand_2,
        CalculatorTests::ERROR_CODES expected_error_code
        ) {
    chooseType(type);
    inputNumber(operand_1);
    pushButton(operations[operation]);
    inputNumber(operand_2);
    pushButton("=");
    auto expected_error = getErrorMessage(expected_error_code);
    PRAC_COMPARE(l_result->text(), expected_error);
    auto hex_color = l_result->palette().text().color().name().toStdString();
    PRAC_COMPARE2(hex_color, "#ff0000", "Цвет текста ошибки отличен от красного");
    pushButton("C");
    hex_color = l_result->palette().text().color().name().toStdString();
    PRAC_COMPARE2(hex_color, initial_hex_color, "Цвет текста результата не был восстановлен после сброса");
}


void TestYourApp::TestInterfaceZeroDivisionError() {

    testErrorCode("int", "-214", "/", "0", CalculatorTests::ZERO_DIVISION);
    testErrorCode("uint8_t", "3", "/", "0", CalculatorTests::ZERO_DIVISION);
    testErrorCode("size_t", "59823468762", "/", "0", CalculatorTests::ZERO_DIVISION);
    testErrorCode("int64_t", "-135678756123789", "/", "0", CalculatorTests::ZERO_DIVISION);
    testErrorCode("Rational", "2/4", "/", "0", CalculatorTests::ZERO_DIVISION);

}

void TestYourApp::TestInterfaceZeroPowerToZeroError() {

    testErrorCode("int", "0", "^", "0", CalculatorTests::ZERO_POWER_TO_ZERO);
    testErrorCode("uint8_t", "0", "^", "0", CalculatorTests::ZERO_POWER_TO_ZERO);
    testErrorCode("size_t", "0", "^", "0", CalculatorTests::ZERO_POWER_TO_ZERO);
    testErrorCode("int64_t", "0", "^", "0", CalculatorTests::ZERO_POWER_TO_ZERO);
    testErrorCode("Rational", "0", "^", "0", CalculatorTests::ZERO_POWER_TO_ZERO);

}

void TestYourApp::TestInterfaceFractionalPowerError() {
    testErrorCode("Rational", "1 / 2", "^", "2 / 3", CalculatorTests::FRACTIONAL_POWER);
    testErrorCode("Rational", "1 / 2", "^", "3 / 2", CalculatorTests::FRACTIONAL_POWER);
}

void TestYourApp::TestInterfaceIntegerNegativePower() {
    testErrorCode("int", "-21412", "^", "-15", CalculatorTests::INTEGER_NEGATIVE_POWER);
    testErrorCode("int64_t", "876987293847", "^", "-6", CalculatorTests::INTEGER_NEGATIVE_POWER);
}

void TestYourApp::cleanup()
{
    delete window;
}

QTEST_MAIN(TestYourApp)

#include "tests.moc"
