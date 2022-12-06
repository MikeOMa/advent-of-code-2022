#ifndef DBMW_DAY6_HPP_
#define DBMW_DAY6_HPP_

#include"util.hpp"
#include<vector>
#include<string>
#include<iostream>
#include <bits/stdc++.h>

bool check_unique(string k){
       // If at any time we encounter 2
    // same characters, return false
    unsigned n = k.length();
    for (unsigned i = 0; i < n - 1; i++) {
        for (unsigned idx = i + 1; idx < n; idx++) {
            if (k[i] == k[idx]) {
                return true;
            }
        }
    }
    return false;
}
struct day6 {
    string input;

    day6(const std::string &st = "input/input6") {
        for (auto s: fiterator(st, '\n')) {
            input = s;
            }
        }






        void firstStar() {
        unsigned totalScore = 0;
        for(unsigned i = 4; i < input.size()-4; i++){
            string substr = input.substr(i-4, 4);
            auto allUnique = check_unique(substr);
            if (not allUnique){
                totalScore= i;
                break;
            }
        }
        std::cout << "Sixth day of Christmas: " << totalScore << std::endl;
        }


        void secondStar() {
            unsigned totalScore = 0;
            for(unsigned i = 14; i < input.size()-14; i++){
                string substr = input.substr(i-14, 14);
                auto allUnique = check_unique(substr);
                if (not allUnique){
                    totalScore= i;
                    break;
                }
            }
            std::cout << "Sixth day of Christmas: " << totalScore << std::endl;

        }
};


#endif