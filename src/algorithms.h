//
// Created by tomasz on 21.04.2022.
//

#include <algorithm>
#include <ctime>
#include <iostream>
#include <random>
#include <vector>

void printSolution(std::vector<int> bestSolution);
std::vector<int> dataGenerator(int minRange, int maxRange, int quantity);
int howManyBin(std::vector<int> startSolution, int binSize, int quantity);
std::vector<int> hillClimbingRandom(std::vector<int> startSolution, int binSize, int quantity, int iterations, int printMode, int endMode);
std::vector<int> hillClimbing(std::vector<int> startSolution, int binSize, int quantity, int iterations, int printMode, int endMode);
std::vector<int> tabuSearch(std::vector<int> startSolution, int binSize, int quantity, int tabuSize, int iterations, int printMode, int endMode);
std::vector<int> simulatedAnnealing(std::vector<int> startSolution, int binSize, int quantity, int iterations, bool uniformRealDistributionIsSet, int printMode, int endMode);
std::vector<int> geneticAlgorithm(std::vector<int> data, int binSize, int quantity, int iterations, int populationSize, bool pointCrossover, bool mutationMethod, int printMode, int endMode);
