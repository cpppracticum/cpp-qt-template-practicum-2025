;
// Core headers
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QWidget>

#include <QtWidgets/QPushButton>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QLabel>
#include <QtWidgets/QComboBox>

#include <QtTest>

#include <vector>
#include <string>

#include "mainwindow.h"
#include "player.h"
#include "signal-processing.h"
#include "trivial_signal_generator.h"

#include "utils.h"

#include "test_utils/practicum_assert.hpp"

std::vector<int16_t> player_data{};


class TestYourApp : public QObject
{
    Q_OBJECT
public:
    TestYourApp()=default;
    ~TestYourApp()=default;

private slots:
    void init();
    void cleanup();


    void TestSignalReplication();
    void TestSinSignal();
    void TestCosSignal();
    void TestMeanderSignal();
    void TestAttackDecayApplying();

    void TestInterfaceDefaults();
    void TestInterfaceSin();
    void TestInterfaceCos();
    void TestInterfaceMeander();

private:

    void setSignalData(const SignalData &data) const;
    void play() const;
    void playSignal(const SignalData &data) const;

    static void checkVec(const std::vector<int16_t> &vec, const std::vector<int16_t> &expected);

    MainWindow *window{};

    QLineEdit *le_duration{};
    QLineEdit *le_wavelength{};
    QLineEdit *le_amplitude{};
    QLineEdit *le_cycle{};

    QComboBox *cb_type{};

    QPushButton *btn_play{};
};



void TestYourApp::init()
{
    window = new MainWindow();

    QVERIFY2(window != nullptr, "Главное окно не создано");
    window->show();
    QVERIFY2(window->isVisible(), "Главное окно не активируется");

    le_duration = getChild<QLineEdit>(window, "le_duration", "QLineEdit");
    le_wavelength = getChild<QLineEdit>(window, "le_wavelength", "QLineEdit");
    le_amplitude = getChild<QLineEdit>(window, "le_amplitude", "QLineEdit");
    le_cycle = getChild<QLineEdit>(window, "le_cycle", "QLineEdit");
    cb_type = getChild<QComboBox>(window, "cb_type", "QComboBox");

    btn_play = getChildByText<QPushButton>(window, "Проиграть", "QPushButton");
}

void TestYourApp::setSignalData(const SignalData& data) const {

    le_duration->setText(QString::number(data.samples));
    le_wavelength->setText(QString::number(data.wave_length));
    le_amplitude->setText(QString::number(data.amplitude));
    le_cycle->setText(QString::number(data.cycle));

    // Because text setting doesn't call ...editingFinished,
    // so index must be strictly changed for applying new signal
    cb_type->setCurrentIndex(0);

    if (data.signal_type == SignalType::kSin) {
        cb_type->setCurrentIndex(cb_type->findText("Синус"));
    } else if (data.signal_type == SignalType::kCos) {
        cb_type->setCurrentIndex(cb_type->findText("Косинус"));
    } else if (data.signal_type == SignalType::kMeander) {
        cb_type->setCurrentIndex(cb_type->findText("Меандр"));
    }

}

void TestYourApp::play() const {
    clickWidget(btn_play);
}

void TestYourApp::playSignal(const SignalData &data) const {
    setSignalData(data);
    play();
}

void TestYourApp::checkVec(const std::vector<int16_t>& vec, const std::vector<int16_t>& expected) {

    QCOMPARE(vec.size() - 1200, expected.size());
    for (size_t i = 0; i < expected.size(); ++i) {
        const auto diff = std::abs(vec[i] - expected[i]);
        QVERIFY(diff <= 1);
    }

    for (size_t i = expected.size(); i < vec.size(); ++i) {
        QCOMPARE(vec[i], 0);
    }
}


void TestYourApp::TestSignalReplication() {
    SignalData data
    {
        .signal_type = SignalType::kMeander,
        .samples = 15,
        .wave_length = 3,
        .amplitude = 15,
        .cycle = 0.33
    };

    auto meander = generateMeander(data, false);
    ReplicateSignal(meander, data.samples);
    const auto expected_meander = generateMeander(data);
    PRAC_COMPARE(meander, expected_meander);

    data.signal_type = SignalType::kSin;
    data.wave_length = 30;
    data.amplitude = 5874;
    data.samples = 1500;

    auto sin = generateSin(data, false);
    ReplicateSignal(sin, data.samples);
    const auto expected_sin = generateSin(data);
    PRAC_COMPARE(sin, expected_sin);

    data.signal_type = SignalType::kCos;

    auto cos = generateCos(data, false);
    ReplicateSignal(cos, data.samples);
    const auto expected_cos = generateCos(data);
    PRAC_COMPARE(cos, expected_cos);
}

void TestYourApp::TestSinSignal() {

    SignalData data
    {
        .signal_type = SignalType::kSin,
        .samples = 157,
        .wave_length = 16,
        .amplitude = 5784,
        .cycle = 0.34
    };
    const auto vec = GenerateSignal(data);
    const auto expected_vec = generateSin(data);
    PRAC_COMPARE(vec, expected_vec);

    data.wave_length = 275;
    data.samples = 19867;
    data.amplitude = 3;
    data.cycle = 0.987;

    const auto vec2 = GenerateSignal(data);
    const auto expected_vec2 = generateSin(data);
    PRAC_COMPARE(vec2, expected_vec2);

    data.wave_length = 2750;
    data.samples = 198;

    const auto vec3 = GenerateSignal(data);
    const auto expected_vec3 = generateSin(data);
    PRAC_COMPARE(vec3, expected_vec3);
}

void TestYourApp::TestCosSignal() {

    SignalData data
    {
        .signal_type = SignalType::kCos,
        .samples = 8979,
        .wave_length = 384,
        .amplitude = 9803,
        .cycle = 0.12
    };
    const auto vec = GenerateSignal(data);
    const auto expected_vec = generateCos(data);
    PRAC_COMPARE(vec, expected_vec);

    data.wave_length = 983;
    data.samples = 17653;
    data.amplitude = 9689;
    data.cycle = 0.0003;

    const auto vec2 = GenerateSignal(data);
    const auto expected_vec2 = generateCos(data);
    PRAC_COMPARE(vec2, expected_vec2);


    data.wave_length = 9830;
    data.samples = 176;

    const auto vec3 = GenerateSignal(data);
    const auto expected_vec3 = generateCos(data);
    PRAC_COMPARE(vec3, expected_vec3);

}

void TestYourApp::TestMeanderSignal() {

    SignalData data
    {
        .signal_type = SignalType::kMeander,
        .samples = 10000,
        .wave_length = 1258,
        .amplitude = 354,
        .cycle = 0.5
    };
    const auto vec = GenerateSignal(data);
    const auto expected_vec = generateMeander(data);
    PRAC_COMPARE(vec, expected_vec);

    data.wave_length = 10000;
    data.samples = 57691;
    data.amplitude = 9689;
    data.cycle = 0.874;

    const auto vec2 = GenerateSignal(data);
    const auto expected_vec2 = generateMeander(data);
    PRAC_COMPARE(vec2, expected_vec2);

    data.wave_length = 100;
    data.samples = 57;
    data.cycle = 0.12;

    const auto vec3 = GenerateSignal(data);
    const auto expected_vec3 = generateMeander(data);
    PRAC_COMPARE(vec3, expected_vec3);
}

void TestYourApp::TestAttackDecayApplying() {

    SignalData data
    {
        .signal_type = SignalType::kCos,
        .samples = 10000,
        .wave_length = 384,
        .amplitude = 8096,
        .cycle = 0.6
    };

    auto vec = generateSin(data);
    auto expected_vec = vec;
    constexpr size_t max_length = 8670;

    ApplyDecayAndAttack(vec, max_length);
    applyDecay(expected_vec, max_length);
    applyAttack(expected_vec, max_length);
    PRAC_COMPARE(vec, expected_vec);

}

void TestYourApp::TestInterfaceDefaults() {
    QCOMPARE(le_duration->text(), QString::number(48000));
    QCOMPARE(le_wavelength->text(), QString::number(200));
    QCOMPARE(le_amplitude->text(), QString::number(10000));
    QCOMPARE(le_cycle->text(), QString::number(0.5));
}

void TestYourApp::TestInterfaceSin() {
    SignalData data
    {
        .signal_type = SignalType::kSin,
        .samples = 157,
        .wave_length = 16,
        .amplitude = 5784,
        .cycle = 0.34
    };
    auto expected_vec = generateSin(data);
    applyAttack(expected_vec, 12000);
    applyDecay(expected_vec, 12000);

    playSignal(data);
    checkVec(player_data, expected_vec);


    data.wave_length = 16000;
    data.amplitude = 8750;
    data.samples = 160000;

    expected_vec = generateSin(data);
    applyAttack(expected_vec, 12000);
    applyDecay(expected_vec, 12000);

    playSignal(data);
    checkVec(player_data, expected_vec);
}

void TestYourApp::TestInterfaceCos() {
    SignalData data
    {
        .signal_type = SignalType::kCos,
        .samples = 570030,
        .wave_length = 1286,
        .amplitude = 32054,
        .cycle = 0.98
    };

    auto expected_vec = generateCos(data);
    applyAttack(expected_vec, 12000);
    applyDecay(expected_vec, 12000);

    playSignal(data);
    checkVec(player_data, expected_vec);

    data.wave_length = 8601;
    data.amplitude = 136;
    data.samples = 875000;

    expected_vec = generateCos(data);
    applyAttack(expected_vec, 12000);
    applyDecay(expected_vec, 12000);

    playSignal(data);
    checkVec(player_data, expected_vec);

}

void TestYourApp::TestInterfaceMeander() {
    SignalData data
    {
        .signal_type = SignalType::kMeander,
        .samples = 1000,
        .wave_length = 256,
        .amplitude = 10,
        .cycle = 0.75
    };

    auto expected_vec = generateMeander(data);
    applyAttack(expected_vec, 12000);
    applyDecay(expected_vec, 12000);

    playSignal(data);
    checkVec(player_data, expected_vec);

    data.wave_length = 1000;
    data.amplitude = 136;
    data.samples = 988823;
    data.cycle = 0.985;

    expected_vec = generateMeander(data);
    applyAttack(expected_vec, 12000);
    applyDecay(expected_vec, 12000);

    playSignal(data);
    checkVec(player_data, expected_vec);
}

void TestYourApp::cleanup()
{
    delete window;
}

QTEST_MAIN(TestYourApp)

#include "tests.moc"
