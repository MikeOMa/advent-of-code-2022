#ifndef DBMW_DAY16_HPP_
#define DBMW_DAY16_HPP_

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
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/johnson_all_pairs_shortest.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/property_map/property_map.hpp>
using namespace std;

typedef boost::property<boost::edge_weight_t, int> EdgeWeightProperty;
typedef boost::adjacency_list<boost::listS, boost::vecS, boost::directedS,
                              boost::no_property, EdgeWeightProperty>
        Graph;
typedef pair<int, int> Edge;
vector<Edge> edgesForward;
typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;

struct day16 {
    map<string, long> pipeflows;
    vector<long> pipeflows_vec;
    vector<string> valves;
    vector<vector<string>> leads;
    Graph g;
    vector<Edge> edges;
    map<string, long> valve2idx;
    vector<vector<int>> pairwiseDists;
    day16(const std::string &st = "input/input16") {
        for (auto s: fiterator(st, '\n')) {
            if (s != "") {
                auto t = tokenize(s, {';'});
                string valveflow = t[0];
                auto tunnels = t[1];
                auto split = tokenize(valveflow);
                auto valve = split[1];
                valves.push_back(valve);
                auto spliteq = tokenize(valveflow, {'='});
                int flow = stoi(spliteq[1]);
                pipeflows.insert({valve, flow});
                pipeflows_vec.push_back(flow);

                auto leads_s = tokenize(t[1], {' ', ','});
                vector<string> x = {};
                int n = leads_s.size();
                for (int i = 5; i < n; i++) {
                    if (leads_s[i] != "") {
                        x.push_back(leads_s[i]);
                    }
                }
                leads.push_back(x);
            }
        }
        for (int i = 0; i < valves.size(); i++) {
            valve2idx.insert(make_pair(valves[i], i));
        }

        for (int i = 0; i < valves.size(); i++) {
            for (auto j: leads[i]) {
                int toValve = valve2idx.at(j);
                Edge tmp = {i, toValve};
                edges.push_back(tmp);
            }
        }

        int n_vert = valves.size();
        g = Graph(n_vert);
        for (int i = 0; i < ((int) (edges.size())); ++i) {
            boost::add_edge(edges[i].first, edges[i].second, 1, g);
        }
        for (int i = 0; i < n_vert; i++) {
            vector<int> tmp;
            for (int j = 0; j < n_vert; j++) {
                tmp.push_back(0);
            }
            pairwiseDists.push_back(tmp);
        }
        boost::johnson_all_pairs_shortest_paths(g, pairwiseDists);
        for (int i = 0; i < n_vert; i++) {
            for (int j = 0; j < n_vert; j++) {
                assert(pairwiseDists[i][j] < n_vert);
            }
        }
    }
    struct simState {
        vector<int> pipes;
        vector<long> flows;
        vector<bool> isopen;
        vector<int> openvents;
        int time = 1;
        long score = 0;
        vector<int> position;
        vector<int> target = {-1};
        vector<int> timeToTarget = {-1};
        int n = 1;
        simState(vector<long> flows, int pos) : flows(flows) {
            position.push_back(pos);
            for (int i = 0; i < flows.size(); i++) {
                pipes.push_back(0);
                isopen.push_back(false);
            }
        }
        simState(vector<long> flows, int pos, int players) : flows(flows), position({pos}), n(players) {
            for (int i = 0; i < flows.size(); i++) {
                pipes.push_back(0);
                isopen.push_back(false);
            }
            target.push_back(-1);
            position.push_back(position[0]);
            timeToTarget.push_back(-1);
        }
        simState() : flows({}), position(0) {
        }
        void stepForward() {
            time += 1;
            for (auto i: pipes) {
                score += i;
            }
            for (int i = 0; i < 2; i++) {
                timeToTarget[i] += -1;
                if (timeToTarget[i] == 0) {
                    position[i] = target[i];
                    openvents.push_back(position[i]);
                    pipes[position[i]] = flows[position[i]];
                    target[i] = -1;
                }
            }
        }
        void setTarget(int nextloc, int time, int idx = 0) {
            if (nextloc != target[idx]) {
                target[idx] = nextloc;
                timeToTarget[idx] = time;
                isopen[nextloc] = true;
            }
        }
    };
    template<typename T>
    vector<size_t> sort_indexes(const vector<T> &v) {

        // initialize original index locations
        vector<size_t> idx(v.size());
        iota(idx.begin(), idx.end(), 0);

        // sort indexes based on comparing values in v
        // using std::stable_sort instead of std::sort
        // to avoid unnecessary index re-orderings
        // when v contains elements of equal values
        stable_sort(idx.begin(), idx.end(),
                    [&v](size_t i1, size_t i2) { return v[i1] < v[i2]; });

        return idx;
    }

    vector<pair<int, int>> getCandidates(simState &sim, int maxt, int idx = 0) {
        vector<pair<int, int>> out;
        auto Drow = pairwiseDists[sim.position[idx]];
        auto sortedEl = sort_indexes(Drow);
        int n_el = 0;
        for (auto j: sortedEl) {
            auto val = Drow[j];
            bool open = sim.isopen[j];
            int timeleft = maxt - sim.time;
            if ((not open) & (sim.flows[j] > 0) & (timeleft > val)) {
                out.push_back({val, j});
                n_el += 1;
            }
            if (n_el == 3) {
                break;
            }
        }
        return out;
    }
    long highestScore = 0;

    simState bestSim;
    long TreeSearch1(simState &sim, int maxt = 30) {
        if (sim.time == maxt) {
            if (sim.score > highestScore) {
                bestSim = sim;
            }
            return sim.score;
        }

        vector<bool> checkIdle;
        for (auto i: sim.timeToTarget) {
            bool v = (i == -1);
            checkIdle.push_back(v);
        }
        if (sim.timeToTarget.size() > 1) {
            long bestScore = 0;
            vector<pair<int, int>> options;
            if (checkIdle[0]) {
                options = getCandidates(sim, maxt);
            }
            if (options.size() == 0) {
                options = {make_pair(sim.timeToTarget[0], sim.target[0])};
            }
            for (auto i: options) {
                auto tmpSim = sim;
                tmpSim.setTarget(i.second, i.first, 0);
                vector<pair<int, int>> options2;
                if (checkIdle[1]) {
                    options2 = getCandidates(tmpSim, maxt, 1);
                }
                if (options2.size() == 0) {
                    options2 = {make_pair(sim.timeToTarget[1], sim.target[1])};
                }
                for (auto j: options2) {
                    auto tmpSim2 = tmpSim;
                    tmpSim2.setTarget(j.second, j.first, 1);
                    tmpSim2.stepForward();
                    long score = TreeSearch1(tmpSim2, maxt);
                    if (bestScore < score) {
                        bestScore = score;
                    }
                }
            }
            return bestScore;
        } else {
            if (checkIdle[0]) {
                auto options = getCandidates(sim, maxt);
                if (options.size() != 0) {
                    long bestScore = 0;
                    for (auto i: options) {
                        auto tmpSim = sim;
                        tmpSim.setTarget(i.second, i.first, 0);
                        tmpSim.stepForward();
                        long score = TreeSearch1(tmpSim, maxt);
                        if (bestScore < score) {
                            bestScore = score;
                        }
                    }
                    return bestScore;
                }
            }
        }
        sim.stepForward();
        long bestScore = TreeSearch1(sim, maxt);
        return bestScore;
    };
    void firstStar() {
        auto initialState = simState(pipeflows_vec, valve2idx.at("AA"));
        auto out = TreeSearch1(initialState);
        assert(out == 2265);
        std::cout << "Sixteenth day of Christmas: " << out << std::endl;
    }

    void secondStar() {
        auto initialState = simState(pipeflows_vec, valve2idx.at("AA"), 2);
        bestSim = simState();
        auto out = TreeSearch1(initialState, 26);
        //2778 too low;
        assert(out == 2811);
        std::cout << "Sixteenth Day of Christmas: " << out << std::endl;
    }
};

#endif