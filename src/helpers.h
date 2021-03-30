#ifndef HELPERS_H
#define HELPERS_H

#include <chrono>
#include <random>
#include <functional>
#include <ncurses.h>

using namespace std;

// NCurses wrappers
// make color "mvprintw" from ncurses
void mvprintwc(WINDOW* win, const int y, const int x, const int color, const char* str, ...);
// make color "printw" from ncurses
void printwc(WINDOW* win, const int color, const char* str, ...);
// make color "vw_printw" from ncurses
void vw_printwc(WINDOW* win, const int color, const char* str, va_list args);
// user color input (ncurses)
void UserInputNCurses(WINDOW* win, const int color, int& val, const int from, const int till, const char* str, ...);


// color strings struct cross-platform implementation
struct ColorString {
  private:
    string str;
    string color;

  public:
#if defined(__linux__) || defined(__APPLE__)
    static const string kNoColor;
    static const string kRed;
    static const string kGreen;
    static const string kYellow;
    static const string kBlue;
    static const string kMagenta;
    static const string kCyan;
    static const string kWhite;
#else
    static const string kNoColor;
    static const string kRed;
    static const string kGreen;
    static const string kYellow;
    static const string kBlue;
    static const string kMagenta;
    static const string kCyan;
    static const string kWhite;
#endif
    ColorString(const char* s);
    explicit ColorString(const string& s, const string c);
    friend ostream& operator<< (ostream& out, const ColorString& cs);
};


// timer struct implementation
struct Timer {
  private:
    using clock_t = chrono::high_resolution_clock;
    using second_t = chrono::duration<double, ratio<1>>;

    chrono::time_point<clock_t> start;

  public:
    Timer();
    void Reset();
    double Elapsed() const;
};


// random generator struct implementation
struct Random {
  private:
    using int_r = uniform_int_distribution<int>;
    using double_r = uniform_real_distribution<double>;
    random_device rd;
    mt19937 gen;
    uniform_int_distribution<int> int_dist;
    uniform_real_distribution<double> double_dist;
    
  public:
    explicit Random();
    explicit Random(const int from, const int to); 
    explicit Random(const double from, const double to);
    void Interval(const int from, const int to);
    void Interval(const double from, const double to);
    int Integer();
    double Real();
};


// just helpers..
// user input integer
void UserInput(const char* str, int* val, 
    const int from=numeric_limits<int>::min(), 
    const int till=numeric_limits<int>::max());

// user input double
void UserInputDouble(const char* str, double* val, 
    const double from=numeric_limits<double>::lowest(), 
    const double till=numeric_limits<double>::max());

// compare double
bool IsEqual(const double x, const double y);

// double to string without trailing zeros
string DblToStr(const double d);

// print array
void PrintArray(const char* str, const int array[], const int count);

#endif
