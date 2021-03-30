#include "helpers.h"

#include <iostream>
#include <iomanip>
#include <limits>
#include <cmath>
#include <ncurses.h>

#if defined(_WIN64) || defined(_WIN32)
#include <windows.h>
#endif

using namespace std;

// NCurses wrappers
// make color "mvprintw" from ncurses
void mvprintwc(WINDOW* win, const int y, const int x, const int color, const char* str, ...) {
  va_list args;
  va_start(args, str);
  move(y, x);
  if (has_colors()) {  
    attron(COLOR_PAIR(color));
  }
  vw_printw(win, str, args);
  if (has_colors()) {
    attroff(COLOR_PAIR(color));
  }
  va_end(args);
}

// make color "printw" from ncurses
void printwc(WINDOW* win, const int color, const char* str, ...) {
  va_list args;
  va_start(args, str);
  if (has_colors()) {
    attron(COLOR_PAIR(color));
  }
  vw_printw(win, str, args);
  if (has_colors()) {
    attroff(COLOR_PAIR(color));
  }
  va_end(args);
}

// make color "vw_printw" from ncurses
void vw_printwc(WINDOW* win, const int color, const char* str, va_list args) {
  va_list tmp;
  va_copy(tmp, args);
  if (has_colors()) {
    attron(COLOR_PAIR(color));
  }
  vw_printw(win, str, tmp);
  if (has_colors()) {
    attroff(COLOR_PAIR(color));
  }
  va_end(tmp);
}

// user color input (ncurses)
void UserInputNCurses(WINDOW* win, const int color, int& val, const int from, const int till, const char* str, ...) {
  int def = val;
  va_list args;
  va_start(args, str);
  echo();
  do {
    val = def;
    vw_printwc(win, color, str, args);
  } while (!scanw("%d", &val) || val < from || val > till);
  noecho();
  va_end(args);
}


// color strings struct cross-platform implementation
#if defined(__linux__) || defined(__APPLE__)
const string ColorString::kNoColor = "\033[0m";
const string ColorString::kRed = "\033[0;31m";
const string ColorString::kGreen = "\033[0;32m";
const string ColorString::kYellow = "\033[0;33m";
const string ColorString::kBlue = "\033[1;34m";
const string ColorString::kMagenta = "\033[1;35m";
const string ColorString::kCyan = "\033[1;36m";
const string ColorString::kWhite = "\033[1;37m";
#else
const string ColorString::kNoColor = "-1";
const string ColorString::kRed = "4";
const string ColorString::kGreen = "2";
const string ColorString::kYellow = "6";
const string ColorString::kBlue = "1";
const string ColorString::kMagenta = "5";
const string ColorString::kCyan = "3";
const string ColorString::kWhite = "15";
#endif

ColorString::ColorString(const char* s) {
  str = s;
  color = ColorString::kNoColor;
}

ColorString::ColorString(const string& s, const string c) {
  str = s;
  color = c;
}

ostream& operator<< (ostream& out, const ColorString& cs) {
#if defined(_WIN64) || defined(_WIN32)
  HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
  CONSOLE_SCREEN_BUFFER_INFO info;
  GetConsoleScreenBufferInfo(hConsole, &info);
  WORD attr = info.wAttributes;
  SetConsoleTextAttribute(hConsole, (cs.color == ColorString::kNoColor ? attr : stoi(cs.color, nullptr)));
  cout << cs.str;
  SetConsoleTextAttribute(hConsole, attr);
#else
  cout << cs.color << cs.str << ColorString::kNoColor;
#endif
  return out;
}


// timer struct implementation
Timer::Timer() : start(clock_t::now()) { 
}

void Timer::Reset() {
  start = clock_t::now();
}

double Timer::Elapsed() const {
  return chrono::duration_cast<second_t>(clock_t::now() - start).count();
}


// random generator struct implementation
Random::Random() {
  gen.seed(rd());
}

Random::Random(const int from, const int to) {
  gen.seed(rd());
  int_dist.param(uniform_int_distribution<int>::param_type(from, to));
}

Random::Random(const double from, const double to) {
  gen.seed(rd());
  double_dist.param(uniform_real_distribution<double>::param_type(from, to));
}

void Random::Interval(const int from, const int to) {
  int_dist.param(uniform_int_distribution<int>::param_type(from, to));
}

void Random::Interval(const double from, const double to) {
  double_dist.param(uniform_real_distribution<double>::param_type(from, to));
}

int Random::Integer() {
  return int_dist(gen);
}

double Random::Real() {
  return double_dist(gen);
}


// just helpers..
// user input integer
void UserInput(const char* str, int* val, const int from, const int till) {
  while ((cout << str) && (!(cin >> *val) || *val < from || *val > till)) {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
  }
}

// user input double
void UserInputDouble(const char* str, double* val, const double from, const double till) {
  while ((cout << str) && (!(cin >> *val) || *val < from || *val > till)) {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
  }
}

// double compare
bool IsEqual(const double x, const double y) {
  return fabs(x - y) < numeric_limits<double>::epsilon();
}

// double to string without trailing zeros
string Dbl2Str(const double d) {
    size_t len = snprintf(0, 0, "%.15f", d);
    string s(len+1, 0);
    // technically non-portable, see below
    snprintf(&s[0], len+1, "%.15f", d);
    // remove nul terminator
    s.pop_back();
    // remove trailing zeros
    s.erase(s.find_last_not_of('0') + 1, string::npos);
    // remove trailing point
    if(s.back() == '.')
      s.pop_back();
    return s;
}

// print array
void PrintArray(const char* str, const int array[], const int count) {
  cout << str << endl;
  for (int i = 0; i < count; i++) {
    cout << setw(6) << array[i];
  }
  cout << endl;
}
