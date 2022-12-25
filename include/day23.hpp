#ifndef DBMW_DAY23_HPP_
#define DBMW_DAY23_HPP_

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
typedef pair<int, int> coord;
typedef vector<coord> elfArr;
typedef set<pair<int, int>> elfSet;
struct day23 {
    elfArr initialElfLoc;
    const vector<int> pm1 = {-1, 0, 1};

    struct dirSearch {
        complex<int> constant;
        complex<int> multi;
        vector<complex<int>> searchspace = {};
        dirSearch(complex<int> x, complex<int> y) : constant(x), multi(y) {
            for (auto i: {-1, 0, 1}) {
                searchspace.push_back(constant + i * multi);
            }
        };
    };
    dirSearch north = {{-1, 0}, {0, 1}};
    dirSearch south = {{1, 0}, {0, 1}};
    dirSearch east = {{0, 1}, {1, 0}};
    dirSearch west = {{0, -1}, {1, 0}};
    vector<dirSearch> search = {north, south, west, east};
    day23(const std::string &st = "input/input23") {
        int row = 0;
        int col = 0;
        map<char, unsigned> parseDict = {{' ', 0}, {'.', 1}, {'#', 2}};
        for (auto s: fiterator(st, '\n')) {
            if (s != "") {
                col = 0;
                for (auto k: s) {
                    if (k == '#') {
                        initialElfLoc.push_back({row, col});
                    }
                    col += 1;
                }
                row += 1;
            }
        }
        //test tests;
        /*
            assert(contains(initialElfLoc, {0,4}));
            assert(contains(initialElfLoc, {1,3}));
            cout << initialElfLoc.size() << endl;
            printArr(initialElfLoc);
             */
    }

    coord addCoord(coord x, coord y) {
        coord out = {x.first + y.first, x.second + y.second};
        return out;
    }
    struct ArrSet {
        elfArr arr;
        set<coord> s;
        ArrSet(elfArr arr) : arr(arr) {
            for (coord i: arr) {
                s.insert(i);
            }
        }
    };
    bool contains(ArrSet x, coord y) {
        return x.s.contains(y);
    }
    void printArr(elfArr l) {
        auto size = getMaxes(l);
        cout << size[2] << size[3] << endl;
        for (int i = size[0]; i <= size[1]; i++) {
            for (int j = size[2]; j <= size[3]; j++) {
                if (contains(l, {i, j})) {
                    cout << '#';
                } else {
                    cout << '.';
                }
            }
            cout << endl;
        }
    }

    bool contains(elfArr x, coord y) {
        auto k = find(x.begin(), x.end(), y);
        auto out = k != x.end();
        return out;
    }
    bool anySurroundings(coord x, ArrSet y) {
        bool out = false;
        for (int i: {-1, 0, 1}) {
            for (int j: {-1, 0, 1}) {
                auto newProposal = addCoord(x, {i, j});
                if (not((i == 0) & (j == 0))) {
                    auto flag = contains(y, {newProposal});
                    out = out | flag;
                    if (out) {
                        break;
                    }
                }
            }
            if (out) {
                break;
            }
        }
        return out;
    }

    bool checkDirection(ArrSet elfs, coord x, dirSearch y) {
        // return false if nothing is found
        bool out = false;
        for (auto i: y.searchspace) {
            coord trial = {i.real() + x.first, i.imag() + x.second};
            out = out | contains(elfs, trial);
            if (out) {
                break;
            }
        }
        return out;
    }


    elfArr buildProposal(ArrSet elfs, int turn_number) {
        elfArr proposal = {};
        auto start = turn_number % 4;
        for (int i = 0; i < elfs.arr.size(); i++) {
            auto elf = elfs.arr[i];
            if (not anySurroundings(elf, elfs)) {
                proposal.push_back(elf);
            } else {
                bool moved = false;
                for (int diridx = 0; diridx < 4; diridx++) {
                    auto mod4 = (start + diridx) % 4;
                    auto dir = search[mod4];
                    auto check = not checkDirection(elfs, elf, dir);
                    if (check) {
                        coord move = {elf.first + dir.constant.real(), elf.second + dir.constant.imag()};
                        proposal.push_back(move);
                        moved = true;
                        break;
                    }
                }
                if (not moved) {
                    proposal.push_back(elf);
                }
            }
        }
        return proposal;
    }

    elfArr pruneProposal(ArrSet elfs, elfArr proposal) {
        //check if all moves do not overlap;
        elfArr newProposal = proposal;
        set<coord> proposalSet = {};
        set<coord> duplicates = {};
        for (auto i: proposal) {
            if (not proposalSet.contains(i)) {
                proposalSet.insert(i);
            } else {
                duplicates.insert(i);
            }
        }
        for (int i = 0; i < elfs.arr.size(); i++) {
            if (duplicates.contains(proposal[i])) {
                newProposal[i] = elfs.arr[i];
            }
        }
        return newProposal;
    }

    vector<int> getMaxes(elfArr x) {
        vector<int> out = {0, 0, 0, 0};
        for (int i = 0; i < x.size(); i++) {
            auto elf = x[i];
            if (elf.first < out[0])
                out[0] = elf.first;
            if (elf.first > out[1])
                out[1] = elf.first;
            if (elf.second < out[2])
                out[2] = elf.second;
            if (elf.second > out[3])
                out[3] = elf.second;
        }
        return out;
    }
    int calculateScore(elfArr x) {
        auto range = getMaxes(x);
        auto spaces = (range[1] - range[0] + 1) * (range[3] - range[2] + 1);
        int out = spaces - x.size();
        return out;
    }
    bool checkProposalIsNone(elfArr proposal, elfArr elfs) {
        bool out = true;
        for (int i = 0; i < elfs.size(); i++) {
            out = out & (proposal[i] == elfs[i]);
            if (not out) {
                break;
            }
        }
        return out;
    }
    void firstStar() {
        ArrSet elfConfig = {initialElfLoc};
        cout << calculateScore(elfConfig.arr) << endl;
        for (int i = 0; i < 10; i++) {
            auto proposal = buildProposal(elfConfig, i);
            auto prunedProposal = pruneProposal(elfConfig, proposal);
            elfConfig = prunedProposal;
        }
        int totalScore = calculateScore(elfConfig.arr);
        std::cout << "TwentyThird day of Christmas: " << totalScore << std::endl;
    }

    void secondStar() {
        int totalScore = 0;
        ArrSet elfConfig = {initialElfLoc};
        cout << calculateScore(elfConfig.arr) << endl;
        for (int i = 0; i < 2000; i++) {
            totalScore = i;
            auto proposal = buildProposal(elfConfig, i);
            auto prunedProposal = pruneProposal(elfConfig, proposal);
            if (checkProposalIsNone(prunedProposal, elfConfig.arr)) {
                break;
            }
            elfConfig = ArrSet(prunedProposal);
            cout << i << endl;
        }
        std::cout << "TwentyThird day of Christmas: " << totalScore << std::endl;
    }
};

#endif