#ifndef DBMW_DAY9_HPP_
#define DBMW_DAY9_HPP_

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

int sign(int x) {
  if (x > 0)
    return 1;
  if (x < 0)
    return -1;
  return 0;
}

struct day9 {
  vector<int> axes;
  vector<int> howfar;

  day9(const std::string &st = "input/input9") {
    int axis = 0;
    int n = 0;
    for (auto s : fiterator(st, '\n')) {
      if (s != "") {
        auto split = s.find(" ");
        auto numStr = s.substr(split + 1, s.length() - split - 1);
        auto num = stoi(numStr);
        auto dir = s.substr(0, split);
        char l = dir[0];
        if (l == 'R') {
          axis = 0;
          n = num;
        } else if (l == 'L') {
          axis = 0;
          n = -num;
        } else if (l == 'U') {
          axis = 1;
          n = -num;
        } else { // D
          axis = 1;
          n = num;
        }
      }
      axes.push_back(axis);
      howfar.push_back(n);
    }
  }
  string locToString(vector<int> loc) {
    stringstream ss;
    ss << loc[0] << "_" << loc[1];
    string str = ss.str();
    return str;
  }

  void updateTailLoc(vector<int> &tailLoc, vector<int> headLoc) {
    auto axis1Error = headLoc[0] - tailLoc[0];
    auto axis2Error = headLoc[1] - tailLoc[1];
    if ((abs(axis1Error) <= 1) & (abs(axis2Error) <= 1)) {
      return;
    }
    // there is an error if we get this far.
    vector<int> moveDir = {};
    // say head  = [2,2] tail = [0,0] error = [2,2];
    // tail will move [+1, +1]
    moveDir.push_back(sign(axis1Error));
    moveDir.push_back(sign(axis2Error));
    tailLoc[0] += moveDir[0];
    tailLoc[1] += moveDir[1];
  }

  void firstStar() {
    vector<int> headLoc = {0, 0};
    vector<int> tailLoc = {0, 0};
    set<string> positions = {};
    for (unsigned i = 0; i < axes.size(); i++) {
      auto axis = axes[i];
      auto dist = howfar[i];
      auto dir = sign(dist);
      for (int j = 0; abs(j) < abs(dist); j += dir) {
        headLoc[axis] += dir;
        updateTailLoc(tailLoc, headLoc);
        positions.insert(locToString(tailLoc));
      }
    }
    std::cout << "Ninth day of Christmas: " << positions.size() << std::endl;
  }

  void secondStar() {
    vector<vector<int>> locs;
    for (unsigned i = 0; i < 10; i++) {
      vector<int> temp = {0, 0};
      locs.push_back(temp);
    }
    set<string> positions = {};
    for (unsigned i = 0; i < axes.size(); i++) {
      auto axis = axes[i];
      auto dist = howfar[i];
      auto dir = sign(dist);
      for (int j = 0; abs(j) < abs(dist); j += dir) {
        locs[0][axis] += dir;
        for (unsigned j = 1; j < 10; j++) {
          updateTailLoc(locs[j], locs[j - 1]);
        }
        positions.insert(locToString(locs[9]));
      }
    }
    std::cout << "Ninth Day of Christmas: " << positions.size() << std::endl;
  }
};

#endif