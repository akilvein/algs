#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <set>
#include <bitset>
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

    void print() const {
        cout << "  ";
        for (int c = 0; c < COLS; c++) {
            cout << (char)('A' + c);
        }
        cout << endl;
        cout << "  ";
        for (int c = 0; c < COLS; c++) {
            cout << "-";
        }
        cout << endl;

        for (int r = 0; r < ROWS; r++) {
            cout << r << "|";
            for (int c = 0; c < COLS; c++) {
                int i = r * COLS + c;
                Figure f = cells[i];

                if (f.color == Figure::WHITE) {
                    switch (f.type) {
                    case Figure::EMPTY: cout << ' '; break;
                    case Figure::PAWN: cout << 'P'; break;
                    case Figure::ROOK: cout << 'R'; break;
                    case Figure::BISHOP: cout << 'B'; break;
                    case Figure::KNIGHT: cout << 'N'; break;
                    case Figure::QUEEN: cout << 'Q'; break;
                    }
                } else {
                    switch (f.type) {
                    case Figure::EMPTY: cout << ' '; break;
                    case Figure::PAWN: cout << 'p'; break;
                    case Figure::ROOK: cout << 'r'; break;
                    case Figure::BISHOP: cout << 'b'; break;
                    case Figure::KNIGHT: cout << 'n'; break;
                    case Figure::QUEEN: cout << 'q'; break;
                    }
                }
            }
            cout << endl;
        }

        cout << endl;
    }

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
        //cout << "should upgrage pos: " << pos << " color: " << (int)cells[pos].color << " type: " << (int)cells[pos].type << endl;

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
    //cout << "==============" << endl;
    //cout << (movingColor == Figure::WHITE ? "WHITE" : "BLACK") << " moves done: " << movesDone << endl;
    //board.print();

    if (movesDone == movesLimit) {
        //cout << "reached moves limit " << movesLimit << endl;
        return -1; // whites will not win
    }

    vector<int> moves;
    moves.reserve(2 * ROWS + 2 * COLS - 2);

    for (int pos = 0; pos < ROWS * COLS; pos++) {
        const Figure &movingFigure = board.get(pos);
        if (movingFigure.type == Figure::EMPTY || movingFigure.color != movingColor) {
            continue;
        }

        //cout << "----moving " << (movingFigure.color == Figure::BLACK ? "BLACK " : "WHITE ") << (int)movingFigure.type << endl;

        board.getMoves(pos, moves);
        //cout << "moves.size() " << moves.size() << endl;
        for (int move : moves) {
            const Figure &targetFigure = board.get(move);
            //cout << "----target " << (targetFigure.color == Figure::BLACK ? "BLACK " : "WHITE ") << (int)targetFigure.type << endl;

            // capture queen!
            if (targetFigure.type == Figure::QUEEN) {
                //cout << (movingColor == Figure::WHITE ? "WHITE" : "BLACK") << " ";
                //cout << "targeting queen" << endl;
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

            if (newBoard.shouldUpgrade(move)) {
                int result;

                newBoard.set(move, Figure(Figure::ROOK, movingColor));
                result = movesToWhiteWin(newBoard, movesLimit, movesDone + 1, (Figure::Color)(1 - movingColor));
                if (movingColor == Figure::WHITE && result >= 0) {
                    return result;
                } else if (movingColor == Figure::BLACK && result < 0)  {
                    return -1;
                }

                newBoard.set(move, Figure(Figure::KNIGHT, movingColor));
                result = movesToWhiteWin(newBoard, movesLimit, movesDone + 1, (Figure::Color)(1 - movingColor));
                if (movingColor == Figure::WHITE && result >= 0) {
                    return result;
                } else if (movingColor == Figure::BLACK && result < 0)  {
                    return -1;
                }

                newBoard.set(move, Figure(Figure::BISHOP, movingColor));
                result = movesToWhiteWin(newBoard, movesLimit, movesDone + 1, (Figure::Color)(1 - movingColor));
                if (movingColor == Figure::WHITE && result >= 0) {
                    return result;
                } else if (movingColor == Figure::BLACK && result < 0)  {
                    return -1;
                }
            } else {
                int result;
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

template <int ROWS, int COLS>
Board<ROWS, COLS> createTestBoard(vector<const char *> b) {
    Board<ROWS, COLS> result;
    // UPPERCASE are white, lowercase are black
    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLS; c++) {
            char type = b[r][c];
            if (type == 'P') {
                result.putFigure('P', 'W', r, c);
            } else if (type == 'R') {
                result.putFigure('R', 'W', r, c);
            } else if (type == 'B') {
                result.putFigure('B', 'W', r, c);
            } else if (type == 'N') {
                result.putFigure('N', 'W', r, c);
            } else if (type == 'Q') {
                result.putFigure('Q', 'W', r, c);
            } else if (type == 'p') {
                result.putFigure('p', 'b', r, c);
            } else if (type == 'r') {
                result.putFigure('r', 'b', r, c);
            } else if (type == 'b') {
                result.putFigure('b', 'b', r, c);
            } else if (type == 'n') {
                result.putFigure('n', 'b', r, c);
            } else if (type == 'q') {
                result.putFigure('q', 'b', r, c);
            }
        }
    }

    return result;
}

void compareMoves(vector<int> m1, vector<int> m2) {
    sort(m1.begin(), m1.end());
    sort(m2.begin(), m2.end());

    assert(m1.size() == m2.size());
    for (int i = 0; i < m1.size(); i++) {
        assert(m1[i] == m2[i]);
    }
}

template <int ROWS, int COLS>
int posToIndex(int r, int c) {
    assert(r >= 0);
    assert(r < ROWS);
    assert(c >= 0);
    assert(c < COLS);

    return r * COLS + c;
}

template <int ROWS, int COLS>
vector<int> makeReferenceMoves(vector<const char *> b) {
    vector<int> moves;

    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLS; c++) {
            char type = b[r][c];
            if (type != ' ') {
                moves.push_back(posToIndex<ROWS, COLS>(r, c));
            }
        }
    }

    return moves;
}

template <int ROWS, int COLS>
void printMoves(const vector<int> &moves) {
    cout << "  ";
    for (int c = 0; c < COLS; c++) {
        cout << (char)('A' + c);
    }
    cout << endl;
    cout << "  ";
    for (int c = 0; c < COLS; c++) {
        cout << "-";
    }
    cout << endl;

    for (int r = 0; r < ROWS; r++) {
        cout << r << "|";
        for (int c = 0; c < COLS; c++) {
            int i = r * COLS + c;
            bool found = false;
            for (int m : moves) {
                if (m == i) {
                    cout << "X";
                    found = true;
                    break;
                }
            }
            if (!found) {
                cout << " ";
            }
        }
        cout << endl;
    }

    cout << endl;
}

template <int ROWS, int COLS>
void printBoard(Board<ROWS, COLS> board) {
    board.print();
}

void testBoard1() {
    Board<4, 4> b = createTestBoard<4, 4>({"   P",
                                           " Qr ",
                                           "    ",
                                           " p P"});

    printBoard<4, 4>(b);

    assert(!b.shouldUpgrade(posToIndex<4, 4>(3, 1)));
    assert(!b.shouldUpgrade(posToIndex<4, 4>(0, 3)));
    assert(b.shouldUpgrade(posToIndex<4, 4>(3, 3)));

    // test white queen
    vector<int> moves;
    b.getMoves(posToIndex<4, 4>(1, 1), moves);

    vector<int> refMoves = makeReferenceMoves<4, 4>({"XXX ",
                                                     "X X ",
                                                     "XXX ",
                                                     " X  "});


    compareMoves(moves, refMoves);

    // test black rook
    b.getMoves(posToIndex<4, 4>(1, 2), moves);
    refMoves = makeReferenceMoves<4, 4>({"  X ",
                                         " X X",
                                         "  X ",
                                         "  X "});

    compareMoves(moves, refMoves);
}


void testBoard2() {
    Board<4, 4> b = createTestBoard<4, 4>({"    ",
                                           " Q  ",
                                           "  p ",
                                           "    "});

    printBoard<4, 4>(b);

    assert(!b.shouldUpgrade(posToIndex<4, 4>(2, 2)));

    vector<int> moves;
    b.getMoves(posToIndex<4, 4>(2, 2), moves);

    vector<int> refMoves = makeReferenceMoves<4, 4>({"    ",
                                                     " XX ",
                                                     "    ",
                                                     "    "});


    compareMoves(moves, refMoves);
}


void testBoard3() {
    Board<4, 4> b = createTestBoard<4, 4>({"    ",
                                           "  p ",
                                           "  p ",
                                           "    "});

    printBoard<4, 4>(b);

    assert(!b.shouldUpgrade(posToIndex<4, 4>(2, 2)));

    vector<int> moves;
    b.getMoves(posToIndex<4, 4>(2, 2), moves);

    vector<int> refMoves = makeReferenceMoves<4, 4>({"    ",
                                                     "    ",
                                                     "    ",
                                                     "    "});


    compareMoves(moves, refMoves);
}

void testBoard4() {
    Board<4, 4> b = createTestBoard<4, 4>({"    ",
                                           "    ",
                                           "  N ",
                                           "    "});

    printBoard<4, 4>(b);

    assert(!b.shouldUpgrade(posToIndex<4, 4>(2, 2)));

    vector<int> moves;
    b.getMoves(posToIndex<4, 4>(2, 2), moves);

    vector<int> refMoves = makeReferenceMoves<4, 4>({" X X",
                                                     "X   ",
                                                     "    ",
                                                     "X   "});


    compareMoves(moves, refMoves);
}

void testBoard5() {
    Board<4, 4> b = createTestBoard<4, 4>({" Q  ",
                                           "    ",
                                           " P  ",
                                           "q   "});

    printBoard<4, 4>(b);

    assert(!b.shouldUpgrade(posToIndex<4, 4>(2, 1)));

    vector<int> moves;
    b.getMoves(posToIndex<4, 4>(2, 1), moves);

    vector<int> refMoves = makeReferenceMoves<4, 4>({"    ",
                                                     "    ",
                                                     "    ",
                                                     "XX  "});


    compareMoves(moves, refMoves);
}

int main()
{
#ifdef _DEBUG
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

        //printBoard<4, 4>(board);

        if (movesToWhiteWin<4, 4>(board, m) >= 0) {
            cout << "YES" << endl;
        } else {
            cout << "NO" << endl;
        }
    }
}
