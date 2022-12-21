#ifndef DBMW_DAY3_HPP_
#define DBMW_DAY3_HPP_

#include "util.hpp"
#include <bits/stdc++.h>
#include <iostream>
#include <string>
#include <vector>

struct day3 {
    vector<std::set<char>> A;
    vector<std::set<char>> firstRucksack;
    vector<std::set<char>> secondRucksack;

    unsigned char2Score(char x) {
        unsigned out = -1;
        if (isupper(x)) {
            out = x - 'A' + 27;
        } else
            out = x - 'a' + 1;
        return out;
    }

    day3(const std::string &st = "input/input3") {
        for (auto s: fiterator(st, '\n')) {
            if (s != "") {
                set<char> full_sack;
                for (unsigned i = 0; i < s.length(); i++) {
                    full_sack.insert(s[i]);
                }
                A.push_back(full_sack);
                unsigned N = s.length() / 2;
                set<char> compartment1;
                set<char> compartment2;
                for (unsigned i = 0; i < N; i++) {
                    compartment1.insert(s[i]);
                    compartment2.insert(s[i + N]);
                }

                firstRucksack.push_back(compartment1);
                secondRucksack.push_back(compartment2);
            }
        }
    }

    void firstStar() {
        unsigned totalScore = 0;
        for (unsigned e = 0; e < A.size(); e++) {
            auto compartment1 = firstRucksack[e];
            auto compartment2 = secondRucksack[e];
            set<char> intersect;
            std::set_intersection(compartment1.begin(), compartment1.end(),
                                  compartment2.begin(), compartment2.end(),
                                  std::inserter(intersect, intersect.begin()));
            for (auto i: intersect) {
                totalScore += char2Score(i);
            }
        }
        std::cout << "Third day of Christmas: " << totalScore << std::endl;
    }

    void secondStar() {

        unsigned totalScore = 0;
        for (unsigned e = 0; e < A.size(); e += 3) {
            std::set<char> set1 = A[e];
            for (unsigned row = 1; row < 3; row++) {
                auto set2 = A[e + row];
                std::set<char> intersect;
                std::set_intersection(set1.begin(), set1.end(), set2.begin(),
                                      set2.end(),
                                      std::inserter(intersect, intersect.begin()));
                set1 = intersect;
            }

            for (auto i: set1) {
                totalScore += char2Score(i);
            }
        }
        std::cout << "Third Day of Christmas: " << totalScore << std::endl;
    }
};

#endif