#ifndef DBMW_DAY18_HPP_
#define DBMW_DAY18_HPP_

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

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/astar_search.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/property_map/property_map.hpp>


using namespace std;

typedef boost::property<boost::edge_weight_t, int> EdgeWeightProperty;
typedef boost::adjacency_list<boost::listS, boost::vecS, boost::directedS,
                              boost::no_property, EdgeWeightProperty>
        Graph;
typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;


struct day18 {
    vector<vector<int>> pairwiseD;
    typedef vector<int> droplet;
    vector<droplet> lava;
    std::set<droplet> lavaSet;
    Graph g;
    map<droplet, int> stateMap;
    int n_vert;

    day18(const std::string &st = "input/input18") {
        for (auto s: fiterator(st, '\n')) {
            if (s != "") {
                droplet tmp;
                auto q = tokenize(s, {','});
                for (auto i: q) {
                    tmp.push_back(stoi(i));
                }
                lava.push_back(tmp);
                lavaSet.insert(tmp);
            }
        }

        vector<droplet> verts;
        int c = 0;
        for (int i = -1; i < 21; i++) {
            for (int j = -1; j < 21; j++) {
                for (int k = -1; k < 21; k++) {
                    droplet x = {i, j, k};
                    stateMap.insert({x, c});
                    verts.push_back(x);
                    c += 1;
                }
            }
        }
        vector<Edge> edges;
        for (auto vert: verts) {
            for (auto e: buildEdges(vert)) {
                edges.push_back(e);
            }
        }
        n_vert = (int) verts.size();
        g = Graph(n_vert);
        for (int i = 0; i < ((int) (edges.size())); ++i) {
            boost::add_edge(edges[i].first, edges[i].second, 1, g);
        }
        for (int i = 0; i < n_vert; i++) {
            vector<int> tmp;
            for (int j = 0; j < n_vert; j++) {
                tmp.push_back(2);
            }
            pairwiseD.push_back(tmp);
        }
        boost::johnson_all_pairs_shortest_paths(g, pairwiseD);
    }

    vector<int> pm = {-1, 1};
    int countFreeEdge(droplet x) {
        int sides = 0;
        for (int idx = 0; idx < 3; idx++) {
            for (auto i: pm) {
                droplet up = x;
                up[idx] += i;
                if (not lavaSet.contains(up)) {
                    sides += 1;
                }
            }
        }
        return sides;
    }


    int countFreeEdge2(droplet x) {
        int sides = 0;
        for (int idx = 0; idx < 3; idx++) {
            for (auto i: pm) {
                droplet up = x;
                up[idx] += i;
                if (not lavaSet.contains(up)) {
                    if (pairwiseD[0][stateMap.at(up)] < (n_vert)) {
                        sides += 1;
                    }
                }
            }
        }
        return sides;
    }
    vector<Edge> buildEdges(droplet x) {
        vector<Edge> out;
        bool flag = lavaSet.contains(x);
        if (not flag) {
            for (int idx = 0; idx < 3; idx++) {
                for (auto i: pm) {
                    droplet up = x;
                    up[idx] += i;
                    if (not lavaSet.contains(up)) {
                        if (stateMap.contains(up)) {
                            Edge t = {stateMap.at(x), stateMap.at(up)};

                            out.push_back(t);
                        }
                    }
                }
            }
        }
        return out;
    }
    void firstStar() {
        int totalScore = 0;
        for (auto i: lava) {
            totalScore += countFreeEdge(i);
        }
        //6069 too high;
        std::cout << "Eighteenth day of Christmas: " << totalScore << std::endl;
    }

    void secondStar() {
        int totalScore = 0;
        for (auto i: lava) {
            totalScore += countFreeEdge2(i);
        }

        std::cout << "Eighteenth Day of Christmas: " << totalScore << std::endl;
    }
};

#endif