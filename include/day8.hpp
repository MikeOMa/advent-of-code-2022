#ifndef DBMW_DAY8_HPP_
#define DBMW_DAY8_HPP_

#include "util.hpp"
#include <bits/stdc++.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

struct day8 {
  vector<vector<unsigned>> treeHeights;
  unsigned nrow;
  unsigned ncol;
  day8(const std::string &st = "input/input8") {
    for (auto s : fiterator(st, '\n')) {
      vector<unsigned> newVec = {};
      for (auto i : s) {
        newVec.push_back(i - '0');
      }
      treeHeights.push_back(newVec);
    }
    nrow = treeHeights.size();
    ncol = treeHeights[0].size();
  }
  bool checkAxisTwo(unsigned start, unsigned row, unsigned inc = 1) {
    // default to being seen unless other evidence is found;
    bool out = true;
    auto height = treeHeights[row][start];
    if (start % (ncol - 1) == 0) {
      return true;
    }
    for (unsigned i = start + inc; (i >= 0) & (i < ncol); i += inc) {
      auto newHeight = treeHeights[row][i];
      if (height <= newHeight) {
        out = false;
        break;
      }
    }
    return out;
  }

  bool checkAxisOne(unsigned start, unsigned col, int inc = 1) {
    // default to being seen unless other evidence is found;
    bool out = true;
    auto height = treeHeights[start][col];
    if (start % (nrow - 1) == 0) {
      return true;
    }
    for (unsigned i = start + inc; (i >= 0) & (i < nrow); i += inc) {
      auto newHeight = treeHeights[i][col];
      if (height <= newHeight) {
        out = false;
        break;
      }
    }
    return out;
  }

  bool checkUpDown(unsigned row, unsigned col) {
    auto out = checkAxisTwo(col, row, 1) | checkAxisTwo(col, row, -1);
    return out;
  }

  bool checkLeftRight(unsigned row, unsigned col) {
    auto out = checkAxisOne(row, col, 1) | checkAxisOne(row, col, -1);
    return out;
  }
  bool checkVisible(unsigned row, unsigned col) {

    auto updown = checkUpDown(row, col);
    auto leftright = checkLeftRight(row, col);
    return updown | leftright;
  }

  void firstStar() {
    unsigned totalScore = 0;
    for (unsigned i = 0; i < nrow; i++) {
      for (unsigned j = 0; j < ncol; j++) {
        if (checkVisible(i, j)) {
          totalScore += 1;
        }
      }
    }
    std::cout << "Eighth day of Christmas: " << totalScore << std::endl;
  }

  unsigned countAxisOne(unsigned start, unsigned col, int inc = 1) {
    // default to being seen unless other evidence is found;
    unsigned out = 0;
    //  int currentHighest = -1;
    auto height = (int)treeHeights[start][col];
    for (unsigned i = start + inc; (i >= 0) & (i < nrow); i += inc) {
      int newHeight = (int)treeHeights[i][col];
      //  if (newHeight>=currentHighest){
      //      currentHighest=newHeight;
      out += 1;
      if (newHeight >= height)
        break;
      //   }
    }
    return out;
  }

  unsigned countAxisTwo(unsigned start, unsigned row, int inc = 1) {
    // default to being seen unless other evidence is found;
    unsigned out = 0;
    // int currentHighest = -1;
    auto height = (int)treeHeights[row][start];
    for (int i = (int)start + inc; (i >= 0) & (i < (int)ncol); i += inc) {
      int newHeight = (int)treeHeights[row][i];
      // if (newHeight>=currentHighest){
      //    currentHighest=newHeight;
      out += 1;
      if (newHeight >= height)
        break;
      //}
    }
    return out;
  }

  unsigned scoreLocation(unsigned row, unsigned col) {
    auto up = countAxisOne(row, col, -1);
    auto down = countAxisOne(row, col, 1);
    auto left = countAxisTwo(col, row, -1);
    auto right = countAxisTwo(col, row, 1);
    auto out = left * right * down * up;
    return out;
  }

  void secondStar() {
    unsigned bestScore = 0;
    vector<unsigned> bestLoc = {0, 0};
    for (unsigned i = 0; i < nrow; i++) {
      for (unsigned j = 0; j < ncol; j++) {
        auto score = scoreLocation(i, j);
        if (score > bestScore) {
          bestScore = score;
          bestLoc = {i, j};
        }
      }
    }
    std::cout << "Eighth Day of Christmas: " << bestScore << std::endl;
  }
};

#endif