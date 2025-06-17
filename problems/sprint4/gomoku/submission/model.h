#pragma once

#include <vector>
#include <QString>

enum class Symbol {
    kEmpty = ' ',
    kCross = 'x',
    kNought = 'o'
};

inline QString ToQString(Symbol s) {
    return QChar(static_cast<char>(s));
}

class Model {
public:
    Model() : Model(0,0) {}
    Model(size_t field_size_x, size_t field_size_y)
        : field_size_x_(field_size_x), field_size_y_(field_size_y) {
    }

    void Reset(size_t field_size_x, size_t field_size_y) {
        *this = Model(field_size_x, field_size_y);
    }

    Symbol DoMove(size_t row, size_t col) {
        if (winner_.has_value()) {
            return Symbol::kEmpty;
        }

        auto cur_symbol = next_move_;
        bool result = SetSymbol(row, col, cur_symbol);

        if (!result) {
            return Symbol::kEmpty;
        }

        if (--empty_cells_ == 0) {
            winner_ = Symbol::kEmpty;
        }

        auto path = GetLongestPath(row, col);
        if (path.size() >= win_length_) {
            winner_ = cur_symbol;
            winner_path_ = path;
        }

        next_move_ = Invert(cur_symbol);
        return cur_symbol;
    }

    Symbol GetNextPlayer() const {
        return next_move_;
    }

    std::optional<Symbol> GetWinner() const {
        return winner_;
    }

    std::vector<std::pair<size_t, size_t>> GetWinnerPath() const {
        return winner_path_;
    }

private:
    void AppendFieldsInDirection(size_t row, size_t col, Symbol smb, int dx, int dy, std::vector<std::pair<size_t, size_t>>& fields) const {
        for (;;) {
            row += dx;
            col += dy;
            if (Get(row, col) != smb) {
                return;
            }
            fields.push_back({row, col});
        }
    }

    std::vector<std::pair<size_t, size_t>> GetFieldsInBothDirections(size_t row, size_t col, Symbol smb, int dx, int dy) const {
        std::vector<std::pair<size_t, size_t>> result;
        result.push_back({row, col});

        AppendFieldsInDirection(row, col, smb, dx, dy, result);
        AppendFieldsInDirection(row, col, smb, -dx, -dy, result);

        return result;
    }

    std::vector<std::pair<size_t, size_t>> GetLongestPath(size_t row, size_t col) {
        Symbol smb = Get(row, col);
        if (smb == Symbol::kEmpty) {
            return {};
        }

        auto h_path = GetFieldsInBothDirections(row, col, smb, -1, 0);
        auto v_path = GetFieldsInBothDirections(row, col, smb, 0, -1);
        auto hv_path = GetFieldsInBothDirections(row, col, smb, 1, 1);
        auto vh_path = GetFieldsInBothDirections(row, col, smb, 1, -1);

        return std::max({h_path, v_path, hv_path, vh_path}, [](const auto& l, const auto& r){
            return l.size() < r.size();
        });
    }

    Symbol Get(size_t row, size_t col) const {
        if (row >= field_size_x_ || col >= field_size_y_) {
            return Symbol::kEmpty;
        }
        return field_[row][col];
    }

    static Symbol Invert(Symbol smb) {
        switch(smb) {
        case Symbol::kCross:
            return Symbol::kNought;
        case Symbol::kNought:
            return Symbol::kCross;
        default:
            return smb;
        }
    }

    bool SetSymbol(size_t row, size_t col, Symbol symbol) {
        if(row >= field_size_x_ || col >= field_size_y_) {
            return false;
        }

        if(field_[row][col] != Symbol::kEmpty) {
            return false;
        }

        field_[row][col] = symbol;
        return true;
    }

private:
    Symbol next_move_ = Symbol::kCross;
    std::optional<Symbol> winner_;
    std::vector<std::pair<size_t, size_t>> winner_path_;

    size_t field_size_x_, field_size_y_;
    size_t win_length_ = std::min<size_t>({field_size_x_, field_size_y_, 5u});
    size_t empty_cells_ = field_size_x_ * field_size_y_;

    std::vector<std::vector<Symbol>> field_ = std::vector<std::vector<Symbol>>(
        field_size_x_,
        std::vector<Symbol>(field_size_y_, Symbol::kEmpty)
    );
};
