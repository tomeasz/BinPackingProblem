//
// Created by tomasz on 21.04.2022.
//

#include "algorithms.h"

#include <algorithm>
#include <random>

std::vector<int> hillClimbing(std::vector<int> startSolution, int binSize, int quantity, int iterations) {

    int score = howManyBin(startSolution, binSize, quantity);
    int bestScore = score;
    std::vector<int> bestSolution = startSolution;

    srand((unsigned) time(NULL));

    for (int i = 0; i < iterations; ++i) {
        int x = rand() % (std::end(startSolution) - std::begin(startSolution));
        for (int j = 0; j < startSolution.size(); ++j) {
            std::swap(startSolution[x], startSolution[j]);
            score = howManyBin(startSolution, binSize, quantity);
            if (score < bestScore) {
                bestScore = score;
                bestSolution = startSolution;
            } else{
                std::swap(startSolution[x], startSolution[j]);
            }
        }
        startSolution = bestSolution;
        //std::cout << i << " " << score << " " << bestScore << std::endl;
    }
    for (auto i : bestSolution) {
        std::cout << i << ", ";
    }
    std::cout << std::endl << std::endl;
    return bestSolution;
}

std::vector<int> hillClimbingRandom(std::vector<int> startSolution, int binSize, int quantity, int iterations) {

    int score = howManyBin(startSolution, binSize, quantity);
    int bestScore = score;
    std::vector<int> bestSolution = startSolution;
    srand((unsigned) time(NULL));

    for (int i = 0; i < iterations; ++i) {
        int x = rand() % (std::end(startSolution) - std::begin(startSolution));
        std::swap(startSolution[x], startSolution[i]);
        score = howManyBin(startSolution, binSize, quantity);
        if (score < bestScore) {
            bestScore = score;
            bestSolution = startSolution;
        } else{
            std::swap(startSolution[x], startSolution[i]);
        }
        //std::cout << i << " " << score << " " << bestScore << std::endl;
    }
    for (auto i : bestSolution) {
        std::cout << i << ", ";
    }
    std::cout << std::endl << std::endl;
    return bestSolution;
}
