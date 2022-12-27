#ifndef DBMW_INCLUDE_UTIL_HPP_
#define DBMW_INCLUDE_UTIL_HPP_

#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <queue>
#include <string>
#include <vector>

using namespace std;

typedef std::chrono::duration<float> fsec;
typedef chrono::time_point<std::chrono::system_clock> time_typ;
typedef std::chrono::system_clock Time;
struct timer {
    time_typ t;
    timer() { t = Time::now(); };
    fsec getIncrement() {
        time_typ tnow = Time::now();
        fsec delta = tnow - t;
        t = tnow;
        return delta;
    }

    void print() {
        auto x = getIncrement();
        cout << "time " << x.count() << endl;
    }
};

struct fiterator {

    struct _iter {
        fiterator *src;

        std::string operator*() const;
        _iter &operator++();
        bool operator!=(const _iter &other) const;
    };

    fiterator(const std::string &filename, const char &separator = ' ');

    _iter begin();
    _iter end();
    std::string next();

    char sep;
    std::ifstream f;
};


std::vector<std::string> tokenize(const std::string &st,
                                  std::vector<char> separators = {' '});

#endif