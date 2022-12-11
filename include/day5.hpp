#ifndef DBMW_DAY5_HPP_
#define DBMW_DAY5_HPP_

#include "util.hpp"
#include <bits/stdc++.h>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

void insertDeque(vector<deque<char>> &stacks, unsigned place, char letter) {
  stacks[place].push_front(letter);
}

struct day5 {
  vector<deque<char>> locations = {};
  vector<deque<char>> locations_new = {};
  vector<unsigned> fromVec;
  vector<unsigned> toVec;
  vector<unsigned> moveVec;

  tuple<char, unsigned> get_place(string s) {
    unsigned loc = s.find('[');
    char boxletter = s[loc + 1];
    unsigned pos = (loc) / 4;
    return {boxletter, pos};
  }
  day5(const std::string &st = "input/input5") {
    for (unsigned i = 0; i < 9; i++) {
      deque<char> new_deque = {};
      locations.push_back(new_deque);
    }
    for (auto s : fiterator(st, '\n')) {
      string substr = s;
      unsigned previouspos = 0;
      if (s[0] == '[') {
        while (1) {
          auto [letter, pos] = get_place(substr);
          previouspos += pos + 1;
          insertDeque(locations, previouspos - 1, letter);
          unsigned todrop = pos * 4 + 2;
          if (todrop + 2 < substr.length()) {
            substr = substr.substr(todrop + 2, substr.length());
          } else {
            break;
          }
        }
      } else if ((s != "") & (s[1] != '1')) {
        // move 1 from 3 to 9
        unsigned move_end = s.find(" ");
        unsigned from_start = s.find("f");
        auto move = stoi(s.substr(move_end + 1, from_start - 1 - move_end - 1));
        moveVec.push_back(move);
        auto from_end = from_start + 4;
        auto to_start = s.find("t");
        auto from = stoi(s.substr(from_end + 1, to_start - 2 - from_end));
        fromVec.push_back(from);
        toVec.push_back(
            stoi(s.substr(to_start + 2, s.length() - to_start - 2)));
      }
      locations_new = locations;
    }
  }

  void moveCrates(unsigned from, unsigned to, unsigned howmany) {
    for (unsigned i = 0; (i < howmany) & (locations[from].size() > 0); i++) {
      auto box = locations[from].back();
      locations[from].pop_back();
      locations[to].push_back(box);
    }
  }
  void firstStar() {

    for (unsigned i = 0; i < moveVec.size(); i++) {
      moveCrates(fromVec[i] - 1, toVec[i] - 1, moveVec[i]);
    }

    std::cout << "Fifth day of Christmas: ";
    for (unsigned i = 0; i < 9; i++) {
      cout << locations[i].back();
    }
    cout << endl;
  }

  void moveCrates9001(unsigned from, unsigned to, unsigned howmany) {
    deque<char> pickedUp = {};
    for (unsigned i = 0; (i < howmany) & (locations_new[from].size() > 0);
         i++) {
      auto box = locations_new[from].back();
      locations_new[from].pop_back();
      pickedUp.push_front(box);
    }
    for (auto box : pickedUp) {
      locations_new[to].push_back(box);
    }
  }
  void secondStar() {

    for (unsigned i = 0; i < moveVec.size(); i++) {
      moveCrates9001(fromVec[i] - 1, toVec[i] - 1, moveVec[i]);
    }

    std::cout << "Fifth Day of Christmas: ";
    for (unsigned i = 0; i < 9; i++) {
      cout << locations_new[i].back();
    }
    cout << endl;
  }
};

#endif