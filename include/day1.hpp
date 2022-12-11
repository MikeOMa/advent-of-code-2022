#ifndef DBMW_DAY1_HPP_
#define DBMW_DAY1_HPP_

#include "util.hpp"
#include <bits/stdc++.h>
#include <iostream>
#include <string>
#include <vector>

struct day1 {
  vector<vector<unsigned>> vectorOfVectors;
  vector<unsigned> init_vector() {
    vector<unsigned> tmp_vec;
    return tmp_vec;
  }
  vector<unsigned> calScores;

  day1(const std::string &st = "input/input1") {
    auto tmp_vec = init_vector();
    for (auto s : fiterator(st, '\n')) {
      if (s == "") {
        auto new_vec = tmp_vec;
        vectorOfVectors.push_back(new_vec);
        tmp_vec.clear();
      } else {
        tmp_vec.push_back(std::stoi(s));
      }
    }
  }

  void firstStar() {
    unsigned maxValue = 0;

    for (unsigned e = 0; e < vectorOfVectors.size(); e++) {
      auto v = vectorOfVectors[e];
      auto result = std::reduce(v.begin(), v.end());
      calScores.push_back(result);
      if (result >= maxValue) {
        maxValue = result;
      }
    }
    std::cout << "First Day of christmas: " << maxValue << std::endl;
  }

  void secondStar() {

    vector<unsigned> V(calScores.size());
    std::iota(V.begin(), V.end(), 0); // Initializing
    sort(V.begin(), V.end(),
         [&](int i, int j) { return calScores[i] > calScores[j]; });
    auto answer = calScores[V[0]] + calScores[V[1]] + calScores[V[2]];
    std::cout << "First Day of Christmas: " << answer << std::endl;
  }
};

#endif