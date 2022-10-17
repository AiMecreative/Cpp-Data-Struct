# 搜索问题

## 简介

基于8-puzzle问题，目标是基于广度优先搜索（breadth first search）、深度有限搜索（depth limit search）、启发式搜索（A*）搜索该问题的解。该部分内容对应《Artificial Intelligence: A Modern Approach 3rd》中的第三章内容：Solving Problems by Searching。

##### 8-puzzle

该问题为一个九宫格，只有0可以和其上、下、左、右的数码交换位置。

该问题的目标状态为

```shell
0 1 2
3 4 5
6 7 8
```

随机的初始状态可能为

```shell
1 2 0
3 4 5
6 7 8
```

在本例子中，对应该初始状态，动作序列`[LEFT, LEFT]`可以将该状态恢复为目标状态，也就是把0和其左边的数码交换两次。

完成Assignement1项目中的五个函数

Search.cpp中

```c++
std::vector<Action> bfs(Problem& problem);
std::vector<Action> recursiveDLS(Node& node, Problem& problem, int limit);
std::vector<Action> aStar(Problem& problem, heuristicFunc heuristicFunc);
```

Heuristic.cpp中

```c++
int misplace(std::vector<int>& state, std::vector<int>& goalState);
int manhattan(std::vector<int>& state, std::vector<int>& goalState);
```

#### Tips

1. `main.cpp`中的main函数为该项目的主入口。
2. `void doExperiment(int iteration, int problemSize, int shuffleSteps, searchFunc searchFunc)`函数可用于调试和检验算法的效率。
3. `void testSearchFunction(searchFunc searchFunc)`函数用于检验搜索算法的正确性。



## 评估标准

1. 搜索算法的正确性：算法得出的结果可以通过testSearchFunction函数的验证。
2. 搜索算法的搜索时间：每个搜索算法都可以在2分钟内跑出结果。
3. 内存管理：算法的内存消耗需要符合该算法应有的空间复杂度。
