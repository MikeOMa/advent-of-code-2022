#ifndef DBMW_DAY14_HPP_
#define DBMW_DAY14_HPP_

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
typedef pair<int, int> coord;
struct day14 {
    vector<vector<coord>> rockstructures;
    vector<vector<int>> rocksMap;
    vector<vector<int>> sandMap;
    pair<int, int> xrange = {0, 0};
    pair<int, int> yrange = {0, 0};
    int sign(int x) {
        int xc;
        if (x > 0) {
            xc = 1;
        } else if (x == 0) {
            xc = 0;
        } else {
            xc = -1;
        }
        return xc;
    }
    coord getVector(coord from, coord to) {
        int x = to.first - from.first;
        int y = to.second - from.second;
        coord out = {sign(x), sign(y)};
        return out;
    }
    day14(const std::string &st = "input/input14") {
        for (auto s: fiterator(st, '\n')) {
            if (s != "") {
                auto data = tokenize(s, {' '});
                vector<coord> tmp;
                for (auto i: data) {
                    if (i != "->") {
                        auto v = tokenize(i, {','});
                        coord x = {stoi(v[1]), stoi(v[0])};
                        tmp.push_back(x);
                        if (x.first < xrange.first) {
                            xrange.first = x.first;
                        }
                        if (x.first > xrange.second) {
                            xrange.second = x.first;
                        }
                        if (x.second < yrange.first) {
                            yrange.first = x.second;
                        }
                        if (x.second > yrange.first) {
                            yrange.second = x.second;
                        }
                    }
                }
                rockstructures.push_back(tmp);
            }
        }
        xrange.second += 2;
        for (int i = 0; i < xrange.second; i++) {
            vector<int> tmp;
            //+50 as a sand buffer
            for (int j = 0; j < (yrange.second + 400); j++) {
                if (i != xrange.second) {
                    tmp.push_back(0);
                } else {
                    tmp.push_back(1);
                }
            }
            rocksMap.push_back(tmp);
        }
        for (auto instructions: rockstructures) {
            coord initialPos = instructions[0];
            for (auto instruction: instructions) {
                coord vec = getVector(initialPos, instruction);
                while (initialPos != instruction) {
                    rocksMap[initialPos.first][initialPos.second] = 1;
                    initialPos.first += vec.first;
                    initialPos.second += vec.second;
                }
                rocksMap[initialPos.first][initialPos.second] = 1;
            }
        }
    }
    coord dropSand(vector<vector<int>> &Map) {
        coord pos = {0, 500};
        for (int i = 0; i < (xrange.second - 1); i++) {
            auto nextRow = Map[pos.first + 1];
            int next = nextRow[pos.second];
            int west = nextRow[pos.second - 1];
            int east = nextRow[pos.second + 1];
            if (next == 0) {
                pos.first += 1;
            } else if (west == 0) {
                pos.first += 1;
                pos.second += -1;
            } else if (east == 0) {
                pos.first += 1;
                pos.second += 1;
            } else {
                break;
            }
        }
        return pos;
    }
    bool isVoid(coord pos) {
        return pos.first > (xrange.second - 2);
    }
    void firstStar() {
        sandMap = rocksMap;
        int totalScore = 0;
        for (int i = 1; i < 1000; i++) {
            auto pos = dropSand(sandMap);
            if (isVoid(pos)) {
                totalScore = i - 1;
                break;
            }
            sandMap[pos.first][pos.second] = 1;
        }
        std::cout << "Fourteenth day of Christmas: " << totalScore << std::endl;
    }

    void secondStar() {
        int totalScore = 0;
        sandMap = rocksMap;
        for (int i = 1; i < 100000; i++) {
            auto pos = dropSand(sandMap);
            if ((pos.first == 0) and (pos.second == 500)) {
                totalScore = i;
                break;
            }
            sandMap[pos.first][pos.second] = 1;
        }
        std::cout << "Fourteenth Day of Christmas: " << totalScore << std::endl;
    }
};

#endif
