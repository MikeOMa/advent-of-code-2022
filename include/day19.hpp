#ifndef DBMW_DAY19_HPP_
#define DBMW_DAY19_HPP_

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

struct day19 {
    map<string, int> ore2idx = {{"clay", 1},
                                {"obsidian", 2},
                                {"ore", 0},
                                {"geode", 3}};
    vector<vector<vector<int>>> blueprints;
    vector<int> ingredients(string s) {
        auto d = tokenize(s);
        int l1 = 5;
        vector<int> out = {0, 0, 0, 0};
        for (int i = l1; i < (d.size()); i += 3) {
            out[ore2idx[d[i + 1]]] = stoi(d[i]);
        }
        return out;
    }
    int n;
    day19(const std::string &st = "input/input19") {
        for (auto s: fiterator(st, '\n')) {
            if (s != "") {
                auto data = tokenize(s, {':', '.'});
                vector<vector<int>> blueprint = {};
                for (int i = 1; i < 5; i++) {
                    blueprint.push_back(ingredients(data[i]));
                }
                blueprints.push_back(blueprint);
            }
        }
        n = blueprints.size();
    }
    vector<int> addVec(vector<int> x, vector<int> y) {
        vector<int> result;
        for (int i = 0; i < x.size(); i++) {
            result.push_back(x[i] + y[i]);
        }
        return result;
    }

    vector<int> subVec(vector<int> x, vector<int> y) {
        vector<int> result;
        for (int i = 0; i < x.size(); i++) {
            result.push_back(x[i] - y[i]);
        }
        return result;
    }
    void printVec(vector<int> x) {
        for (auto i: x) {
            cout << i << ", ";
        }
        cout << endl;
    }
    bool checkIfBuildAble(vector<int> &resources, vector<int> &req) {
        bool out = true;
        for (int i = 0; i < resources.size(); i++) {
            out = out & (resources[i] >= req[i]);
        }
        return out;
    }
    bool checkRobots(vector<int> &req, vector<int> &robots) {
        //check if the component will be buildable in future
        bool out = true;
        for (int i = 0; i < req.size(); i++) {
            if (req[i] > 0) {
                out = out & (robots[i] > 0);
            }
        }
        return out;
    }

    struct simulationData {
        vector<int> resources = {0, 0, 0, 0};
        vector<int> robots = {1, 0, 0, 0};
        int round = 0;
        int targetRobot = -1;
        int steps = 24;
        simulationData(){};
        simulationData(int robot) : targetRobot(robot){};
        vector<int> hash() {
            vector<int> x;
            for (int i = 0; i < 3; i++) {
                x.push_back(resources[i]);
            }
            for (int i = 0; i < 4; i++) {
                x.push_back(robots[i]);
            }
            x.push_back(round);
            return x;
        }
        int doNothingScore() {
            return robots[3] * (steps - round) + resources[3];
        }

        int highestPossible() {
            int doNothing = doNothingScore();
            int roundsLeft = (steps - round);
            int rem = roundsLeft * (roundsLeft - 1) / 2;
            return doNothing + rem;
        }
    };
    long long counter = 0;
    int highestGeodes = 0;

    int maxGeodes(simulationData &input) {
        /*
        int n_res = input.resources[3];
        int n_robots = input.robots[3];
        int movesLeft = (input.steps - input.round);
        auto endRes = n_res + movesLeft * n_robots + (movesLeft * (movesLeft - 1) / 2);
         */
        int endRes = input.highestPossible();
        bool out = endRes < highestGeodes;

        return out;
    }

    int simulationScore(simulationData &input, vector<vector<int>> &blueprint) {
        auto resourcesAtStart = input.resources;
        bool canBuild = checkIfBuildAble(resourcesAtStart, blueprint[input.targetRobot]);
        // If the robots can't ever build the target we can prune it.
        // If the maxGeodes this branch could get is lower than the highest achieved;
        bool prune = not checkRobots(blueprint[input.targetRobot], input.robots);
        if (canBuild) {
            prune = (prune) | (maxGeodes(input));
        }
        input.round += 1;
        input.resources = addVec(input.resources, input.robots);
        if ((input.round == input.steps) | prune) {
            int out = input.resources[3];
            if (out > highestGeodes) {
                highestGeodes = out;
            }
            return out;
        } else if (canBuild) {
            int bestScore = 0;
            input.robots[input.targetRobot] += 1;
            input.resources = subVec(input.resources, blueprint[input.targetRobot]);
            auto canBuildGeode = checkIfBuildAble(input.resources, blueprint[3]);
            for (int targ = 0; targ < 4; targ++) {
                // If we have enough robots to make a geode every turn.
                // Do not build another.
                bool startBranch = false;
                for (int other = 0; other < 4; other++) {
                    startBranch = (blueprint[other][targ] > input.robots[targ]);
                    if (startBranch) {
                        break;
                    }
                }
                if (canBuildGeode) {
                    //if we can build a geode only start target 3
                    startBranch = false;
                }
                if (startBranch | (targ == 3)) {
                    counter += 1;
                    auto copiedData = input;
                    copiedData.targetRobot = targ;
                    int score = simulationScore(copiedData, blueprint);
                    if (score > bestScore) {
                        bestScore = score;
                    }
                }
            }
            return bestScore;
        } else {
            int bestScore = simulationScore(input, blueprint);
            return bestScore;
        }
    }


    void firstStar() {
        unsigned totalScore = 0;
        for (int i = 0; i < n; i++) {
            highestGeodes = 0;
            auto blueprint = blueprints[i];
            int bestScore = 0;
            for (int rob = 0; rob < 4; rob++) {
                auto init = simulationData(rob);
                init.steps = 24;
                int x = simulationScore(init, blueprint);
                if (x > bestScore) {
                    bestScore = x;
                }
            }
            totalScore += (i + 1) * bestScore;
        }
        assert(totalScore == 1264);
        std::cout << "Nineteenth day of Christmas: " << totalScore << std::endl;
    }
    void secondStar() {
        unsigned totalScore = 1;
        counter = 0;
        highestGeodes = 0;
        for (int i = 0; i < 3; i++) {
            auto blueprint = blueprints[i];
            highestGeodes = 0;
            int bestScore = 0;
            for (int rob = 0; rob < 4; rob++) {
                auto init = simulationData(rob);
                init.steps = 32;
                int x = simulationScore(init, blueprint);
                if (x > bestScore) {
                    bestScore = x;
                }
            }
            totalScore = totalScore * bestScore;
        }
        assert(totalScore == 13475);
        std::cout << "Nineteenth day of Christmas: " << totalScore << std::endl;
    }
};

#endif