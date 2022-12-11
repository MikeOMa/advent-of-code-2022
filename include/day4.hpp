#ifndef DBMW_DAY4_HPP_
#define DBMW_DAY4_HPP_

#include "util.hpp"
#include <bits/stdc++.h>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

vector<unsigned> vectorsOverlap(vector<unsigned> v1, vector<unsigned> v2) {
  std::vector<unsigned> intersect = {};
  std::set_intersection(v1.begin(), v1.end(), v2.begin(), v2.end(),
                        std::inserter(intersect, intersect.begin()));
  return intersect;
};

struct day4 {
  vector<string> A;
  vector<string> B;
  vector<std::vector<unsigned>> firstElf;
  vector<std::vector<unsigned>> secondElf;
  unsigned N;
  vector<unsigned> expandString(string mindashmax) {
    vector<unsigned> out = {};

    unsigned pos = mindashmax.find('-');
    string l = mindashmax.substr(0, pos);
    unsigned p1 = stoi(l);
    string l2 = mindashmax.substr(pos + 1, mindashmax.size());
    unsigned p2 = stoi(l2);

    for (unsigned i = p1; i <= p2; i++) {
      out.push_back(i);
    }
    return out;
  }

  day4(const std::string &st = "input/input4") {
    for (auto s : fiterator(st, '\n')) {
      if (s != "") {
        unsigned pos = s.find(',');
        auto p1 = s.substr(0, pos);
        auto p2 = s.substr(pos + 1, s.length());
        A.push_back(p1);
        B.push_back(p2);
        firstElf.push_back(expandString(p1));
        secondElf.push_back(expandString(p2));
      }
      N = A.size();
    }
  }

  void firstStar() {

    unsigned totalScore = 0;
    for (unsigned i = 0; i < N; i++) {
      auto overlap = vectorsOverlap(firstElf[i], secondElf[i]);
      if ((overlap.size() == firstElf[i].size()) ||
          (overlap.size() == secondElf[i].size())) {
        totalScore += 1;
      }
    }
    std::cout << "Fourth day of Christmas: " << totalScore << std::endl;
  }

  void secondStar() {
    unsigned totalScore = 0;
    for (unsigned i = 0; i < N; i++) {
      auto overlap = vectorsOverlap(firstElf[i], secondElf[i]);
      if (overlap.size() > 0)
        totalScore += 1;
    }

    std::cout << "Fourth Day of Christmas: " << totalScore << std::endl;
  }
};

#endif