#ifndef DBMW_DAY17_HPP_
#define DBMW_DAY17_HPP_

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

struct day17 {
    typedef vector<vector<bool>> rockMap;
    vector<char> stream;
    typedef pair<int, int> coord;
    int streamSize;

    day17(const std::string &st = "input/input17") {
        for (auto s: fiterator(st, '\n')) {
            if (s != "") {
                for (auto i: s) {
                    stream.push_back(i);
                }
            }
        }
        streamSize = stream.size();
    }
    // 0,0 being the lowest location/column;
    const vector<coord> flat = {{0, 0}, {0, 1}, {0, 2}, {0, 3}};
    const vector<coord> L = {{0, 0}, {0, 1}, {0, 2}, {1, 2}, {2, 2}};
    const vector<coord> plus = {{0, 1}, {1, 1}, {2, 1}, {1, 0}, {1, 2}};
    const vector<coord> ud = {{0, 0}, {1, 0}, {2, 0}, {3, 0}};
    const vector<coord> sq = {{0, 0}, {1, 0}, {1, 1}, {0, 1}};

    const vector<vector<coord>> shapes = {flat, plus, L, ud, sq};

    map<char, coord> move = {
            {'<', {0, -1}},
            {'>', {0, 1}},
            {'d', {-1, 0}}};
    struct Shape {
        vector<coord> x;
        coord pos;// location of 0,0;

        map<char, coord> moveMap = {
                {'<', {0, -1}},
                {'>', {0, 1}},
                {'d', {-1, 0}}};
        Shape(vector<coord> x, int arrsize) : x(x), pos({arrsize + 4, 2}){};

        vector<coord> moveProposal(char d) {
            coord dir = moveMap.at(d);
            vector<coord> out;
            for (auto i: x) {
                int x = i.first + pos.first + dir.first;
                int y = i.second + pos.second + dir.second;
                coord next = {x, y};
                out.push_back(next);
            }
            return out;
        }

        vector<coord> points() {
            vector<coord> out;
            for (auto i: x) {
                int x = i.first + pos.first;
                int y = i.second + pos.second;
                coord next = {x, y};
                out.push_back(next);
            }
            return out;
        }
        bool move(char d, vector<vector<bool>> boolMap) {
            auto next = moveProposal(d);
            bool isOK = true;
            int size = boolMap[0].size();
            for (auto i: next) {
                bool checkVldCoord = (i.first >= 0) & (i.second >= 0) & (i.second < size);
                if (checkVldCoord) {
                    isOK = isOK & (not boolMap[i.first][i.second]);
                } else {
                    isOK = false;
                }
                if (not isOK) {
                    break;
                }
            }
            if (isOK) {
                auto tmp = moveMap.at(d);
                pos.first += tmp.first;
                pos.second += tmp.second;
            }
            return isOK;
        }
    };
    int p1 = 0;
    int findHighestPoint(rockMap x) {
        if (x.size() == 0) {
            return -1;
        }
        for (int i = (x.size() - 1); i >= 0; i--) {
            for (auto k: x[i]) {
                if (k) {
                    return i;
                }
            }
        }
        return 0;
    }
    vector<bool> makeRow(int w) {
        vector<bool> out;
        for (int i = 0; i < w; i++) {
            out.push_back(false);
        }
        return out;
    }
    int streamIdx = 0;
    void dropShape(rockMap &x, int turn, int w = 7) {
        auto highest = findHighestPoint(x);
        auto shp = shapes[turn % 5];
        auto shapeToDrop = Shape(shp, highest);
        for (int i = (x).size(); i < (shapeToDrop.pos.first) + 4; i++) {
            auto tmp = makeRow(w);
            (x).push_back(tmp);
        }
        for (int i = 0; i < x.size(); i++) {
            char d = stream[streamIdx];
            shapeToDrop.move(d, x);
            streamIdx = (streamIdx + 1) % streamSize;
            auto moved = shapeToDrop.move('d', x);
            if (not moved) {
                auto pnts = shapeToDrop.points();
                for (auto k: pnts) {
                    x[k.first][k.second] = true;
                }
                break;
            }
        }
    }
    void print(rockMap x) {
        for (int i = (x.size() - 1); i >= 0; i--) {
            auto row = x[i];
            for (auto i: row) {
                if (i) {
                    cout << "#";
                } else {
                    cout << '.';
                }
            }
            cout << endl;
        }
    }
    void firstStar() {
        vector<vector<bool>> map;
        for (int step = 0; step < 2022; step++) {
            dropShape(map, step);
        }
        for (int i = 0; i < map.size(); i++) {
            bool out = true;
            for (auto k: map[i]) {
                out = out & k;
            }
        }
        int totalScore = findHighestPoint(map) + 1;
        std::cout << "Seventeenth day of Christmas: " << totalScore << std::endl;
    }
    typedef vector<int> hashedState;
    int hashRow(vector<bool> x) {
        int out = 0;
        for (int i = 0; i < x.size(); i++) {
            out += pow(2, i) * x[i];
        }
        return out;
    }
    hashedState hashState(rockMap map, int n, int turn) {
        vector<int> out;
        out.push_back(streamIdx);
        out.push_back(turn % 5);
        int h = findHighestPoint(map);
        for (int i = h; i > (h - n); i--) {
            if (map.size() > i) {
                out.push_back(hashRow(map[i]));
            }
        }
        return out;
    }
    bool checkIfEven(rockMap &map) {
        bool out = true;
        int h = findHighestPoint(map);
        for (auto k: map[h]) {
            out = out & k;
        }
        return out;
    }
    bool vecContains(vector<hashedState> vec, hashedState key) {
        return (find(vec.begin(), vec.end(), key) != vec.end());
    }
    void secondStar() {
        streamIdx = 0;
        vector<vector<bool>> map;
        vector<long> h;
        int hashSize = 20;
        vector<hashedState> statesSet;
        hashedState cyclicState;
        long long repeat = 0;
        for (int step = 0; step < 3000; step++) {
            dropShape(map, step);
            auto hState = hashState(map, hashSize, step);
            h.push_back(findHighestPoint(map));
            if (vecContains(statesSet, hState)) {
                cyclicState = hState;
                repeat = (long long) step;
                break;
            } else {
                statesSet.push_back(hState);
            }
        }
        long long first = 0;
        for (auto el: statesSet) {
            if (el == cyclicState) {
                break;
            }
            first++;
        }

        long long cycleLen = repeat - first;
        long long heightAtfirst = h[first];
        long long heightAtRepeat = h[repeat];
        long long heightPerCycle = heightAtRepeat - heightAtfirst;
        long long target = 1000000000000;
        long long totalScore = heightAtfirst;
        auto rem = target - first;
        auto n_cycles = (long long) rem / cycleLen;
        totalScore += n_cycles * heightPerCycle;
        rem = rem - (n_cycles * cycleLen);
        totalScore += (h[first + rem] - h[first]);

        // 1532163742793 too high
        std::cout << "Seventeenth day of Christmas: " << totalScore << std::endl;
    }
};

#endif