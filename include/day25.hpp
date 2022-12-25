#ifndef DBMW_DAY25_HPP_
#define DBMW_DAY25_HPP_

#include "util.hpp"
#include <algorithm>
#include <bits/stdc++.h>
#include <boost/tokenizer.hpp>
#include <cctype>
#include <cmath>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;
struct day25 {
    vector<vector<long>> A;
    map<char, long> snafchar2d = {{'2', 2}, {'1', 1}, {'0', 0}, {'-', -1}, {'=', -2}};
    map<long, char> d2snafchar;

    day25(const std::string &st = "input/input25") {
        for (auto s: fiterator(st, '\n')) {
            if (s != "") {
                vector<long> tmp = {};
                for (auto i: s) {
                    tmp.push_back(snafchar2d.at(i));
                }
                A.push_back(tmp);
            }
        }
        map<char, long>::reverse_iterator it;
        // rbegin() returns to the last value of map
        for (it = snafchar2d.rbegin(); it != snafchar2d.rend(); it++) {
            d2snafchar.insert(make_pair(it->second, it->first));
        }
    }

    long evaluateSnaf(vector<long> snaf) {
        long n = snaf.size();
        long out = 0;
        if (snaf.size() == 0) {
            return 0;
        }
        for (long i = (n - 1); i >= 0; i--) {
            long coef = snaf[(n - 1) - i];
            long mult = pow(5, i);
            out += mult * coef;
        }
        return out;
    }

    long evaluateSnaf(deque<long> snaf) {
        long n = snaf.size();
        long out = 0;
        if (n == 0) {
            return 0;
        }
        for (long i = (n - 1); i >= 0; i--) {
            long coef = snaf[(n - 1) - i];
            long mult = pow(5, i);
            out += mult * coef;
        }
        return out;
    }
    inline long positive_mod(long i, long n) {
        return (i % n + n) % n;
    }
    deque<long> snafNum = {};
    long treeSearch(deque<long> initial_snaf, long goal) {
        // shouldn't be too bad
        // 314159265 is only 1T combinations;
        long init = evaluateSnaf(initial_snaf);
        long n = initial_snaf.size();
        if (n > 30) {
            return 0;
        }
        if (n > 2) {
            long mod = pow(5, n - 2);
            long goalm5 = positive_mod(goal, mod);
            long inittm5 = positive_mod(init, mod);
            if ((goalm5 != inittm5)) {
                return 0;
            }
        }
        if (init == goal) {
            snafNum = initial_snaf;
            return 1;
        } else {
            for (long i = -2; i <= 2; i++) {
                auto new_snaf = initial_snaf;
                new_snaf.push_front(i);
                auto c = treeSearch(new_snaf, goal);
                if (c == 1) {
                    return 1;
                }
            }
        }
        return 0;
    }


    void firstStar() {
        long totalScore = 0;
        for (long i = 0; i < A.size(); i++) {
            long round = evaluateSnaf(A[i]);
            totalScore += round;
        }
        deque<long> init = {};
        treeSearch(init, totalScore);
        string s = "";
        for (auto i: snafNum) {
            s += d2snafchar.at(i);
        }

        std::cout << "Christmas!: " << s << std::endl;
    }

    void secondStar() {
        unsigned totalScore = 0;
        std::cout << "Christmas!: " << totalScore << std::endl;
    }
};

#endif