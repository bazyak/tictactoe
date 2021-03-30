#ifndef TICTACTOE_H
#define TICTACTOE_H

#include <map>

using namespace std;

struct TicTacToe {
  public:
    // minimal window size
    static const int kMinHeight	    = 35;
    static const int kMinWidth	    = 40;
    // colors
    static const int kRed	    =  1;
    static const int kYellow	    =  2;
    static const int kGreen	    =  3;
    static const int kBlue	    =  4;
    static const int kMagenta	    =  5;
    static const int kCyan	    =  6;
    static const int kWhite	    =  7;
    static const int kLightRed	    =  9;
    static const int kLightGreen    = 10;
    static const int kLightYellow   = 11;
    static const int kLightCyan	    = 14;
    static const int kLightWhite    = 15;
    // game colors
    int human_color		    = kYellow;
    int machine_color		    = kBlue;
    int draw_color		    = kGreen;
    int play_again_color	    = kLightCyan;
    int yes_color		    = kLightYellow;
    int no_color		    = kLightRed;
    int input_rows_color	    = kLightCyan;
    int input_cols_color	    = kLightGreen;
    int input_win_color		    = kLightYellow;

    explicit TicTacToe(const int screen_rows, const int screen_cols);
    ~TicTacToe();
    void Run();

  private:
    // range field size to fit the screen
    const int kMinSize		    =  3;
    const int kMaxSize		    = 11;
    // map borders
    const int kMapLeft		    =  1;
    const int kMapUp		    =  8;
    const int kMapStep		    =  2;

    // default map borders for board min size = 3
    int map_right		    =  5;
    int map_down		    = 12;
    int map_width		    =  7;

    int rows, cols, start_row;

    enum Mark { empty, cross, zero, fill };

    struct Spot {
      int y;
      int x;
      int score;
    };

    struct Board {
      int size_y;
      int size_x;
      Mark** map; // game field
    } board;
    
    map <int, char> marks = {{0, ' '}, {1, 'X'}, {2, 'O'}};
    
    bool machine_first_move;
    bool is_over; // is game over?
    int to_win; // count in row to win
    Mark next; // who is next?
    Mark first; // who is first?
    Mark winner; // emp - draw, zero - 0 win, cross - X win
    Mark human, machine; // players

    // preparing the game
    void GameInit();
    void Init();
    void FreeMemory();
    // draw lots before start the game
    void DrawLots();
    void InitMap();

    void HumanMove();
    // machine moves functions
    void MachineMove();
    // unbeatable minimax algorithm for classic board 3*3 and 3 marks in row to win
    Spot MiniMax(const Mark& player, const int depth);
    // simple algorithm for bigs board
    void EasyAI(const Mark player, const Mark rival, int& y, int& x);
    // map out player's mark
    void MapOut(const Mark player, const int y, const int x);
    void FirstMove(int& y, int& x);
    
    // helpers
    void MakeWinner(const Mark player);
    bool DotIsValid(const int y, const int x);
    bool LineCheck(const Mark player, const int y, const int x, const int vx, const int vy, const int len);
    bool CheckDraw();
    bool WinCheck(const Mark player);
    void GameCheck();
};

#endif
