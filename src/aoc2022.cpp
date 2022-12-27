#include "day1.hpp"
#include "day10.hpp"
#include "day11.hpp"
#include "day12.hpp"
#include "day13.hpp"
#include "day14.hpp"
#include "day15.hpp"
#include "day16.hpp"
#include "day17.hpp"
#include "day18.hpp"
#include "day19.hpp"
#include "day2.hpp"
#include "day20.hpp"
#include "day21.hpp"
#include "day22.hpp"
#include "day23.hpp"
#include "day24.hpp"
#include "day25.hpp"
#include "day3.hpp"
#include "day4.hpp"
#include "day5.hpp"
#include "day6.hpp"
#include "day7.hpp"
#include "day8.hpp"
#include "day9.hpp"
#include "util.hpp"
#include <chrono>
#include <iostream>

using namespace std;


typedef std::chrono::duration<float> fsec;
typedef chrono::time_point<std::chrono::system_clock> time_typ;
typedef std::chrono::system_clock Time;

void appendLine(int dayCount, ofstream *myfile, fsec p1) {
    *myfile << "|" << dayCount << "|" << p1.count() << "|" << endl;
}
int main(void) {
    ofstream myfile;
    myfile.open("times.md");
    myfile << "| Day | time |" << endl;
    myfile << "|--- | --- |" << endl;
    int dayCount = 0;
    auto t = timer();
    {
        day1 d;
        d.firstStar();
        d.secondStar();
    }
    dayCount++;
    appendLine(dayCount, &myfile, t.getIncrement());
    {
        day2 d;
        d.firstStar();
        d.secondStar();
    }
    dayCount++;
    appendLine(dayCount, &myfile, t.getIncrement());
    {
        day3 d;
        d.firstStar();
        d.secondStar();
    }
    dayCount++;
    appendLine(dayCount, &myfile, t.getIncrement());
    {
        day4 d;
        d.firstStar();
        d.secondStar();
    }

    dayCount++;
    appendLine(dayCount, &myfile, t.getIncrement());

    {
        day5 d;
        d.firstStar();
        d.secondStar();
    }
    dayCount++;
    appendLine(dayCount, &myfile, t.getIncrement());
    {
        day6 d;
        d.firstStar();
        d.secondStar();
    }
    dayCount++;
    appendLine(dayCount, &myfile, t.getIncrement());
    {
        day7 d;
        d.firstStar();
        d.secondStar();
    }
    dayCount++;
    appendLine(dayCount, &myfile, t.getIncrement());
    {
        day8 d;
        d.firstStar();
        d.secondStar();
    }
    dayCount++;
    appendLine(dayCount, &myfile, t.getIncrement());
    {
        day9 d;
        d.firstStar();
        d.secondStar();
    }
    dayCount++;
    appendLine(dayCount, &myfile, t.getIncrement());
    {
        day10 d;
        d.secondStar();
    }
    dayCount++;
    appendLine(dayCount, &myfile, t.getIncrement());
    {
        day11 d;
        d.firstStar();
        d.secondStar();
    }
    dayCount++;
    appendLine(dayCount, &myfile, t.getIncrement());
    {
        day12 d;
        d.firstStar();
        d.secondStar();
    }
    dayCount++;
    appendLine(dayCount, &myfile, t.getIncrement());
    {
        day13 d;
        d.firstStar();
        d.secondStar();
    }
    dayCount++;
    appendLine(dayCount, &myfile, t.getIncrement());
    {
        day14 d;
        d.firstStar();
        d.secondStar();
    }
    dayCount++;
    appendLine(dayCount, &myfile, t.getIncrement());
    {
        day15 d;
        d.firstStar();
        d.secondStar();
    }
    dayCount++;
    appendLine(dayCount, &myfile, t.getIncrement());
    {
        day16 d;
        d.firstStar();
        d.secondStar();
    }

    dayCount++;
    appendLine(dayCount, &myfile, t.getIncrement());
    {
        day17 d;
        d.firstStar();
        d.secondStar();
    }

    dayCount++;
    appendLine(dayCount, &myfile, t.getIncrement());
    {
        day18 d;
        d.firstStar();
        d.secondStar();
    }

    dayCount++;
    appendLine(dayCount, &myfile, t.getIncrement());
    {
        day19 d;
        d.firstStar();
        d.secondStar();
    }

    dayCount++;
    appendLine(dayCount, &myfile, t.getIncrement());
    {
        day20 d;
        d.firstStar();
        d.secondStar();
    }

    dayCount++;
    appendLine(dayCount, &myfile, t.getIncrement());
    {
        day21 d;
        d.firstStar();
        d.secondStar();
    }

    dayCount++;
    appendLine(dayCount, &myfile, t.getIncrement());
    {
        day22 d;
        d.firstStar();
        d.secondStar();
    }
    dayCount++;
    appendLine(dayCount, &myfile, t.getIncrement());

    {
        day23 d;
        d.firstStar();
        d.secondStar();
    }

    dayCount++;
    appendLine(dayCount, &myfile, t.getIncrement());
    {
        day24 d;
        d.firstStar();
        d.secondStar();
    }

    dayCount++;
    appendLine(dayCount, &myfile, t.getIncrement());
    {
        day25 d;
        d.firstStar();
        d.secondStar();
    }
    dayCount++;
    appendLine(dayCount, &myfile, t.getIncrement());

    myfile.close();
}
