#ifndef DBMW_DAY11_HPP_
#define DBMW_DAY11_HPP_

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

typedef long long worry_t;
worry_t moduloSq(worry_t num2sq, worry_t modulo) {
    worry_t out = num2sq * num2sq;
    return out % modulo;
}
struct operation {
    char s;
    worry_t num;
    worry_t execute(worry_t old, worry_t modulo) {
        if (s == 'm') {
            return ((old * num) % modulo);
        } else if (s == 'p') {
            return moduloSq(old, modulo);
        } else {
            return ((old + num) % modulo);
        }
    }
    operation(char s, worry_t num) : s(s), num(num){};

    operation() : s('p'), num(1){};
};

struct monkey {
    deque<worry_t> items = {};
    operation op;
    worry_t divisible = 0;
    unsigned ifTrue = 0;
    unsigned ifFalse = 0;
    worry_t InspectionCount = 0;
    monkey(){};
};

deque<worry_t> splitNumber(string s) {
    deque<worry_t> result;
    s.erase(remove_if(s.begin(), s.end(), ::isspace), s.end());
    stringstream sStream(s);
    while (sStream.good()) {
        string substr;
        getline(sStream, substr, ',');
        result.push_back((worry_t) stoi(substr));
    }
    return result;
}
struct day11 {
    vector<monkey> Monkeys = {};
    vector<monkey> originalMonkeys = {};
    worry_t modulo = 1;
    day11(const std::string &st = "input/input11") {
        unsigned lineCount = 0;
        monkey newMonkey;
        string data = "";
        for (auto s: fiterator(st, '\n')) {
            unsigned monkeyInd = lineCount / 7;
            if (s != "") {
                unsigned remainder = lineCount % 7;
                unsigned start_of_data = s.find(":");
                auto preData = s.substr(0, start_of_data);
                if ((start_of_data + 1) != s.length()) {
                    data = s.substr(start_of_data + 1, s.length() - (start_of_data + 1));
                }
                if (remainder == 0) {
                    monkey newMonkey;
                    Monkeys.push_back(newMonkey);
                } else if (remainder == 1) {
                    auto items = splitNumber(data);
                    Monkeys[monkeyInd].items = items;
                } else if (remainder == 2) {
                    auto mult_loc = data.find('*');
                    auto plus_loc = data.find('+');
                    if (mult_loc != string::npos) {
                        auto rem =
                                data.substr(mult_loc + 1, data.length() - (mult_loc + 1));
                        rem.erase(remove_if(rem.begin(), rem.end(), ::isspace), rem.end());
                        if (rem == "old") {
                            Monkeys[monkeyInd].op = operation('p', 0);
                        } else {
                            Monkeys[monkeyInd].op = operation('m', stoi(rem));
                        }
                    } else {
                        auto rem =
                                data.substr(plus_loc + 1, data.length() - (plus_loc + 1));
                        rem.erase(remove_if(rem.begin(), rem.end(), ::isspace), rem.end());
                        Monkeys[monkeyInd].op = operation('s', stoi(rem));
                    }
                } else {
                    auto last_space = data.find_last_of(' ');
                    auto test = stoi(data.substr(last_space, data.length() - last_space));
                    if (remainder == 3) {
                        Monkeys[monkeyInd].divisible = (worry_t) test;
                    } else if (remainder == 4) {
                        Monkeys[monkeyInd].ifTrue = test;
                    } else if (remainder == 5) {
                        Monkeys[monkeyInd].ifFalse = test;
                    }
                }
            }
            lineCount += 1;
        }
        originalMonkeys = Monkeys;
        for (auto mon: Monkeys) {
            modulo *= mon.divisible;
        }
    }

    void runRound(bool fix = true, worry_t modulo = 100000000) {
        for (unsigned i = 0; i < Monkeys.size(); i++) {
            auto nitems = Monkeys[i].items.size();
            for (unsigned itemIdx = 0; itemIdx < nitems; itemIdx++) {
                worry_t worry = Monkeys[i].items[0];
                Monkeys[i].items.pop_front();
                worry_t newWorry = Monkeys[i].op.execute(worry, modulo);
                if (fix) {
                    newWorry = (worry_t) (newWorry / 3);
                }
                bool divBy = newWorry % Monkeys[i].divisible == 0;
                if (divBy) {
                    Monkeys[Monkeys[i].ifTrue].items.push_back(newWorry);
                } else {
                    Monkeys[Monkeys[i].ifFalse].items.push_back(newWorry);
                }
                Monkeys[i].InspectionCount += 1;
            }
        }
    }
    void firstStar() {
        for (unsigned i = 0; i < 20; i++) {
            runRound(true, modulo);
        }
        vector<unsigned> counts;
        for (auto mon: Monkeys) {
            counts.push_back(mon.InspectionCount);
        }
        sort(counts.rbegin(), counts.rend());
        std::cout << "Eleventh day of Christmas: " << counts[0] * counts[1]
                  << std::endl;
    }

    void secondStar() {
        Monkeys = originalMonkeys;
        for (unsigned i = 0; i < 10000; i++) {
            runRound(false, modulo);
        }
        vector<worry_t> counts = {};
        for (auto mon: Monkeys) {
            counts.push_back(mon.InspectionCount);
        }
        sort(counts.rbegin(), counts.rend());
        std::cout << "Eleventh day of Christmas: " << counts[0] * counts[1]
                  << std::endl;
    }
};

#endif