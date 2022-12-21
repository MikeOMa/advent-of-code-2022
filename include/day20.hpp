#ifndef DBMW_DAY20_HPP_
#define DBMW_DAY20_HPP_

#include "util.hpp"
#include <algorithm>
#include <bits/stdc++.h>
#include <boost/circular_buffer.hpp>
#include <boost/tokenizer.hpp>
#include <cctype>
#include <cmath>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;
typedef long long int bigInt;
struct day20 {
    boost::circular_buffer<int> vec2buffer(vector<int> dat) {
        unsigned n = dat.size();
        boost::circular_buffer<int> out(n);
        for (unsigned i = 0; i < n; i++) {
            out.push_back(dat[i]);
        }
        return out;
    }
    boost::circular_buffer<int> data;
    unsigned n;
    struct Node {
        int left;
        int right;
        int num;
        int pos;
        Node(int left, int right, int num, int pos) : left(left), right(right), num(num), pos(pos) {
        }
    };
    vector<Node> linkedList;
    vector<Node> p2linkedList;

    void moveRightNode(int index_from) {
        auto thisNode = linkedList[index_from];
        auto right = thisNode.right;
        auto left = thisNode.left;

        // left node will point to the right
        linkedList[left].right = right;
        // right node will point to this node
        auto tmpRight = linkedList[right].right;
        linkedList[right].right = thisNode.pos;
        linkedList[right].left = left;
        //this node will point to right
        linkedList[index_from].left = right;
        linkedList[index_from].right = tmpRight;
        linkedList[tmpRight].left = thisNode.pos;
    }

    void moveLeftNode(int index_from) {
        auto thisNode = linkedList[index_from];
        auto right = thisNode.right;
        auto left = thisNode.left;
        // left node will point to the right
        linkedList[right].left = left;
        // right node will point to this node
        auto tmpLeft = linkedList.at(left).left;
        linkedList[left].left = thisNode.pos;
        linkedList[left].right = right;
        //this node will point to right
        linkedList[index_from].right = left;
        linkedList[index_from].left = tmpLeft;
        linkedList[tmpLeft].right = thisNode.pos;
    }

    int get_circular_index(int i) {
        return (i + n) % n;
    }
    day20(const std::string &st = "input/input20") {
        vector<int> temp_data;
        for (auto s: fiterator(st, '\n')) {
            int a = stoi(s);
            temp_data.push_back(a);
        }
        n = (int) temp_data.size();
        data = vec2buffer(temp_data);
        for (int i = 0; i < n; i++) {
            auto left = get_circular_index(i - 1);
            auto right = get_circular_index(i + 1);
            auto thisone = i;
            auto value = temp_data[thisone];
            linkedList.push_back(Node(left, right, value, thisone));
            p2linkedList.push_back(Node(left, right, value, thisone));
        }
    }

    int getElement(int howMany, int start = 0) {
        if (howMany == 0)
            return linkedList[start].num;
        else {
            auto next = linkedList[start].right;
            return getElement(howMany - 1, next);
        }
    }

    void printList(int start = 0) {
        auto initial = linkedList[start];

        for (int i = 0; i < n; i++) {
            auto next = initial.num;
            cout << next << " ";
            initial = linkedList[initial.right];
        }
        cout << endl;
    }
    int getLoc(int el) {
        auto out = 0;
        for (int i = 0; i < n; i++) {
            if (linkedList[i].num == el) {
                out = i;
                break;
            }
        }
        return out;
    }
    void firstStar() {
        unsigned totalScore = 0;
        for (auto i = 0; i < n; i++) {
            auto node = linkedList[i];
            int num = node.num;
            int nmoves = abs(num);
            for (int c = 0; c < nmoves; c++) {
                if (num < 0) {
                    moveLeftNode(i);
                } else if (num > 0) {
                    moveRightNode(i);
                }
            }
        }
        auto zeroLoc = getLoc(0);
        for (int i = 1; i <= 3; i++) {
            auto score = getElement(i * 1000, zeroLoc);
            totalScore += score;
        }
        std::cout << "Twentieth day of Christmas: " << totalScore << std::endl;
    }
    int sign(bigInt num) {
        if (num >= 0) {
            return 1;
        } else {
            return -1;
        }
    }
    void secondStar() {
        bigInt mult = 811589153;
        bigInt totalScore = 0;
        linkedList = p2linkedList;
        for (int j = 0; j < 10; j++) {
            for (auto i = 0; i < n; i++) {
                auto node = linkedList.at(i);
                bigInt num = ((bigInt) node.num) * mult;
                bigInt moves = num;
                bigInt nmoves = abs(moves);
                nmoves = nmoves % (n - 1);
                for (int c = 0; c < nmoves; c++) {
                    if (moves < 0) {
                        moveLeftNode(i);
                    } else if (moves > 0) {
                        moveRightNode(i);
                    }
                }
            }
        }
        // too low 1996161849
        // too low 3860939425
        //too low 4284224730

        auto zeroLoc = getLoc(0);
        for (int i = 1; i <= 3; i++) {
            auto num = getElement(i * 1000, zeroLoc);
            bigInt score = ((bigInt) num) * mult;
            totalScore += score;
        }
        std::cout << "Twentieth day of Christmas: " << totalScore << std::endl;
    }
};

#endif