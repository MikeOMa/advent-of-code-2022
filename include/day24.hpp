#ifndef DBMW_DAY24_HPP_
#define DBMW_DAY24_HPP_

#include "util.hpp"
#include <algorithm>
#include <bits/stdc++.h>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/astar_search.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/property_map/property_map.hpp>
#include <boost/tokenizer.hpp>
#include <cctype>
#include <cmath>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

typedef boost::property<boost::edge_weight_t, int> EdgeWeightProperty;
typedef boost::adjacency_list<boost::listS, boost::vecS, boost::directedS,
                              boost::no_property, EdgeWeightProperty>
        Graph;

typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;
struct day24 {
    vector<int> mapSize;
    vector<vector<int>> initialMap;
    Graph g_fow;
    int p1Ans;
    Graph g_back;
    vector<int> startCoord;
    vector<int> endCoord;
    int nStates;
    int nTime;
    void printArr(vector<vector<int>> x) {
        for (int i = 0; i < x.size(); i++) {
            for (int j = 0; j < x[0].size(); j++) {
                cout << x[i][j];
            }
            cout << endl;
        }
    }

    struct blizzard {
        complex<int> loc;
        char dirChar;
        complex<int> dir;
        complex<int> north = {-1, 0};
        complex<int> south = {1, 0};
        complex<int> east = {0, 1};
        complex<int> west = {0, -1};
        vector<int> mapSize;
        blizzard(complex<int> x, char y, vector<int> mapSize) : loc(x), dirChar(y), mapSize(mapSize) {
            switch (dirChar) {
                case '^':
                    dir = north;
                    break;
                case '>':
                    dir = east;
                    break;
                case '<':
                    dir = west;
                    break;
                case 'v':
                    dir = south;
                    break;
            }
        }

        void forward() {
            complex<int> next = loc + dir;
            loc.real((next.real() + mapSize[0]) % mapSize[0]);
            loc.imag((next.imag() + mapSize[1]) % mapSize[1]);
        }
        vector<int> coord() {
            vector<int> x = {loc.real(), loc.imag()};
            return x;
        }
    };
    vector<blizzard> blizzards;
    day24(const std::string &st = "input/input24") {
        int rowCount = 0;
        int colCount;
        vector<complex<int>> blizzardStarts;
        vector<char> blizzardDirs;
        for (auto s: fiterator(st, '\n')) {
            colCount = 0;
            if ((s != "") & (s.substr(2, 3) != "###")) {
                vector<int> tmp = {};
                for (auto i: s) {
                    if (i != '#') {
                        tmp.push_back(0);
                        if (i != '.') {
                            blizzardDirs.push_back(i);
                            blizzardStarts.push_back({rowCount, colCount});
                        }
                        colCount += 1;
                    }
                }
                rowCount += 1;
                initialMap.push_back(tmp);
            }
        }
        mapSize = {(int) initialMap.size(), (int) initialMap[0].size()};
        for (int i = 0; i < blizzardDirs.size(); i++) {
            blizzards.push_back({blizzardStarts[i], blizzardDirs[i], mapSize});
        }
        typedef pair<int, int> Edge;
        vector<Edge> edgesForward;
        vector<Edge> edgesBack;
        vector<int> verts = {};
        vector<int> tmpCoords = {0, 0};
        int fromVrtxIdx = 0;
        vector<int> weights;
        vector<vector<int>> Map;
        vector<vector<int>> checks = {{0, 0}, {1, 0}, {-1, 0}, {0, 1}, {0, -1}};
        nTime = mapSize[0] * mapSize[1];// could change this to LCM maybe
        nStates = nTime + 2;
        for (int t_f = 0; t_f < nTime; t_f++) {
            int t = (t_f + 1) % nTime;
            Map = initialMap;
            for (int b = 0; b < blizzards.size(); b++) {
                blizzards[b].forward();
                auto tmpC = blizzards[b].coord();
                Map[tmpC[0]][tmpC[1]] += 1;
            }
            verts.push_back(t_f * nStates);
            verts.push_back(t * nStates + 1);
            for (int i = 0; i < mapSize[0]; i++) {
                for (int j = 0; j < mapSize[1]; j++) {
                    tmpCoords = {i, j};
                    fromVrtxIdx = coord2idx(tmpCoords) + (t_f) *nStates;
                    verts.push_back(fromVrtxIdx);
                    for (auto step: checks) {
                        vector<int> newCoords = {tmpCoords[0] + step[0],
                                                 tmpCoords[1] + step[1]};
                        if (checkCoordValid(newCoords)) {
                            auto isAcc = Map[newCoords[0]][newCoords[1]];
                            if ((isAcc) == 0) {
                                auto toVrtxIdx = coord2idx(newCoords) + t * nStates;
                                Edge edge = {fromVrtxIdx, toVrtxIdx};
                                edgesForward.push_back(edge);
                                edgesBack.push_back(edge);
                                weights.push_back(1);
                            }
                        }
                    }
                }
            }
            vector<int> lastCoord = {mapSize[0] - 1, mapSize[1] - 1};
            vector<int> firstCoord = {0, 0};
            int lastState = coord2idx(lastCoord) + t * nStates;
            int firstState = coord2idx(firstCoord) + t * nStates;
            Edge last = {lastState, 1};
            Edge first = {firstState, 0};

            Edge start = {t_f * nStates, firstState};
            Edge startBack = {t_f * nStates + 1, lastState};

            Edge edgeStay = {t_f * nStates, t * nStates};
            Edge edgeGo = {t_f * nStates + 1, t * nStates + 1};
            edgesForward.push_back(edgeStay);
            edgesForward.push_back(last);
            edgesForward.push_back(start);

            edgesBack.push_back(edgeGo);
            edgesBack.push_back(first);
            edgesBack.push_back(startBack);
        }
        int n_vert = mapSize[0] * mapSize[1] + 2;
        g_fow = Graph(n_vert);
        g_back = Graph(n_vert);
        for (int i = 0; i < ((int) (edgesForward.size())); ++i) {
            boost::add_edge(edgesForward[i].first, edgesForward[i].second, 1, g_fow);
            boost::add_edge(edgesBack[i].first, edgesBack[i].second, 1, g_back);
        }
    }

    bool checkCoordValid(vector<int> coord) {
        bool check1 = (coord[0] >= 0) & (coord[0] < mapSize[0]);
        bool check2 = (coord[1] >= 0) & (coord[1] < mapSize[1]);
        return check1 & check2;
    }

    int coord2idx(vector<int> coord) {
        return coord[1] + coord[0] * mapSize[1] + 2;
    }

    vector<int> idx2coord(int idx) {
        int cidx = idx % nStates - 2;

        auto coord2 = cidx % mapSize[1];
        auto coord1 = (int) cidx / mapSize[0];
        int t = (int) idx / nStates;
        vector<int> out = {coord1, coord2, t};
        return out;
    }
    int printPath(vector<Vertex> p, Vertex start, Vertex goal) {
        std::vector<boost::graph_traits<Graph>::vertex_descriptor> path;
        boost::graph_traits<Graph>::vertex_descriptor current = goal;

        while (current != start) {
            path.push_back(current);
            current = p[current];
        }
        path.push_back(start);
        vector<boost::graph_traits<Graph>::vertex_descriptor>::reverse_iterator rit;
        int c = 0;
        for (rit = path.rbegin(); rit != path.rend(); ++rit) {
            c += 1;
            auto coord = idx2coord(*rit);
            cout << coord[2] << "[" << coord[0] << ", " << coord[1] << "]. ";
        }
        std::cout << std::endl;
        return c;
    }
    template<class Graph, class CostType>
    class distance_heuristic : public boost::astar_heuristic<Graph, CostType> {
    public:
        distance_heuristic(vector<int> goal, vector<int> mapSize, int nStates) : m_goal(goal), mapSize(mapSize), nStates(nStates) {}

        vector<int> idx2coord(int idx) {
            int cidx = idx % nStates - 2;

            auto coord2 = cidx % mapSize[1];
            auto coord1 = (int) cidx / mapSize[0];
            vector<int> out = {coord1, coord2};
            return out;
        }
        CostType operator()(Vertex u) {
            vector<int> loc = idx2coord((int) u);
            CostType dx = loc[0] - m_goal[0];
            CostType dy = loc[1] - m_goal[1];
            auto out = (int) ::sqrt(dx * dx + dy * dy);
            return out;
        }

    private:
        vector<int> m_goal;
        vector<int> mapSize;
        int nStates;
        vector<int> locGoal;
    };
    // visitor that terminates when we find the goal
    struct found_goal {
    };// exception for termination
    template<class Vertex>
    class astar_goal_visitor : public boost::default_astar_visitor {
    public:
        astar_goal_visitor(Vertex goal) : m_goal(goal) {}
        template<class Graph>
        void examine_vertex(Vertex u, Graph &g) {
            if (u == m_goal)
                throw found_goal();
        }

    private:
        Vertex m_goal;
    };
    void firstStar() {
        std::vector<int> d(num_vertices(g_fow));
        std::vector<Vertex> p(num_vertices(g_fow));
        auto startIdx = 0;
        int endIdx = 1;
        Vertex l = startIdx;
        Vertex e = endIdx;
        vector<int> distances;


        auto goalLoc = mapSize;
        try {
            boost::astar_search(g_fow,
                                l,
                                distance_heuristic<Graph, int>(goalLoc, mapSize, nStates),
                                boost::predecessor_map(&p[0]).distance_map(&d[0]).visitor(astar_goal_visitor<Vertex>(e)));
        } catch (found_goal fg) {// found a path to the goal
        }
        p1Ans = d[e];
        std::cout << "TwentyFourth day of Christmas: " << p1Ans << std::endl;
    }

    void
    secondStar() {
        auto startIdx = 0;
        int endIdx = 1;
        Vertex l = startIdx;
        Vertex e = endIdx;
        std::vector<int> d(num_vertices(g_fow));
        std::vector<Vertex> p(num_vertices(g_fow));
        auto goalLoc = mapSize;
        vector<int> startLoc = {-1, -1};
        int stateAfterFirst = (p1Ans % nTime) * nStates + 1;
        try {
            boost::astar_search(g_back,
                                stateAfterFirst,
                                distance_heuristic<Graph, int>(startLoc, mapSize, nStates),
                                boost::predecessor_map(&p[0]).distance_map(&d[0]).visitor(astar_goal_visitor<Vertex>(l)));
        } catch (found_goal fg) {// found a path to the goal
        }
        int returnTrip = d[l];
        int stateAfterSecond = ((p1Ans + returnTrip) % nTime) * nStates;
        try {
            boost::astar_search(g_fow,
                                stateAfterSecond,
                                distance_heuristic<Graph, int>(goalLoc, mapSize, nStates),
                                boost::predecessor_map(&p[0]).distance_map(&d[0]).visitor(astar_goal_visitor<Vertex>(e)));
        } catch (found_goal fg) {// found a path to the goal
        }
        int out = p1Ans + returnTrip + d[e];
        std::cout << "TwentyFourth day of Christmas: " << out << std::endl;
    }
};

#endif
