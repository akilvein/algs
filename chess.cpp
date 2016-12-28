#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>

using namespace std;


struct Figure {
    enum Type {
        EMPTY,
        QUEEN,
        ROOK,
        BISHOP,
        KNIGHT,
        PAWN
    };

    enum Color {
        WHITE,
        BLACK
    };

    char type;
    char color;

    Figure() : type(EMPTY) {}
    Figure(Type t, Color c) : type(t), color(c) { }

    int code() {
        return 8 * color + type;
    }
};

template <int ROWS, int COLS>
struct Board {
    Figure cells[ROWS * COLS];

    Figure::Color charToColor(const char color) {
        Figure::Color c;
        if (color == 'W' || color == 'w') {
            c = Figure::WHITE;
        } else if (color == 'B' || color == 'b') {
            c = Figure::BLACK;
        } else {
            assert(false);
        }

        return c;
    }

    Figure::Type charToType(const char type) {
        Figure::Type t;

        if (type == 'E' || type == 'e' || type == ' ') {
            t = Figure::EMPTY;
        } else if (type == 'P' || type == 'p') {
            t = Figure::PAWN;
        } else if (type == 'R' || type == 'r') {
            t = Figure::ROOK;
        } else if (type == 'B' || type == 'b') {
            t = Figure::BISHOP;
        } else if (type == 'N' || type == 'n') {
            t = Figure::KNIGHT;
        } else if (type == 'Q' || type == 'q') {
            t = Figure::QUEEN;
        } else {
            cout << "unexpected character \"" << type << "\"" << endl;
            assert(false);
        }

        return t;
    }

    void putFigure(const char type, const char color, int row, int col) {
        cells[row * COLS + col] = Figure(charToType(type), charToColor(color));
    }

    inline void set(int index, const Figure &f) {
        cells[index] = f;
    }

    inline Figure get(int index) const {
        return cells[index];
    }

    inline Figure get(int row, int col) const {
        return cells[row * COLS + col];
    }

    inline bool addMoveAndContinue(int currentPos, int targetPos, vector<int> &out) const {
        if (cells[targetPos].type == Figure::EMPTY) {
            // target cell is empty. add this move. figure may continue moving
            out.push_back(targetPos);
            return true;
        } else if (cells[targetPos].color != cells[currentPos].color) {
            // target cell is occupied by opposite figure. capture it. may not continue moving to next cells
            out.push_back(targetPos);
            return false;
        }

        // target cell is occupied with same color figure. do not add this move. and may not move to next cells
        return false;
    }

    void getStraightMoves(int pos, vector<int> &out) const {
        int p;

        // move up
        for (p = pos - COLS; p >= 0; p -= COLS) {
            if (!addMoveAndContinue(pos, p, out)) break;
        }

        // move down
        for (p = pos + COLS; p < ROWS * COLS; p += COLS) {
            if (!addMoveAndContinue(pos, p, out)) break;
        }

        // move left
        const int rowStart = pos - pos % COLS;
        for (p = pos - 1; p >= rowStart; p--) {
            if (!addMoveAndContinue(pos, p, out)) break;
        }

        // move right
        const int rowEnd = rowStart + COLS;
        for (p = pos + 1; p < rowEnd; p++) {
            if (!addMoveAndContinue(pos, p, out)) break;
        }
    }

    void getDiagonalMoves(int pos, vector<int> &out) const {
        int r;
        int c;

        const int row = pos / COLS;
        const int col = pos % COLS;

        // up-right
        for (r = row + 1, c = col + 1; r < ROWS && c < COLS; r++, c++) {
            if (!addMoveAndContinue(pos, r * COLS + c, out)) break;
        }

        // down-right
        for (r = row - 1, c = col + 1; r >= 0 && c < COLS; r--, c++) {
            if (!addMoveAndContinue(pos, r * COLS + c, out)) break;
        }

        // down-left
        for (r = row - 1, c = col - 1; r >= 0 && c >= 0; r--, c--) {
            if (!addMoveAndContinue(pos, r * COLS + c, out)) break;
        }

        // up-left
        for (r = row + 1, c = col - 1; r < ROWS && c >= 0; r++, c--) {
            if (!addMoveAndContinue(pos, r * COLS + c, out)) break;
        }
    }

    void getKnightMoves(int pos, vector<int> &out) const {
        const int rs[] = {1, 2, 2, 1, -1, -2, -2, -1};
        const int cs[] = {-2, -1, 1, 2, 2, 1, -1, -2};

        const int row = pos / COLS;
        const int col = pos % COLS;

        for (int i = 0; i < 8; i++)
        {
            int r = row + rs[i];
            int c = col + cs[i];

            if (r >= 0 && r < ROWS && c >= 0 && c < COLS) {
                addMoveAndContinue(pos, r * COLS + c, out);
            }
        }
    }

    inline bool shouldUpgrade(int pos) const {
        const int row = pos / COLS;

        if (cells[pos].type == Figure::PAWN && cells[pos].color == Figure::WHITE && row == ROWS - 1) {
            return true;
        }

        if (cells[pos].type == Figure::PAWN && cells[pos].color == Figure::BLACK && row == 0) {
            return true;
        }

        return false;
    }

    void getPawnMoves(int pos, vector<int> &out) const {
        const int row = pos / COLS;
        const int col = pos % COLS;

        if (cells[pos].color == Figure::WHITE) { // white goes up
            int r = row + 1;
            int c = col;

            if (r < ROWS && get(r, col).type == Figure::EMPTY) {
                addMoveAndContinue(pos, r * COLS + c, out);
            }

            c = col + 1;
            if (r < ROWS && c < COLS && get(r, c).type != Figure::EMPTY) { // capture move
                addMoveAndContinue(pos, r * COLS + c, out);
            }

            c = col - 1;
            if (r < ROWS && c >= 0 && get(r, c).type != Figure::EMPTY) { // capture move
                addMoveAndContinue(pos, r * COLS + c, out);
            }
        } else { //black goes down
            int r = row - 1;
            int c = col;

            if (r >= 0 && get(r, col).type == Figure::EMPTY) {
                addMoveAndContinue(pos, r * COLS + c, out);
            }

            c = col + 1;
            if (r >= 0 && c < COLS && get(r, c).type != Figure::EMPTY) { // capture move
                addMoveAndContinue(pos, r * COLS + c, out);
            }

            c = col - 1;
            if (r >= 0 && c >= 0 && get(r, c).type != Figure::EMPTY) { // capture move
                addMoveAndContinue(pos, r * COLS + c, out);
            }
        }
    }

    void getMoves(int pos, vector<int> &out) const {
        out.clear();

        const Figure &f = cells[pos];
        switch (f.type) {
        case Figure::QUEEN:
            getStraightMoves(pos, out);
            getDiagonalMoves(pos, out);
            break;
        case Figure::BISHOP:
            getDiagonalMoves(pos, out);
            break;
        case Figure::ROOK:
            getStraightMoves(pos, out);
            break;
        case Figure::KNIGHT:
            getKnightMoves(pos, out);
            break;
        case Figure::PAWN:
            getPawnMoves(pos, out);
            break;
        default:
            assert(false);
        }
    }

    static inline bool inSameColumn(int pos1, int pos2) {
        return (pos1 % COLS) == (pos2 % COLS);
    }
};

template <int ROWS, int COLS>
int movesToWhiteWin(const Board<ROWS, COLS> &board, int movesLimit, int movesDone = 0, Figure::Color movingColor = Figure::WHITE) {
    if (movesDone == movesLimit) {
        return -1; // whites will not win
    }

    vector<int> moves;
    moves.reserve(2 * ROWS + 2 * COLS - 2);

    for (int pos = 0; pos < ROWS * COLS; pos++) {
        const Figure &movingFigure = board.get(pos);
        if (movingFigure.type == Figure::EMPTY || movingFigure.color != movingColor) {
            continue;
        }

        board.getMoves(pos, moves);
        for (int move : moves) {
            const Figure &targetFigure = board.get(move);

            // capture queen!
            if (targetFigure.type == Figure::QUEEN) {
                if (movingColor == Figure::WHITE) { // whites win
                    return movesDone;
                } else { // blacks win
                    return -1;
                }
            }

            // capture some other figure or move to empty cell
            Board<ROWS, COLS> newBoard(board);
            newBoard.set(pos, Figure());
            newBoard.set(move, movingFigure);

            int result;
            if (newBoard.shouldUpgrade(move)) {
                for (int t = Figure::ROOK; t <= Figure::KNIGHT; t++) {
                    newBoard.set(move, Figure((Figure::Type)t, movingColor));
                    result = movesToWhiteWin(newBoard, movesLimit, movesDone + 1, (Figure::Color)(1 - movingColor));
                    if (movingColor == Figure::WHITE && result >= 0) {
                        return result;
                    } else if (movingColor == Figure::BLACK && result < 0)  {
                        return -1;
                    }
                }
            } else {
                result = movesToWhiteWin(newBoard, movesLimit, movesDone + 1, (Figure::Color)(1 - movingColor));
                if (movingColor == Figure::WHITE && result >= 0) {
                    return result;
                } else if (movingColor == Figure::BLACK && result < 0)  {
                    return -1;
                }
            }
        }
    }

    if (movingColor == Figure::BLACK) {
        return movesLimit - movesDone;
    } else {
        return -1;
    }
}


int main() {
#if 1
    freopen("input28.txt", "r", stdin);
    freopen("output28-my.txt", "w", stdout);
#endif

    int tests;
    cin >> tests;

    for (int t = 0; t < tests; t++) {
        int w, b, m;
        cin >> w >> b >> m;

        Board<4, 4> board;
        for (int i = 0; i < w + b; i++) {
            char type;
            int row;
            char col;
            cin >> type >> col >> row;

            board.putFigure(type, (i < w) ? 'W' : 'B', row - 1, col - 'A');
        }

        if (movesToWhiteWin<4, 4>(board, m) >= 0) {
            cout << "YES" << endl;
        } else {
            cout << "NO" << endl;
        }
    }

}
