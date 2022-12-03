#ifndef DBMW_DAY2_HPP_
#define DBMW_DAY2_HPP_

#include"util.hpp"
#include<vector>
#include<string>
#include<iostream>
#include <bits/stdc++.h>


struct day2 {
    vector<char> A;
    vector<char> B;

    day2(const std::string &st = "input/input2") {
        for (auto s: fiterator(st, '\n')) {
            if (s!="") {
                A.push_back(s.front());
                B.push_back(s.back());
            }
            }
        }


    unsigned movescore(char x) {
        unsigned ret;
        if (x=='A' or x=='X') {
            ret = 1;
        }else if (x=='B' or x=='Y')
        {
            ret=2;
        } else ret= 3;
        return ret;
    }

    unsigned winscore(char player1, char player2){
        unsigned ret;
        std::map<char, char> p1top2;
        p1top2['A'] = 'X';
        p1top2['B'] = 'Y';
        p1top2['C'] = 'Z';
        if (p1top2[player1]==player2){
            ret=3;
        }
        else if (player1=='A') {
            if (player2 == 'Y') ret = 6; else ret = 0;
        }
        else if (player1=='B'){
            if (player2=='X') ret = 0; else ret = 6;
        }else {
            if (player2=='X') ret = 6; else ret = 0;
        }
        return ret;
    }

    char setShape(char player1, char player2hint){
        std::vector<char> moves = {'X', 'Y', 'Z'};
        std::map<char, unsigned> hinttoscore;
        hinttoscore['X'] = 0;
        hinttoscore['Y'] = 3;
        hinttoscore['Z'] = 6;
        auto desiredscore= hinttoscore[player2hint];
        char ret = 'l';
        for (auto move: moves){
            auto score = winscore(player1, move);
            if (score==desiredscore){
                ret = move;
                break;
            }
        }
        return ret;
    }

    std::tuple<unsigned, unsigned> myscore(char player1, char player2){
        auto player2win = winscore(player1, player2);
        auto player1score = 6-player2win + movescore(player1);
        auto player2score = player2win+ movescore(player2);
        return {player1score, player2score};
    }

        void firstStar() {
        unsigned totalScore = 0;
            for (unsigned e = 0; e < A.size(); e++) {
                auto [p1, p2] = myscore(A[e], B[e]);
                totalScore += p2;
            }
            std::cout << "Second day of Christmas: " << totalScore << std::endl;
        }


        void secondStar() {

            unsigned totalScore = 0;
            for (unsigned e = 0; e < A.size(); e++) {
                auto new_move = setShape(A[e], B[e]);
                auto [p1, p2] = myscore(A[e], new_move);
                totalScore += p2;
            }
            std::cout << "Second Day of Christmas: " << totalScore << std::endl;
        }
};


#endif