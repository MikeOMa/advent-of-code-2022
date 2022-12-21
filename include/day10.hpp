#ifndef DBMW_DAY10_HPP_
#define DBMW_DAY10_HPP_

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

struct day10 {
    vector<unsigned> instructions;
    vector<int> values;

    day10(const std::string &st = "input/input10") {
        for (auto s: fiterator(st, '\n')) {
            if (s != "") {
                auto space = s.find(" ");
                if (space == string::npos) {
                    instructions.push_back(0);
                    values.push_back(0);
                } else {
                    instructions.push_back(1);
                    values.push_back(stoi(s.substr(space + 1, s.length() - space)));
                }
            }
        }
    }

    void secondStar() {
        unsigned a = 0;
        unsigned cycleBuffer = 0;
        int pos = 0;
        int xvalue = 1;
        unsigned cycle = 1;
        vector<int> track = {};
        while (a < instructions.size()) {
            auto instruction = instructions[a];
            if ((instruction == 1) & (cycleBuffer == 0)) {
                // If addx is starting:
                cycleBuffer = 2;
            }
            if (instruction == 0) {
                cycleBuffer = 1;
            }
            if (abs(xvalue - pos) <= 1) {
                cout << "#";
            } else {
                cout << ".";
            }
            pos += 1;
            if (pos % 40 == 0) {
                cout << endl;
                pos = 0;
            }

            if ((cycle - 20) % 40 == 0) {
                track.push_back(xvalue * cycle);
            }
            cycleBuffer += -1;
            cycle += 1;
            if (cycleBuffer == 0) {
                if (instruction == 1) {
                    xvalue += values[a];
                }
                a += 1;
            }
        }
        auto totalScore = accumulate(track.begin(), track.end(), 0);
        cout << "Tenth day of Christmas: " << totalScore << std::endl;
    }
};

#endif