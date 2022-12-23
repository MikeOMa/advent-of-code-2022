#ifndef DBMW_DAY22_HPP_
#define DBMW_DAY22_HPP_

#include "util.hpp"
#include <algorithm>
#include <bits/stdc++.h>
#include <boost/tokenizer.hpp>
#include <cctype>
#include <cmath>
#include <complex>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;
typedef complex<int> pos;
typedef complex<double> cpos;
struct day22 {
    vector<vector<unsigned>> mazeVec;
    string tmpInstructions;
    vector<string> instructions;
    vector<string> originalFile;
    vector<string> file;
    cpos centre = {24.5, 24.5};
    int nrows;
    int ncols;
    day22(const std::string &st = "input/input22") {
        map<char, unsigned> parseDict = {{' ', 0}, {'.', 1}, {'#', 2}};
        for (auto s: fiterator(st, '\n')) {
            if (s != "") {
                if (isdigit(s[0])) {
                    string tmp = "";
                    for (auto l: s) {
                        if (isdigit(l)) {
                            tmp = tmp + l;
                        } else {
                            instructions.push_back(tmp);
                            string turn(1, l);
                            instructions.push_back(turn);
                            tmp = "";
                        }
                    }
                    instructions.push_back(tmp);
                } else {
                    file.push_back(s);
                    vector<unsigned> tmp = {};
                    for (auto k: s) {
                        tmp.push_back(parseDict.at(k));
                    }
                    mazeVec.push_back(tmp);
                }
            }
        }

        nrows = mazeVec.size();
        ncols = mazeVec[0].size();
        for (int i = 0; i < nrows; i++) {
            int s = (int) mazeVec[i].size();
            for (int j = s; j < nrows; j++) {
                mazeVec[i].push_back(0);
            }
        }
        originalFile = file;
    }

    struct mover {
        complex<int> dir = {0, 1};
        complex<int> right = {0, -1};
        complex<int> left = {0, 1};
        int nrow;
        int ncol;
        mover(int nrows, int ncols) : nrow(nrows), ncol(ncols) {}
        void turn(char l) {
            if (l == 'L') {
                dir = dir * left;
            } else {
                dir = dir * right;
            }
        }
        pos move(pos x) {
            x += dir;
            if (x.real() == -1) {
                x.real(nrow - 1);
            } else if (x.real() == nrow) {
                x.real(0);
            }
            if (x.imag() == -1) {
                x.imag(ncol - 1);
            } else if (x.imag() == ncol) {
                x.imag(0);
            }
            return x;
        }
        char toChar() {
            string s = "rdlu";
            mover tmp(ncol, nrow);
            char out = 'x';
            for (int i = 0; i < 4; i++) {
                if (tmp.dir == dir) {
                    out = s[i];
                    break;
                }
                tmp.turn('R');
            }
            return out;
        }
    };

    mover fromChar(char x) {
        mover tmp(ncols, nrows);
        while (tmp.toChar() != x) {
            tmp.turn('R');
        }
        return tmp;
    }

    pos getInitialCoord() {
        auto firstRow = mazeVec[0];
        int posCounter = 0;
        for (auto i: firstRow) {
            if (i == 1) {
                break;
            }
            posCounter += 1;
        }
        pos out = {0, posCounter};
        return out;
    }
    unsigned loc(pos x) {
        return mazeVec[x.real()][x.imag()];
    }
    pos teleport(pos currentPos, mover *dir, unsigned v = 1) {
        auto nextPos = currentPos;
        if (v == 1) {
            auto flag = true;
            while (flag) {
                nextPos = (*dir).move(nextPos);
                if (loc(nextPos) != 0) {
                    flag = false;
                }
            }
        } else {
            auto cubeIdx = pos2cube(currentPos);
            auto x = nextCube(cubeIdx, *dir);
            unsigned cube = get<unsigned>(x);
            mover newDir = get<mover>(x);
            nextPos = (*dir).move(nextPos);
            auto crds = cubeCoords(nextPos, pos2cube(nextPos));
            auto newcrds = rotateFrame(crds, cube, (*dir).toChar(), newDir.toChar());

            nextPos = newcrds.toReal();
            if (loc(nextPos) != 2) {
                (*dir).dir = newDir.dir;
            }
        }
        if (loc(nextPos) == 2) {
            //if it's a wall return to where we were.
            return currentPos;
        } else {
            return nextPos;
        }
    }
    struct cubeCoords {
        unsigned cubeIdx;
        pos cubePos;
        cubeCoords(pos x, unsigned y) : cubeIdx(y) { cubePos = {x.real() % 50, x.imag() % 50}; };
        pos toReal() {
            int cubeCol = cubeIdx % 3;
            int cubeRow = (cubeIdx - cubeCol) / 3;
            pos out = {cubeRow * 50 + cubePos.real(), cubeCol * 50 + cubePos.imag()};
            return out;
        }
    };
    cubeCoords rotateFrame(cubeCoords x, unsigned newCube, char oldDir, char newDir) {
        auto m = fromChar(oldDir);
        cpos cx = {(double) x.cubePos.real(), (double) x.cubePos.imag()};
        auto centred = cx - centre;
        auto out = centred;
        cpos left = {0, 1};
        while (m.toChar() != newDir) {
            m.turn('L');
            out = left * out;
        }
        out = out + centre;
        pos l = {(int) round(out.real()), (int) round(out.imag())};
        x.cubePos = l;
        x.cubeIdx = newCube;
        return x;
    }
    pos walk(pos currentPos, int count, mover *dir, unsigned v = 1) {
        for (int i = 0; i < count; i++) {
            auto newPos = (*dir).move(currentPos);
            auto newSpace = loc(newPos);
            if (newSpace == 0) {
                newPos = teleport(currentPos, dir, v);
            } else if (newSpace == 2) {
                break;
            }
            currentPos = newPos;
            file[currentPos.real()][currentPos.imag()] = (*dir).toChar();
        }
        return currentPos;
    }
    tuple<mover, unsigned> nextCube(unsigned cubeIdx, mover dir) {
        char mChar = dir.toChar();
        char nDir = ' ';
        unsigned nxtCube;
        if (cubeIdx == 1) {
            if (mChar == 'u') {
                nxtCube = 9;
                nDir = 'r';
            } else {// l
                nxtCube = 6;
                nDir = 'r';
            }
        } else if (cubeIdx == 2) {
            switch (mChar) {
                case 'u':
                    nxtCube = 9;
                    nDir = 'u';
                    break;
                case 'r':
                    nxtCube = 7;
                    nDir = 'l';
                    break;
                case 'd':
                    nxtCube = 4;
                    nDir = 'l';
                    break;
            }
        } else if (cubeIdx == 4) {
            switch (mChar) {
                case 'r':
                    nxtCube = 2;
                    nDir = 'u';
                    break;
                case 'l':
                    nxtCube = 6;
                    nDir = 'd';
                    break;
            }
        } else if (cubeIdx == 7) {
            switch (mChar) {
                case 'r':
                    nxtCube = 2;
                    nDir = 'l';
                    break;
                case 'd':
                    nxtCube = 9;
                    nDir = 'l';
                    break;
            }
        } else if (cubeIdx == 6) {
            switch (mChar) {
                case 'u':
                    nxtCube = 4;
                    nDir = 'r';
                    break;
                case 'l':
                    nxtCube = 1;
                    nDir = 'r';
                    break;
            }
        } else if (cubeIdx == 9) {
            switch (mChar) {
                case 'l':
                    nxtCube = 1;
                    nDir = 'd';
                    break;
                case 'd':
                    nxtCube = 2;
                    nDir = 'd';
                    break;
                case 'r':
                    nxtCube = 7;
                    nDir = 'u';
                    break;
            }
        }
        auto newMover = fromChar(nDir);
        return {newMover, nxtCube};
    }
    unsigned getScore(pos x, mover m) {
        unsigned out = 0;
        out += (x.real() + 1) * 1000;
        out += (x.imag() + 1) * 4;
        mover tmp(nrows, ncols);
        unsigned dirScore = 0;
        for (int i = 0; i < 4; i++) {
            if (tmp.dir == m.dir) {
                dirScore = i;
                break;
            }
            tmp.turn('R');
        }
        out += dirScore;
        return out;
    }
    void firstStar() {
        mover p1(nrows, ncols);
        auto position = getInitialCoord();
        for (auto i: instructions) {
            if (isalpha(i[0])) {
                p1.turn(i[0]);
            } else {
                position = walk(position, stoi(i), &p1);
            }
        }
        unsigned totalScore = getScore(position, p1);
        // 19416 too low;
        std::cout << "TwentySecond day of Christmas: " << totalScore << std::endl;
    }

    unsigned pos2cube(pos x) {
        int cube_row = (int) x.real() / 50;
        int cube_col = (int) x.imag() / 50;
        return cube_col + 3 * cube_row;
    }
    void test1() {
        //tests
        pos testPos = {0, 54};
        mover testMover(nrows, ncols);
        testMover.turn('L');
        auto newPos = walk(testPos, 1, &testMover, 2);
        assert(newPos.real() == 154);
        assert(newPos.imag() == 0);
        assert(testMover.toChar() == 'r');
    }

    void test2() {
        //tests
        pos testPos = {0, 104};
        mover testMover(nrows, ncols);
        testMover.turn('L');
        auto newPos = walk(testPos, 3, &testMover, 2);
        assert(newPos.real() == 198);
        assert(newPos.imag() == 4);
        assert(testMover.toChar() == 'u');
    }
    void test3() {
        //tests
        pos testPos = {0, 108};
        mover testMover(nrows, ncols);
        testMover.turn('L');
        auto newPos = walk(testPos, 3, &testMover, 2);
        assert(newPos.real() == 0);
        assert(newPos.imag() == 108);
        assert(testMover.toChar() == 'u');
    }

    void test4() {
        //tests
        pos testPos = {8, 149};
        mover testMover(nrows, ncols);
        auto newPos = walk(testPos, 1, &testMover, 2);
        assert(testMover.toChar() == 'l');
        assert(newPos.real() == 141);
        assert(newPos.imag() == 99);
    }
    void secondStar() {
        file = originalFile;
        mover p1(nrows, ncols);
        auto position = getInitialCoord();
        test1();
        test2();
        test3();
        test4();
        int cnt = 0;
        for (auto i: instructions) {
            if (isalpha(i[0])) {
                p1.turn(i[0]);
            } else {
                position = walk(position, stoi(i), &p1, 2);
            }

            cnt++;
            if (cnt == 100) {
                fstream debug;
                debug.open("debug.out", ios_base::out);
                for (auto l: file)
                    debug << l << endl;
                debug.close();
            }
        }
        unsigned totalScore = getScore(position, p1);
        //2378 low;
        //21385 low;
        //98250 high;
        std::cout << "TwentySecond day of Christmas: " << totalScore << std::endl;
    }
};

#endif