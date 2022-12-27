#ifndef DBMW_DAY15_HPP_
#define DBMW_DAY15_HPP_

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
struct day15 {
    typedef pair<long, long> coord;
    vector<vector<int>> beaconsMap;
    vector<coord> sensors;
    vector<coord> beacons;
    set<coord> beaconsset;
    coord xrange = {0, 0};
    coord yrange = {0, 0};
    vector<long> distances;
    int extractx(string k) {
        auto tmp = tokenize(k, {'=', ',', ':'});
        return stoi(tmp[1]);
    }
    day15(const std::string &st = "input/input15") {
        for (auto s: fiterator(st, '\n')) {
            if (s != "") {
                auto data = tokenize(s, {' '});
                auto sensor_1_raw = data[2];
                auto sensor_2_raw = data[3];

                auto beacon_1_raw = data[8];
                auto beacon_2_raw = data[9];
                coord sensorCoord = {extractx(sensor_2_raw), extractx(sensor_1_raw)};
                coord beaconCoord = {extractx(beacon_2_raw), extractx(beacon_1_raw)};
                beacons.push_back(beaconCoord);
                beaconsset.insert(beaconCoord);
                sensors.push_back(sensorCoord);
            }
        }
        for (int i = 0; i < beacons.size(); i++) {
            distances.push_back(dist(beacons[i], sensors[i]));
            long d = distances[i];

            int xmin = sensors[i].first - d;
            int xmax = sensors[i].first + d;
            int ymin = sensors[i].second - d;
            int ymax = sensors[i].second + d;
            /*
            long xmin = min(sensors[i].first, beacons[i].first);
            long ymin = min(sensors[i].second, beacons[i].second);

            long xmax = max(sensors[i].first, beacons[i].first);
            long ymax = max(sensors[i].second, beacons[i].second);
             */
            if (xrange.first > xmin) {
                xrange.first = xmin;
            }
            if (xrange.second < xmax) {
                xrange.second = xmax;
            }
            if (yrange.second < ymax) {
                yrange.second = ymax;
            }
            if (yrange.first > ymin) {
                yrange.first = ymin;
            }
        }
    };
    long dist(coord x, coord y) {
        long out = abs(x.first - y.first) + abs(x.second - y.second);
        return out;
    }

    bool checkCovered(coord x) {
        bool out = false;
        for (int i = 0; i < sensors.size(); i++) {
            long d = dist(sensors[i], x);
            if (d <= distances[i]) {
                out = true;
                break;
            }
        }
        return out;
    }
    void firstStar() {
        long totalScore = 0;
        long row = 2000000;
        //long row=10;
        for (long i = yrange.first; i <= yrange.second; i++) {
            coord check = {row, i};
            if (not beaconsset.contains(check)) {
                if (checkCovered(check)) {
                    totalScore += 1;
                }
            }
        }
        //5116327 too high
        std::cout << "Fifteenth day of Christmas: " << totalScore << std::endl;
    }
    vector<coord> candidate(coord sensor, long dist) {
        vector<coord> out;
        vector<int> ne = {-1, 0, 1, 1};
        vector<int> se = {-1, 0, 1, -1};
        vector<int> sw = {1, 0, -1, -1};
        vector<int> nw = {1, 0, -1, 1};
        vector<vector<int>> dirs = {nw, se, sw, ne};
        for (long i = 0; i < (dist + 1); i++) {
            for (auto dir: dirs) {
                long x = sensor.first + (dist + 1) * (dir[0]) + i * dir[2];
                long y = sensor.second + (dist + 1) * (dir[1]) + i * dir[3];
                out.push_back({x, y});
            }
        }
        return out;
    }
    void secondStar() {
        coord minmax = {0, 4000000};
        //coord minmax = {0, 20};
        coord final = {0, 0};
        bool flag = false;
        for (int i = 0; i < sensors.size(); i++) {
            for (auto check: candidate(sensors[i], distances[i])) {
                bool inbox = (check.first < minmax.second) & (check.first >= minmax.first) & (check.second < minmax.second) & (check.second >= minmax.first);
                if (inbox) {
                    if (not checkCovered(check)) {
                        final = check;
                        flag = true;
                        break;
                    }
                }
            }
            if (flag) {
                break;
            }
        }
        long totalScore = final.second * 4000000 + final.first;

        std::cout << "Fifteenth Day of Christmas: " << totalScore << std::endl;
    }
};

#endif
