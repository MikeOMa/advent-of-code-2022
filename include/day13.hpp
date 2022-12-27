#ifndef DBMW_DAY13_HPP_
#define DBMW_DAY13_HPP_

#include "util.hpp"
#include <algorithm>
#include <bits/stdc++.h>
#include <boost/tokenizer.hpp>
#include <cctype>
#include <cmath>
#include <deque>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

struct day13 {
    struct item {
        int num = -1;
        vector<item> data = {};
        bool root = false;
        item(int num) : num(num){};
        item(string num) : num(stoi(num)){};
        item(){};
    };

    tuple<item, int> readItem(string s, int pos) {
        item out;
        string tempStr = "";
        for (int i = pos; i < s.size(); i++) {
            char c = s[i];
            if (c == '[') {
                auto k = readItem(s, i + 1);
                auto it = get<item>(k);
                out.data.push_back(it);
                auto newpos = get<int>(k);
                i = newpos;
            } else if (c == ']') {
                if (tempStr != "") {
                    item newItem = {tempStr};
                    out.data.push_back(newItem);
                }
                return {out, i};
            } else if (c == ',') {
                if (tempStr != "") {
                    item newItem = {tempStr};
                    out.data.push_back(newItem);
                    tempStr = "";
                }
            } else {
                tempStr += c;
            }
        }
        return {out, s.size()};
    }

    vector<item> leftData = {};
    vector<item> rightData = {};
    vector<item> allItems = {};
    item div1 = get<item>(readItem("[[2]]", 1));
    item div2 = get<item>(readItem("[[6]]", 1));
    day13(const std::string &st = "input/input13") {
        int lineCount = 0;
        item tmpItem;
        for (auto s: fiterator(st, '\n')) {
            if (s != "") {
                auto k = readItem(s, 1);
                tmpItem = get<item>(k);
            }
            if (lineCount % 3 == 0) {
                leftData.push_back(tmpItem);
                allItems.push_back(tmpItem);
            } else if (lineCount % 3 == 1) {
                rightData.push_back(tmpItem);
                allItems.push_back(tmpItem);
            }
            lineCount += 1;
        }
        allItems.push_back(div1);
        allItems.push_back(div2);
    }

    int compareItem(item leftItem, item rightItem) {
        // 1  if left is smaller than right.
        // 0 for tie.
        // -1 for right smaller than left;

        bool leftNum = leftItem.num != -1;
        bool rightNum = rightItem.num != -1;
        int out;
        if (leftNum & rightNum) {
            if (leftItem.num < rightItem.num) {
                return 1;
            } else if (leftItem.num == rightItem.num) {
                return 0;
            } else {
                return -1;
            }
        } else if ((!leftNum) & (!rightNum)) {
            // both are lists
            int nleft = leftItem.data.size();
            int nright = rightItem.data.size();
            if ((nright == 0) & (nleft > 0)) {
                out = -1;
            } else if ((nright > 0) & (nleft == 0)) {
                out = 1;
            } else if ((nright == 0) & (nleft == 0)) {
                out = 0;
            } else {
                for (int i = 0; i < nleft; i++) {
                    if ((nright - 1) < i) {
                        // right side ran out
                        out = -1;
                    } else {
                        out = compareItem(leftItem.data[i], rightItem.data[i]);
                    }
                    if (out != 0) {
                        return out;
                    }
                }
                //run out of items and right has some left
                if ((out == 0) & (nright > nleft)) {
                    return 1;
                }
            }
        } else {
            // one is a number the other is not
            if (leftNum) {
                item l;
                l.data.push_back(leftItem);
                out = compareItem(l, rightItem);
            } else {
                item r;
                r.data.push_back(rightItem);
                out = compareItem(leftItem, r);
            }
        }
        return out;
    }
    string item2str(item x) {
        stringstream s;
        if (x.num != -1) {
            s << x.num << " ";
        } else {
            s << '[';
            for (auto i: x.data) {
                s << item2str(i);
            }
            s << ']';
        }
        return s.str();
    }
    void firstStar() {
        unsigned totalScore = 0;
        for (unsigned i = 1; i <= leftData.size(); i++) {
            int flag = compareItem(leftData[i - 1], rightData[i - 1]);
            if (flag == 1) {
                totalScore += i;
            }
        }
        //6104 too high;
        //5796 too high
        std::cout << "Thirteenth day of Christmas: " << totalScore << std::endl;
    }
    int getIdx(item a, vector<item> l) {
        int out = -1;
        for (int i = 0; i < l.size(); i++) {
            auto k = l[i];
            if (compareItem(k, a) == 0) {
                out = i;
                break;
            }
        }
        return out;
    }
    void secondStar() {

        sort(allItems.begin(), allItems.end(), [this](item a, item b) {
            return compareItem(a, b) == 1;
        });
        auto i = getIdx(div1, allItems) + 1;
        auto j = getIdx(div2, allItems) + 1;
        int totalScore = i * j;
        std::cout << "Thirteenth Day of Christmas: " << totalScore << std::endl;
    }
};

#endif
