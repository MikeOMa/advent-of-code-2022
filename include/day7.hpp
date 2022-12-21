#ifndef DBMW_DAY7_HPP_
#define DBMW_DAY7_HPP_

#include "util.hpp"
#include <bits/stdc++.h>
#include <iostream>
#include <string>
#include <vector>

bool is_number(const std::string &s) {
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it))
        ++it;
    return !s.empty() && it == s.end();
}
typedef int32_t Offset;
vector<uint32_t> free_list;
struct Node {
    string parent = "";// 0 means no parent, so root node
    string fullDirName = "/";
    vector<string> fileNames;
    vector<int> fileSizes = {};
    vector<string> children = {};
    Node(string parent, string fullDirName)
        : parent(parent), fullDirName(fullDirName){};
};

struct day7 {
    int totalSize = 0;
    string cwd = "/";
    map<string, int> sizeOfDir;
    map<string, int> numVisits;
    std::vector<uint32_t> free_list;
    map<string, Node> tree;

    day7(const std::string &st = "input/input7") {
        tree.insert({"/", Node("", "/")});

        for (auto s: fiterator(st, '\n')) {
            auto first4 = s.substr(0, 4);
            if (first4 == "$ cd") {
                auto remainder = s.substr(5, s.length() - 5);
                if (remainder == "/") {
                    cwd = "/";
                } else if (remainder == "..") {
                    auto lenStr = cwd.length();
                    auto lastPos = lenStr - 1;
                    auto lastSlash = cwd.find_last_of("/", lastPos - 1);
                    if (lastSlash != 0) {
                        cwd = cwd.substr(0, lastSlash + 1);
                    }
                } else {
                    auto new_cwd = cwd + remainder + "/";
                    tree.insert({new_cwd, Node(cwd, new_cwd)});
                    tree.find(cwd)->second.children.push_back(new_cwd);
                    cwd = new_cwd;
                }
                numVisits[cwd] += 1;
            } else {
                auto x = s.find(" ");
                auto upToSpace = s.substr(0, x);
                auto fname = s.substr(x + 1, s.length());
                auto currentNode = tree.find(cwd)->second;
                if (is_number(upToSpace)) {
                    if (find(currentNode.fileNames.begin(), currentNode.fileNames.end(),
                             fname) == currentNode.fileNames.end()) {
                        tree.find(cwd)->second.fileSizes.push_back(stoi(upToSpace));
                        tree.find(cwd)->second.fileNames.push_back(fname);
                    }
                }
            }
        }
    }
    int recursiveDirectoryAdd(string rootNodeIdx) {
        auto node = tree.find(rootNodeIdx)->second;
        auto sum = 0;
        sum += accumulate(node.fileSizes.begin(), node.fileSizes.end(), 0);
        for (auto child: node.children) {
            auto totalDirSize = recursiveDirectoryAdd(child);
            sum += totalDirSize;
        }
        return sum;
    }
    void firstStar() {
        vector<int> countedDirSizes = {};
        for (auto pair: tree) {
            auto totalSize = recursiveDirectoryAdd(pair.first);
            countedDirSizes.push_back(totalSize);
        }
        int totalScore = 0;
        for (auto i: countedDirSizes) {
            if (i <= 100000)
                totalScore += i;
        }
        std::cout << "Seventh day of Christmas: " << totalScore << std::endl;
    }

    void secondStar() {
        int diskSize = 70000000;
        int needed = 30000000;
        auto currentlyUsed = recursiveDirectoryAdd("/");
        auto freeSpace = diskSize - currentlyUsed;
        auto neededToFree = needed - freeSpace;
        string smallest = "";
        int smallestSize = diskSize;// initalize to something v large
        for (auto pair: tree) {
            auto totalSize = recursiveDirectoryAdd(pair.first);
            if ((totalSize > neededToFree) & (totalSize < smallestSize)) {
                smallest = pair.first;
                smallestSize = totalSize;
            }
        }
        std::cout << "Seventh Day of Christmas: " << smallestSize << std::endl;
    }
};

#endif