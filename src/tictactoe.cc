#include "tictactoe.h"

#include <ncurses.h>

#include "helpers.h"

using namespace std;

// public methods implementation
TicTacToe::TicTacToe(const int screen_rows, const int screen_cols) {
  rows = screen_rows;
  cols = screen_cols;
}

TicTacToe::~TicTacToe() {
  // end ncurses mode
  endwin();
}

void TicTacToe::Run() {
  GameInit();
  // main cycle
  while (true) {
    const int off_y = start_row + map_down;
    Init();
    DrawLots();
    InitMap();

    // game cycle
    while (true) {
      // who is next ?
      if (next == human) {
	HumanMove();
      } else {
	MachineMove();
      }

      // check game status
      GameCheck();

      // game is over ?
      if (is_over) {
	// is there a winner?
	if (!winner) {
	  mvprintwc(stdscr, off_y + 2, (cols - 4) / 2, draw_color, "Draw");
	} else {
	  if (winner == human) {
	    mvprintwc(stdscr, off_y + 2, (cols - 11) / 2, human_color, "Human wins!");
	  } else {
	    mvprintwc(stdscr, off_y + 2, (cols - 13) / 2, machine_color, "Machine wins!");
	  }
	}
	break;
      }
    }

    FreeMemory();
    // user input for play again
    string mesg = "Play again (y/n)?";
    const int off_x = (cols - mesg.length()) / 2;
    mvprintwc(stdscr, off_y + 4, off_x, play_again_color, mesg.c_str());
    mvprintwc(stdscr, off_y + 4, off_x + 12, yes_color, "y");
    mvprintwc(stdscr, off_y + 4, off_x + 14, no_color, "n");
    move(off_y + 4, off_x + 18);
    refresh();
    int key;
    do {
     key = getch();
    } while (key != ERR && key != 'n' && key != 'N' && key != 'y' && key != 'Y');
    // pressed char
    if (key == 'n' || key == 'N') {
      break;
    }
    if (key == 'y' || key == 'Y') {
      continue;
    }
  }
}


// private methods implementation
void TicTacToe::GameInit() {
  // turn on colors output to console
  if (has_colors()) {
    use_default_colors();
    start_color();
    init_pair(kRed, COLOR_RED, -1);
    init_pair(kYellow, COLOR_YELLOW, -1);
    init_pair(kGreen, COLOR_GREEN, -1);
    init_pair(kBlue, COLOR_BLUE, -1);
    init_pair(kMagenta, COLOR_MAGENTA, -1);
    init_pair(kCyan, COLOR_CYAN, -1);
    init_pair(kWhite, COLOR_WHITE, -1);
    init_pair(kLightRed, kLightRed, -1);
    init_pair(kLightYellow, kLightYellow, -1);
    init_pair(kLightGreen, kLightGreen, -1);
    init_pair(kLightCyan, kLightCyan, -1);
    init_pair(kLightWhite, kLightWhite, -1);
  }

  // user input board size and count marks in row to win
  int r = kMinSize, c = kMinSize, win = kMinSize;
  UserInputNCurses(stdscr, input_rows_color, r, kMinSize, kMaxSize, 
      "Please input number of rows in range [%d; %d] default [%d]: ", 
      kMinSize, kMaxSize, kMinSize);
  UserInputNCurses(stdscr, input_cols_color, c, kMinSize, kMaxSize, 
      "Please input number of cols in range [%d; %d] default [%d]: ", 
      kMinSize, kMaxSize, kMinSize);
  UserInputNCurses(stdscr, input_win_color, win, kMinSize, kMaxSize, 
      "Please input number of marks in row to win the game in range [%d; %d] default [%d]: ", 
      kMinSize, kMaxSize, kMinSize);

  // calculate vars for nice printing
  map_width = c * 2 + 1;
  map_right = c * kMapStep - 1;
  map_down = kMapUp + (r - 1) * kMapStep;
  start_row = (rows - (map_down + 5)) / 2;

  // init board size
  board.size_y = r;
  board.size_x = c;
  to_win = win;
  // allocate memory for board and init default properties
  Init();
}

void TicTacToe::Init() {
  // allocate memory for board
  board.map = new Mark* [board.size_y];
  for (int y = 0; y < board.size_y; ++y) {
    board.map[y] = new Mark[board.size_x];
    for (int x = 0; x < board.size_x; ++x) {
      board.map[y][x] = empty;
    }
  }

  // init properties
  is_over = false;
  machine_first_move = true;
  winner = empty;
  next = empty;
  first = empty;
  human = empty;
  machine = empty;
}

void TicTacToe::FreeMemory() {
  for (int i = 0; i < board.size_y; i++) {
    delete[] board.map[i];
  }
  delete[] board.map;
}

// draw lots
void TicTacToe::DrawLots() {
  Random r(0., 2.);
  human = (int) r.Real() ? cross : zero;
  machine = human == cross ? zero : cross;
  first = (int) r.Real() ? human : machine;
  next = first;
}

// nice printout of the lot results and the empty board
void TicTacToe::InitMap() {
  clear();
  // calculate offset macros
  // print welcome text
  move(start_row, (cols - 11) / 2);
  printwc(stdscr, human_color, "Tic ");
  printwc(stdscr, draw_color, "Tac ");
  printwc(stdscr, machine_color, "Toe");

  string mesg = "Draw lots..";
  mvprintw(start_row + 2, (cols - mesg.length()) / 2, mesg.c_str());
  
  // human color print
  mesg = " plays with ";
  mvprintwc(stdscr, start_row + 3, (cols - 18) / 2, human_color, "Human");
  printw(mesg.c_str());
  printwc(stdscr, human_color, "%c", marks[human]);

  // machine color print
  mvprintwc(stdscr, start_row + 4, (cols - 20) / 2, machine_color, "Machine");
  printw(mesg.c_str());
  printwc(stdscr, machine_color, "%c", marks[machine]);
 
  // who goes first color print
  mesg = " goes first whith ";
  if (first == human) {
    mvprintwc(stdscr, start_row + 6, (cols - 24) / 2, human_color, "Human");
    printw(mesg.c_str());
    printwc(stdscr, human_color, "%c", marks[first]);
  } else {
    mvprintwc(stdscr, start_row + 6, (cols - 26) / 2, machine_color, "Machine");
    printw(mesg.c_str());
    printwc(stdscr, machine_color, "%c", marks[first]);
  }
 
  // print map
  const int off_y = start_row + kMapUp;
  const int off_x = (cols - (board.size_x*2 + 1)) / 2;
  for (int y = 0; y < board.size_y; y++) {
    move(off_y + y*2, off_x);
    printwc(stdscr, kLightWhite, "|");
    for (int x = 0; x < board.size_x; x++) {
      printwc(stdscr, kLightWhite, " |");
    }
    if (y < board.size_y - 1) {
      move(off_y + y*2 + 1, off_x);
      printwc(stdscr, kLightWhite, "-");
      for (int x = 0; x < board.size_x; x++) {
        printwc(stdscr, kLightWhite, "--");
      }
    }
  }

  refresh();
}  


void TicTacToe::HumanMove() {
  const int left = (cols - map_width) / 2 + kMapLeft;
  const int right = (cols - map_width) / 2 + map_right;
  const int up = start_row + kMapUp;
  const int down = start_row + map_down;

  const int center_y = board.size_y / 2;
  const int center_x = board.size_x / 2;

  int x = 0, y = 0, key;
  int r = up + kMapStep * center_y, c = left + kMapStep * center_x;
  move(r, c);

  // user interactive action
  do {
    key = getch();
    switch (key) {
      case KEY_RIGHT: if (c < right) { c += kMapStep; move(r, c); } break;
      case KEY_LEFT: if (c > left) { c -= kMapStep; move(r, c); } break;
      case KEY_UP: if (r > up) { r -= kMapStep; move(r, c); } break;
      case KEY_DOWN: if (r < down) { r += kMapStep; move(r, c); } break;
      defaulf: break;
    };
    // convert to real coordinates
    y = (r - up) / kMapStep;
    x = (c - left) / kMapStep;
  } while (key != ERR && (key != '\n' || board.map[y][x] != empty));
 
  // map out
  MapOut(human, y, x);
  move(r, c);
  next = machine;
}

void TicTacToe::MachineMove() {
  int y = 0, x = 0;
  // machine first move and machine goes first ?
  if (machine_first_move && first == machine) {
    FirstMove(y, x);
    machine_first_move = false;
  } else {
    if (board.size_y == kMinSize && board.size_x == kMinSize && to_win == kMinSize) {
      // classic game 3*3 with 3 marks in row to win
      // minimax algorithm with recursion
      Spot spot = MiniMax(machine, 0);
      y = spot.y;
      x = spot.x;
    } else {
      // simple algorithm for big boards
      EasyAI(machine, human, y, x);
    }
  }
  // calculate real window coordicates
  const int r1 = y * kMapStep + start_row + kMapUp;
  const int c1 = x * kMapStep + (cols - map_width) / 2 + kMapLeft;
  move(r1, c1);
  MapOut(machine, y, x);
  next = human;
}

// unbeatable minimax algorithm for classic board 3*3 and 3 marks in row to win
TicTacToe::Spot TicTacToe::MiniMax(const Mark& player, const int depth) {
  // check for terminal states (win, lose, draw)
  if (WinCheck(human)) {
    Spot ret;
    ret.score = -100;
    return ret;
  } else if (WinCheck(machine)) {
    Spot ret;
    ret.score = 100 - depth;
    return ret;
  } else if (CheckDraw()) {
    is_over = false;
    Spot ret;
    ret.score = 0;
    return ret;
  }

  // vector to collect all moves
  vector<Spot> moves;
  for (int i = 0; i < board.size_y; i++) {
    for (int j = 0; j < board.size_x; j++) {
      if (board.map[i][j] == empty) {
	Spot move;
	move.y = i;
	move.x = j;
	// emulate player move
	board.map[i][j] = player;

	// if collect the score resulted from calling MiniMax on the rival of the player
	if (player == machine) {
	  const auto result = MiniMax(human, depth + 1);
	  move.score = result.score;
	} else {
	  const auto result = MiniMax(machine, depth + 1);
	  move.score = result.score;
	}

	// reset to empty
	board.map[i][j] = empty;

	moves.push_back(move);
      }
    }
  }

  // if it is the computer's turn loop over the moves and choose the move with the highest score
  int best_move;
  if (player == machine) {
    int best_score = -10000;
    for (int i = 0; i < moves.size(); i++) {
      if (moves[i].score > best_score) {
        best_score = moves[i].score;
        best_move = i;
      }
    }
  } else {
    // else loop over the moves and choose the move with the lowest score
    int best_score = 10000;
    for (int i = 0; i < moves.size(); i++) {
      if (moves[i].score < best_score) {
        best_score = moves[i].score;
        best_move = i;
      }
    }
  }

  // return the chosen move (object) from the array to the higher depth
  return moves[best_move];
}

// simple algorithm for bigs board
void TicTacToe::EasyAI(const Mark player, const Mark rival, int& y, int& x) {
  for (int i = 0; i < board.size_y; i++) {
    for (int j = 0; j < board.size_x; j++) {
      // if empty cell
      if (!board.map[i][j]) {
	
	// check player win
        board.map[i][j] = player;
	if (WinCheck(player)) {
	  y = i, x = j;
	  board.map[i][j] = empty;
	  return;
	}

	// check rival win
	board.map[i][j] = rival;
	if (WinCheck(rival)) {
	  y = i, x = j;
	  board.map[i][j] = empty;
	  return;
	}
        
	board.map[i][j] = empty;
      }
    }
  }
  Random ry(0., (double) board.size_y);
  Random rx(0., (double) board.size_x);
  do {
    y = ry.Real();
    x = rx.Real();
  } while (board.map[y][x]);
}

// map out player's mark
void TicTacToe::MapOut(const Mark player, const int y, const int x) {
  board.map[y][x] = player;
  printwc(stdscr, (player == human ? human_color : machine_color), "%c", marks[player]);
}

void TicTacToe::FirstMove(int& y, int& x) {
  y = board.size_y / 2;
  x = board.size_x / 2;
}

// helpers to check the game status
void TicTacToe::MakeWinner(const Mark player) {
  winner = player;
  is_over = true;
}

bool TicTacToe::DotIsValid(const int y, const int x) {
  return y >= 0 && y < board.size_y && x >= 0 && x < board.size_x;
}
 
bool TicTacToe::LineCheck(const Mark player, const int y, const int x, const int vx, const int vy, const int len) {
  // calculate end of line
  const int end_y = y + (len - 1) * vy;
  const int end_x = x + (len - 1) * vx;
  if (!DotIsValid(end_y, end_x)) {
    return false;
  }

  for (int i = 0; i < len; i++) {
    if (board.map[y + i*vy][x + i*vx] != player) {
      return false;
    }
  }

  return true;
}

bool TicTacToe::CheckDraw() {
  // do nothing if there is at least one empty cell
  for (int y = 0; y < board.size_y; y++) {
    for (int x = 0; x < board.size_x; x++) {
      if (board.map[y][x] == empty) {
        return false;
      }
    }
  }

  // otherwise set game over flag
  is_over = true;
  return true;
}

bool TicTacToe::WinCheck(const Mark player) {
  for (int y = 0; y < board.size_y; y++) {
    for (int x = 0; x < board.size_x; x++) {
      if (LineCheck(player, y, x, 1, 0, to_win)) {
	return true;
      }
      if (LineCheck(player, y, x, 1, 1, to_win)) {
	return true;
      }
      if (LineCheck(player, y, x, 0, 1, to_win)) {
	return true;
      }
      if (LineCheck(player, y, x, 1, -1, to_win)) {
	return true;
      }
    }
  }

  return false;
}

void TicTacToe::GameCheck() {
  if (WinCheck(human)) {
    MakeWinner(human);
    return;
  }

  if (WinCheck(machine)) {
    MakeWinner(machine);
    return;
  }

  CheckDraw();
}


