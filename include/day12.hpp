#ifndef DBMW_DAY12_HPP_
#define DBMW_DAY12_HPP_

#include "util.hpp"
#include <algorithm>
#include <bits/stdc++.h>
#include <cctype>
#include <cmath>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/property_map/property_map.hpp>
#include <boost/tokenizer.hpp>
using namespace std;

typedef boost::property<boost::edge_weight_t, int> EdgeWeightProperty;
typedef boost::adjacency_list<boost::listS, boost::vecS, boost::directedS,
                              boost::no_property, EdgeWeightProperty>
    Graph;

typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;
struct day12 {
  Graph g;
  vector<vector<int>> heightMap;
  vector<int> startCoord;
  vector<int> endCoord;
  int xrange;
  int yrange;
  day12(const std::string &st = "input/input12") {
    int rowCount = 0;
    int colCount;
    int tmpHeight;
    for (auto s : fiterator(st, '\n')) {
      colCount = 0;
      if (s != "") {
        vector<int> tmp = {};
        for (auto i : s) {
          if (i == 'E') {
            endCoord = {rowCount, colCount};
            tmpHeight = 25;
          } else if (i == 'S') {
            startCoord = {rowCount, colCount};
            tmpHeight = 0;
          } else {
            tmpHeight = (int)i - 'a';
          }
          tmp.push_back(tmpHeight);
          colCount += 1;
        }
        heightMap.push_back(tmp);
      }
      rowCount += 1;
    }
    xrange = heightMap[0].size();
    yrange = heightMap.size();
    vector<vector<int>> checks = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};
    typedef pair<int, int> Edge;
    vector<Edge> edges;
    vector<int> verts = {};
    vector<int> tmpCoords = {0, 0};
    int fromVrtxIdx = 0;
    vector<int> weights;
    for (int i = 0; i < yrange; i++) {
      for (int j = 0; j < xrange; j++) {
        tmpCoords = {i, j};
        tmpHeight = heightMap[i][j];
        fromVrtxIdx = coord2idx(tmpCoords);
        verts.push_back(fromVrtxIdx);
        for (auto step : checks) {
          vector<int> newCoords = {tmpCoords[0] + step[0],
                                   tmpCoords[1] + step[1]};
          if (checkCoordValid(newCoords)) {
            auto newHeight = heightMap[newCoords[0]][newCoords[1]];
            if ((newHeight - tmpHeight) <= 1) {
              auto toVrtxIdx = coord2idx(newCoords);
              Edge edge = {fromVrtxIdx, toVrtxIdx};
              edges.push_back(edge);
              weights.push_back(1);
            }
          }
        }
      }
    }
    int n_vert = xrange * yrange;
    g = Graph(n_vert);
    for (int i = 0; i < ((int)(edges.size())); ++i)
      boost::add_edge(edges[i].first, edges[i].second, 1, g);
  }
  bool checkCoordValid(vector<int> coord) {
    bool check1 = (coord[0] >= 0) & (coord[0] < yrange);
    bool check2 = (coord[1] >= 0) & (coord[1] < xrange);
    return check1 & check2;
  }
  int coord2idx(vector<int> coord) {
    // xrange = 8, yrange = 5
    //  0,7 -> 7
    //  1,1 -> 9
    //  1,5 -> 13
    return coord[1] + coord[0] * xrange;
  }
  vector<int> idx2coord(int idx) {
    auto coord2 = idx % xrange;
    auto coord1 = (int)idx / xrange;
    vector<int> out = {coord1, coord2};
    return out;
  }

  void firstStar() {
    std::vector<int> d(num_vertices(g));
    auto startIdx = coord2idx(startCoord);
    auto endIdx = coord2idx(endCoord);
    Vertex l = startIdx;
    dijkstra_shortest_paths(g, l, boost::distance_map(&d[0]));
    std::cout << "Twelfth day of Christmas: " << d[endIdx] << std::endl;
  }

  void secondStar() {
    int bestScore = 100000;
    vector<int> d(num_vertices(g));
    auto endIdx = coord2idx(endCoord);
    for (int i = 0; i < yrange; i++) {
      for (int j = 0; j < xrange; j++) {
        if ((heightMap[i][j]) == 0) {
          int newStart = coord2idx({i, j});
          Vertex l = newStart;
          dijkstra_shortest_paths(g, l, boost::distance_map(&d[0]));
          int score = d[endIdx];
          if (bestScore > score) {
            bestScore = score;
          }
        }
      }
    }
    std::cout << "Twelfth day of Christmas: " << bestScore << std::endl;
  }
};

#endif