
// Core headers
#include <QtTest>

#include <vector>

#include "../submission/controller.h"
#include "model.h"
#include "view.h"

#include "test_utils/practicum_assert.hpp"

void checkViewStatus(
    const QString& expected_status,
    const QString& expected_status_color,
    const bool expected_status_boldness) {

    const auto actual_status = View::getStatus();
    const auto actual_status_color = View::getStatusColor();
    const auto actual_status_boldness = View::getStatusBoldness();

    PRAC_COMPARE2(actual_status, expected_status, "Статус не совпадает с ожидаемым");
    PRAC_COMPARE2(actual_status_color, expected_status_color, "Цвет статуса не совпадает с ожидаемым");
    PRAC_COMPARE2(actual_status_boldness, expected_status_boldness, "Жирность шрифта статуса не совпадает с ожидаемой");
}

void checkNextMoveStatus(const Symbol& next_player) {
    const auto player = ToQString(next_player);
    const QString expected_status = "Ход игрока " + player;
    checkViewStatus(expected_status, "black", false);
}

void checkViewCallbacks() {
    const auto new_game = View::getNewGameCallbackIsSet();
    const auto move = View::getMoveCallbackIsSet();

    QVERIFY2(new_game, "Не задана функция старта игры");
    QVERIFY2(move, "Не задана функция выполнения хода");
}

void checkViewSize(const size_t size_x, const size_t size_y) {
    const auto size = View::getSize();
    const std::pair expected_size{size_x, size_y};
    PRAC_COMPARE2(size, expected_size, "Размер поля представления не соответсвуте ожидаемому");
}

void checkModelSize(const size_t size_x, const size_t size_y) {
    const auto size = Model::getSize();
    const std::pair expected_size{size_x, size_y};
    PRAC_COMPARE2(size, expected_size, "Размер поля модели не соответсвуте ожидаемому");
}

void checkNewGame(const size_t size_x, const size_t size_y) {
    checkNextMoveStatus(Symbol::kCross);
    checkViewSize(size_x, size_y);
    checkModelSize(size_x, size_y);
    QVERIFY2(Model::getWasReset(), "Модель не была сброшена во время создания новой игры");
}

void checkViewMoves(const std::vector<View::Move>& expected_moves) {
    auto moves = View::getMoves();
    PRAC_COMPARE2(moves.size(), expected_moves.size(), "Количество ходов не соответствует ожидаемому");
    PRAC_COMPARE2(moves, expected_moves, "Фактические ходы не соответсвуют ожидаемым");
}

void checkViewWinningMovesStyle(const std::vector<std::pair<size_t, size_t>>& winning_moves) {
    auto styles = View::getMovesStyles();
    std::vector<View::MoveStyle> expected_styles;

    for (const auto& move : winning_moves) {
        const View::MoveStyle style{move, "green", true};
        expected_styles.push_back(style);
    }

    PRAC_COMPARE(styles, expected_styles);
}


class TestYourApp : public QObject
{
    Q_OBJECT
public:
    TestYourApp()=default;
    ~TestYourApp()=default;

private slots:
    void init() {
        moves.clear();
    }

    void TestConstructor();
    void TestNewGameCallback();
    void TestMoveCallback();
    void TestBadMove();

    void TestWin();
    void TestTie();

private:
    void doMove(const View::Move& move);
    std::vector<View::Move> moves;

};

void TestYourApp::doMove(const View::Move& move) {
    auto symbol = ToSymbol(move.second);
    Model::setDoMove(symbol);
    View::move(move.first);
    moves.push_back(move);
    checkViewMoves(moves);
}

void TestYourApp::TestConstructor() {

    Controller controller;

    checkViewCallbacks();
    checkNewGame(3, 3);
    QVERIFY2(View::getIsShown(), "Поле не показано");

}

void TestYourApp::TestNewGameCallback() {
    Controller controller;
    View::newgame(5, 15);
    checkNewGame(5, 15);
    View::newgame(15, 25);
    checkNewGame(15, 25);
}

void TestYourApp::TestMoveCallback() {

    {
        // Single successful not winning move
        Controller controller;
        Model::setWinner(std::nullopt);
        View::Move move{{1, 1}, "o"};

        Model::setNextMove(Symbol::kCross);
        doMove(move);

        checkNextMoveStatus(Symbol::kCross);
    }
    moves.clear();
    {
        // Two simple moves
        Controller controller;
        View::Move move_1{{1, 1}, "o"};
        View::Move move_2{{2, 2}, "x"};
        Model::setNextMove(Symbol::kNought);
        doMove(move_1);
        doMove(move_2);
        checkNextMoveStatus(Symbol::kNought);
    }
}

void TestYourApp::TestBadMove() {
    Controller controller;
    Model::setWinner(std::nullopt);
    Model::setNextMove(Symbol::kCross);
    Model::setDoMove(Symbol::kEmpty);
    View::move(1, 1);
    const auto moves = View::getMoves();
    QVERIFY(moves.empty());
}

void TestYourApp::TestWin() {

    {
        Controller controller;
        std::vector<View::Move> moves{
            {{1, 1}, "x"},
            {{1, 2}, "o"},
            {{2, 2}, "x"},
            {{1, 3}, "o"},
            };

        for (const auto& move : moves) {
            doMove(move);
        }
        View::Move winning_move{{3, 3}, "x"};
        Model::setWinner(Symbol::kCross);
        std::vector winning_moves{
            moves[0].first,
            moves[2].first,
            winning_move.first
        };
        Model::setWinnerPath(winning_moves);
        doMove(winning_move);
        checkViewWinningMovesStyle(winning_moves);
        checkViewStatus("Выиграл x", "green", true);
    }
    moves.clear();
    {
        Controller controller;
        std::vector<View::Move> moves{
                {{2, 1}, "o"},
                {{1, 2}, "x"},
                {{2, 2}, "o"},
                {{3, 3}, "x"},
                };

        for (const auto& move : moves) {
            doMove(move);
        }
        View::Move winning_move{{2, 3}, "o"};
        Model::setWinner(Symbol::kNought);
        const std::vector winning_moves{
            moves[0].first,
            moves[2].first,
            winning_move.first
        };
        Model::setWinnerPath(winning_moves);
        doMove(winning_move);
        checkViewWinningMovesStyle(winning_moves);
        checkViewStatus("Выиграл o", "green", true);
    }
}

void TestYourApp::TestTie() {
    Controller controller;
    std::vector<View::Move> moves{
                {{1, 1}, "x"},
                {{2, 2}, "o"},
                };

    for (const auto& move : moves) {
        doMove(move);
    }

    View::Move final_move{{3, 3}, "x"};
    Model::setWinner(Symbol::kEmpty);

    doMove(final_move);
    checkViewWinningMovesStyle({});
    checkViewStatus("Ничья", "red", true);
}


QTEST_MAIN(TestYourApp)

#include "tests.moc"
