#include <iostream>
#include <ncurses.h>

#include "tictactoe.h"

using namespace std;

int main(int argc, char** argv) {
  // prepare nCurses lib
  initscr();
  crmode();
  keypad(stdscr, TRUE);
  noecho();

  // get current screen size
  int rows, cols;
  getmaxyx(stdscr, rows, cols);
  if (rows < TicTacToe::kMinHeight || cols < TicTacToe::kMinWidth) {
    endwin();
    cout << "Too small window" << endl
	 << "Please encrease window size and start again" << endl
	 << "Minimum window size is " << TicTacToe::kMinWidth << " x " << TicTacToe::kMinHeight << endl;
    return EXIT_SUCCESS;
  }

  // make the game
  TicTacToe* t = new TicTacToe(rows, cols);
  t->human_color = TicTacToe::kGreen;
  t->draw_color = TicTacToe::kYellow;
  t->Run();
  delete t;

  return EXIT_SUCCESS;
}
