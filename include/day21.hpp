#ifndef DBMW_DAY21_HPP_
#define DBMW_DAY21_HPP_

#include "util.hpp"
#include <algorithm>
#include <bits/stdc++.h>
#include <boost/math/tools/polynomial.hpp>
#include <cctype>
#include <cmath>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>


using namespace boost::math;
using namespace boost::math::tools;// for polynomial
using namespace std;
using boost::lexical_cast;

typedef long num_t;
typedef long double p2type;
typedef polynomial<p2type> polydbl;

struct day21 {
    struct monkeyOperation {
        char s;
        num_t execute(num_t x, num_t y) {
            if (s == '/') {
                return x / y;
            } else if (s == '+') {
                return x + y;
            } else if (s == '*') {
                return x * y;
            } else {
                return x - y;
            }
        }

        polydbl execute(polydbl x, polydbl y) {
            if (s == '/') {
                return x / y;
            } else if (s == '+') {
                return x + y;
            } else if (s == '*') {
                return x * y;
            } else {
                return x - y;
            }
        }

        monkeyOperation(char s) : s(s){};

        monkeyOperation() : s('p'){};
    };

    struct monkeyNode {
        bool root = false;// 0 means no parent, so root monkey
        bool isnum;
        string name;
        string child_left = "";
        string child_right = "";
        monkeyOperation op;
        num_t num = 0;
        monkeyNode(string name, string child1, string child2, char op_char) : name(name),
                                                                              child_left(child1),
                                                                              child_right(child2),
                                                                              op(op_char) {
            if (name == "root") {
                root = true;
            }
            isnum = false;
        };
        monkeyNode(string name, num_t num) : name(name), op(), num(num) {
            if (name == "root") {
                root = true;
            }
            isnum = true;
        }
    };

    num_t execute(monkeyNode node) {
        if (node.isnum) {
            return node.num;
        } else {
            auto left_ans = execute(monkeyMap.at(node.child_left));
            auto right_ans = execute(monkeyMap.at(node.child_right));
            auto out = node.op.execute(left_ans, right_ans);
            return out;
        }
    }

    map<string, monkeyNode> monkeyMap = {};
    day21(const std::string &st = "input/input21") {
        for (auto s: fiterator(st, '\n')) {
            if (s != "") {
                auto data = tokenize(s);
                string name = data[0].substr(0, 4);
                if (data.size() > 2) {
                    auto op_str = data[2][0];
                    auto child_left = data[1];
                    auto child_right = data[3];
                    monkeyMap.insert({name, monkeyNode(name, child_left, child_right, op_str)});
                }
                if (data.size() == 2) {
                    auto num = stoi(data[1]);
                    monkeyMap.insert({name, monkeyNode(name, num)});
                }
            }
        }
    }

    void firstStar() {
        auto root = monkeyMap.at("root");
        num_t score = execute(root);
        std::cout << "Twenty first day of Christmas: " << score << std::endl;
    }


    polydbl smartExecute(monkeyNode node) {
        if (node.name == "humn") {
            polydbl out({0, 1});
            return out;
        }
        if (node.isnum) {
            polydbl out({(p2type) node.num, 0});
            return out;
        } else {
            auto left_ans = smartExecute(monkeyMap.at(node.child_left));
            auto right_ans = smartExecute(monkeyMap.at(node.child_right));
            auto out = node.op.execute(left_ans, right_ans);
            return out;
        }
    }

    void secondStar() {
        auto root = monkeyMap.at("root");
        auto leftMonkey = monkeyMap.at(root.child_left);
        auto rightMonkey = monkeyMap.at(root.child_right);
        monkeyMap.at("humn").num = 'x';
        auto l = smartExecute(leftMonkey);
        // should be 21830569590923
        auto r = smartExecute(rightMonkey);
        // low 3327575724808
        auto answer = ((r[0] - l[0]) / l[1]);
        cout.precision(17);
        std::cout << "Twenty first day of Christmas: " << answer << std::endl;
    }
};

#endif