//
// Created by tomasz on 16.06.2022.
//

#include "algorithms.h"

std::vector<std::vector<int>> createPopulation(int populationSize, int dataSize) {
    std::vector<int> chromosome;
    std::vector<std::vector<int>> population;
    for (int i = 0; i < dataSize; ++i) chromosome.push_back(i);
    for (int i = 0; i < populationSize; ++i) {
        population.push_back(chromosome);
        std::shuffle(std::begin(population[i]), std::end(population[i]), std::mt19937(std::random_device()()));
    }
    return population;
}

std::vector<int> getPopulationFitness(std::vector<int> data, std::vector<std::vector<int>> population, int binSize) {
    std::vector<int> populationFitness;
    for (int i = 0; i < population.size(); ++i) {
        std::vector<int> tmp;
        for (int j = 0; j < data.size(); ++j) tmp.push_back(data[population[i][j]]);
        populationFitness.push_back(howManyBin(tmp, binSize, data.size()));
        tmp.clear();
    }
    return populationFitness;
}

std::vector<std::vector<int>>
burtalRoulette(std::vector<std::vector<int>> population, std::vector<int> populationFitness, int bestScore) {
    long sumOfPopulationFitness = 0;
    int populationSize = population.size();
    std::default_random_engine generator;

    for (int i = 0; i < populationSize; ++i) {
        populationFitness[i] = populationFitness[i] - bestScore;
        sumOfPopulationFitness += populationFitness[i];
    }

    for (int j = 0; j < populationSize / 2; j++) {
        int offset = 0;
        std::uniform_int_distribution<int> dist(0, sumOfPopulationFitness);
        if (sumOfPopulationFitness != 0) offset = dist(generator);
        int z = 0;
        while (offset > 0) {
            if (offset < populationFitness[z]) break;
            else {
                offset = offset - populationFitness[z];
                z++;
            }
        }
        sumOfPopulationFitness -= populationFitness[z];
        populationFitness.erase(populationFitness.begin() + z);
        population.erase(population.begin() + z);

    }
    return population;
}

std::vector<std::vector<int>>
roulette(std::vector<std::vector<int>> population, std::vector<int> populationFitness, int worstScore) {
    long sumOfPopulationFitness = 0;
    int populationSize = population.size();
    std::default_random_engine generator;
    std::vector<std::vector<int>> parent;

    for (int i = 0; i < populationSize; ++i) {
        populationFitness[i] = worstScore - populationFitness[i];
        sumOfPopulationFitness += populationFitness[i];
    }

    for (int j = 0; j < populationSize; j++) {
        int offset = 0;
        std::uniform_int_distribution<int> dist(0, sumOfPopulationFitness - 1);
        if (sumOfPopulationFitness != 0) offset = dist(generator);
        //searching randomly selected parent
        int z = 0;
        while (offset > 0) {
            if (offset < populationFitness[z]) break;
            else {
                offset = offset - populationFitness[z];
                z++;
            }
        }
        parent.push_back(population[z]);
    }
    return parent;
}

std::vector<std::vector<int>> onePointCrossover(std::vector<std::vector<int>> parents) {
    std::vector<std::vector<int>> offspring;
    int i = 0;
    std::default_random_engine generator;

    while (i < parents.size() - 1) {
        std::vector<int> firstChildren;
        std::vector<int> secondChildren;

        //Select a random crossover point
        std::uniform_int_distribution<int> dist(0, parents[0].size());
        int crossOverPoint = dist(generator);

        //Swap values among parents
        for (int j = 0; j < crossOverPoint; j++) {
            firstChildren.push_back(parents[i][j]);
            secondChildren.push_back(parents[i + 1][j]);
        }
        for (int j = 0; j < parents[0].size(); ++j) {
            auto it = std::find(firstChildren.begin(), firstChildren.end(), parents[i + 1][j]);
            if (it == firstChildren.end()) firstChildren.push_back(parents[i + 1][j]);
            it = std::find(secondChildren.begin(), secondChildren.end(), parents[i][j]);
            if (it == secondChildren.end()) secondChildren.push_back(parents[i][j]);
        }
        offspring.push_back(firstChildren);
        offspring.push_back(secondChildren);
        i += 2;
    }
    return offspring;
}

std::vector<std::vector<int>> twoPointCrossover(std::vector<std::vector<int>> parents) {
    std::vector<std::vector<int>> offspring;
    int i = 0;
    std::default_random_engine generator;

    while (i < parents.size() - 1) {
        std::vector<int> firstChildren;
        std::vector<int> secondChildren;
        std::vector<int> firstLoad;
        std::vector<int> secondLoad;

        //Select a random crossover points
        std::uniform_int_distribution<int> dist(0, parents[0].size());
        int firstCrossOverPoint = dist(generator);
        int secondCrossOverPoint = dist(generator);
        if (firstCrossOverPoint > secondCrossOverPoint) std::swap(firstCrossOverPoint, secondCrossOverPoint);

        //make swap load
        for (int j = firstCrossOverPoint; j < secondCrossOverPoint; j++) {
            firstLoad.push_back(parents[i + 1][j]);
            secondLoad.push_back(parents[i][j]);
        }
        //append first piece of chromosom
        for (int j = 0; j < firstCrossOverPoint; j++) {
            auto it = std::find(firstLoad.begin(), firstLoad.end(), parents[i][j]);
            if (it == firstLoad.end()) firstChildren.push_back(parents[i][j]);
            it = std::find(secondLoad.begin(), secondLoad.end(), parents[i + 1][j]);
            if (it == secondLoad.end()) secondChildren.push_back(parents[i + 1][j]);
        }
        //append swap loads
        firstChildren.insert(firstChildren.end(), firstLoad.begin(), firstLoad.end());
        secondChildren.insert(secondChildren.end(), secondLoad.begin(), secondLoad.end());
        //append second piece of chromosom
        for (int j = secondCrossOverPoint; j < parents[0].size(); j++) {
            auto it = std::find(firstLoad.begin(), firstLoad.end(), parents[i][j]);
            if (it == firstLoad.end()) firstChildren.push_back(parents[i][j]);
            it = std::find(secondLoad.begin(), secondLoad.end(), parents[i + 1][j]);
            if (it == secondLoad.end()) secondChildren.push_back(parents[i + 1][j]);
        }
        //repair lose data
        for (int j = 0; j < parents[0].size(); j++) {
            auto it = std::find(firstChildren.begin(), firstChildren.end(), parents[i][j]);
            if (it == firstChildren.end()) firstChildren.push_back(parents[i][j]);
            it = std::find(secondChildren.begin(), secondChildren.end(), parents[i + 1][j]);
            if (it == secondChildren.end()) secondChildren.push_back(parents[i + 1][j]);
        }
        //push back children
        offspring.push_back(firstChildren);
        offspring.push_back(secondChildren);
        i += 2;
    }
    return offspring;
}

std::vector<std::vector<int>> swapMutation(std::vector<std::vector<int>> population) {
    std::default_random_engine generator;
    for (int i = 0; i < population.size() / 10; ++i) {
        std::uniform_int_distribution<int> distx(0, population.size());
        std::uniform_int_distribution<int> disty(0, population[0].size() - 1);
        auto x = distx(generator);
        auto y = disty(generator);
        auto tmp = population[x][y];
        population[x][y] = population[x][y + 1];
        population[x][y + 1] = tmp;
    }
    return population;
}

std::vector<std::vector<int>> shuffleMutation(std::vector<std::vector<int>> population) {
    std::default_random_engine generator;
    for (int i = 0; i < population.size() / 10; ++i) {
        std::uniform_int_distribution<int> distx(0, population.size());
        std::uniform_int_distribution<int> disty(0, population[0].size());
        auto x = distx(generator);
        auto y = disty(generator);
        auto z = disty(generator);
        if (y > z) std::swap(y, z);
        std::shuffle(&population[x][y], &population[x][z], std::mt19937(std::random_device()()));
    }
    return population;
}

std::vector<int>
geneticAlgorithm(std::vector<int> data, int binSize, int quantity, int iterations, int populationSize,
                 bool pointCrossover, bool mutationMethod, int printMode, int endMode) {
    bool continueCondition = true;

    //create first population
    std::vector<std::vector<int>> population = createPopulation(populationSize, quantity);;
    std::vector<int> populationFitness = getPopulationFitness(data, population, binSize);
    int bestScore = populationFitness[0];
    int worstScore = bestScore;
    std::vector<int> bestSolution = population[0];
    std::vector<int> bestSolutionData;
    int i = 0;

    for (int j = 0; j < populationFitness.size(); ++j) {
        if (populationFitness[j] < bestScore) {
            bestScore = populationFitness[j];
            bestSolution = population[j];
        }
        if (populationFitness[j] > worstScore) worstScore = populationFitness[j];
    }
    clock_t start = clock();
    clock_t end = clock();

    while (continueCondition) {
        //selection parents
        std::vector<std::vector<int>> parents = roulette(population, populationFitness, worstScore);

        //crossing
        if (pointCrossover) population = onePointCrossover(parents);
        else population = twoPointCrossover(parents);

        //mutating a generation
        if (mutationMethod) swapMutation(population);
        else population = shuffleMutation(population);
        populationFitness = getPopulationFitness(data, population, binSize);

        for (int j = 0; j < populationFitness.size(); j++) {
            if (bestScore > populationFitness[j]) {
                bestScore = populationFitness[j];
                bestSolution = population[j];
            }
        }
        if (printMode == 1) std::cout << i << " " << "score" << " " << bestScore << std::endl;

        //ending do-while
        if (endMode == 0) ++i;
        else if (endMode == 1) i = end - start;
        if (endMode == 0 && i > iterations) continueCondition = false;
        else if (endMode == 1 && i > iterations) continueCondition = false;
        end = clock();

    }
    for (auto i: bestSolution) bestSolutionData.push_back(data[i]);

    return bestSolutionData;
}
