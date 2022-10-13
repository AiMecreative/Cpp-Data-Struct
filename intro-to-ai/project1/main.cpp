#include <ctime>
#include <iostream>
#include <vector>
#include <functional>
#include "Problem.h"
#include "Search.h"
#include "Action.h"

typedef std::vector<Action> (*searchFunc)(Problem &problem);

void printStats(std::vector<std::pair<int, float>> &statistics) {
    std::cout << "length, seconds" << '\n';
    for (auto &pair: statistics) {
        std::cout << pair.first << ", " << pair.second << '\n';
    }
}

void doExperiment(int iteration, int problemSize, int shuffleSteps, searchFunc searchFunc) {
    std::vector<std::pair<int, float>> statistics;

    for (int i = 0; i < iteration; i++) {
        Problem problem(problemSize, shuffleSteps);
        problem.printState(problem.initState);

        clock_t t = clock();
        std::vector<Action> solution = searchFunc(problem);
        t = clock() - t;

        std::pair<int, float> stats(solution.size(), ((float) t) / CLOCKS_PER_SEC);
        statistics.push_back(stats);

        printf("Searching tooks %f seconds.\n", stats.second);
        problem.printSolution(solution);

        problem.updateInitState();
    }

    printStats(statistics);
}

void testSearchFunction(searchFunc searchFunc) {
    int iteration = 50;
    int size = 3;
    int shuffleSteps = 30;

    for (int i = 1; i <= iteration; i++) {
        Problem problem(size, shuffleSteps);
        std::vector<Action> solution = searchFunc(problem);
        if (solution == std::vector<Action>({Action::CUTOFF})) {
            std::cout << "Iteration " << i << ": CUTOFF\n";
        } else {
            std::cout << "Iteration " << i << ": ";
            std::cout << problem.isSolution(solution, problem.initState) << std::endl;
        }
        problem.updateInitState();
    }
}


int main() {
//    std::srand((unsigned int) 100); // Generate a random seed
    std::srand((unsigned int) std::time(nullptr)); // Generate a random seed

//    doExperiment(50, 3, 10, (searchFunc) search::bfs);
//    doExperiment(50, 3, 10, (searchFunc) search::dlsWrapper);
    doExperiment(50, 3, 10, (searchFunc) search::aStarMisplace);
//    doExperiment(50, 3, 10, (searchFunc) search::aStarManhattan);


    // 以下函数用于测试搜索函数结果的正确性
//     testSearchFunction((searchFunc)search::bfs);
//     testSearchFunction((searchFunc)search::dlsWrapper);
    testSearchFunction((searchFunc) search::aStarMisplace);
//     testSearchFunction((searchFunc)search::aStarManhattan);
    return 0;
}