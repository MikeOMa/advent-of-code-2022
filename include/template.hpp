#ifndef DBMW_DAY12_HPP_
#define DBMW_DAY12_HPP_

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

struct day3 {
  vector<std::set<char>> A;
  vector<std::set<char>> firstRucksack;
  vector<std::set<char>> secondRucksack;

  day3(const std::string &st = "input/input3") {
    for (auto s : fiterator(st, '\n')) {
      if (s != "") {
      }
    }
  }

  void firstStar() {
    unsigned totalScore = 0;
    std::cout << "Third day of Christmas: " << totalScore << std::endl;
  }

  void secondStar() {
    unsigned totalScore = 0;
    std::cout << "Third Day of Christmas: " << totalScore << std::endl;
  }
};

#endif