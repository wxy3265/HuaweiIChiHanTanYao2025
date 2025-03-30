#include "tag.h"
// 用于保存每个 tag 的统计信息

Tag Tag::disk_tag;

struct TagInfo {
    int tagId;     // tag 编号（1-indexed）
    int netBlocks; // 累计净块数 = ∑(写入 - 删除)，负数按 0 处理
};

// 每个解表示为一个长度为 M 的数组：solution[i] 表示 tag i+1 分配到的磁盘编号（1-indexed）
struct Solution {
    vector<int> assignment; // assignment[i] in [1, N]
    double fitness;         // 适应度，越小越好
};

const int POP_SIZE = 50;         // 种群规模
const int MAX_GENERATIONS = 200;   // 最大迭代代数

// 全局参数（由输入得到）
int nRounds, M, N, V;
// 前 n 轮数据：每个 tag 的删除、写入、读取块数（下标从0开始，tag编号 1~M）
vector<vector<int>> freDel;
vector<vector<int>> freWrite;
vector<vector<int>> freRead;

// 根据每个 tag 的净块数，计算每个磁盘的总负载，返回一个 vector，索引 0 对应磁盘1，以此类推
vector<int> computeDiskLoads(const vector<int>& tagNet, const vector<int>& assignment) {
    vector<int> diskLoads(N, 0);
    for (int i = 0; i < M; i++){
        int disk = assignment[i] - 1; // 1-indexed转换为 0-indexed
        diskLoads[disk] += tagNet[i];
    }
    return diskLoads;
}

// 适应度函数：
// 1. 对于每个磁盘，如果负载超过容量 V，则增加惩罚：惩罚系数较大（例如每超过一个块惩罚 1e5）；
// 2. 同时计算各盘负载的均方差，鼓励负载均衡。
double evaluateSolution(const vector<int>& tagNet, const vector<int>& assignment) {
    vector<int> diskLoads = computeDiskLoads(tagNet, assignment);
    double penalty = 0.0;
    double sum = 0;
    for (int d = 0; d < N; d++){
        if(diskLoads[d] > V) {
            penalty += (diskLoads[d] - V) * 1e5; // 超出容量的惩罚
        }
        sum += diskLoads[d];
    }
    double avg = sum / N;
    double var = 0.0;
    for (int d = 0; d < N; d++){
        var += (diskLoads[d] - avg) * (diskLoads[d] - avg);
    }
    // 适应度 = 惩罚 + 方差
    return penalty + var;
}

// 交叉操作：随机选择一个交叉点，对两个父代进行单点交叉，生成子代解
Solution crossover(const Solution& parent1, const Solution& parent2, const vector<int>& tagNet) {
    int point = rand() % M;
    Solution child;
    child.assignment.resize(M);
    for (int i = 0; i < M; i++){
        if(i < point)
            child.assignment[i] = parent1.assignment[i];
        else
            child.assignment[i] = parent2.assignment[i];
    }
    child.fitness = evaluateSolution(tagNet, child.assignment);
    return child;
}

// 变异操作：随机选择一个 tag，随机改变其分配的磁盘（1~N）
void mutate(Solution& sol, const vector<int>& tagNet) {
    int idx = rand() % M;
    sol.assignment[idx] = (rand() % N) + 1;
    sol.fitness = evaluateSolution(tagNet, sol.assignment);
}

// 生成一个随机初始解
Solution generateRandomSolution(const vector<int>& tagNet) {
    Solution sol;
    sol.assignment.resize(M);
    for (int i = 0; i < M; i++){
        sol.assignment[i] = (rand() % N) + 1;
    }
    sol.fitness = evaluateSolution(tagNet, sol.assignment);
    return sol;
}

// 遗传算法：输入每个 tag 的净块数（tagNet）
// 返回最佳解，即 tag 到磁盘的映射（assignment 数组）
Solution geneticAlgorithm(const vector<int>& tagNet) {
    vector<Solution> population;
    // 初始化种群
    for (int i = 0; i < POP_SIZE; i++){
        population.push_back(generateRandomSolution(tagNet));
    }
    // 迭代进化
    for (int gen = 0; gen < MAX_GENERATIONS; gen++){
        // 排序：适应度越小越好
        sort(population.begin(), population.end(), [](const Solution& a, const Solution& b) {
            return a.fitness < b.fitness;
        });
        vector<Solution> newPopulation;
        // 精英保留（保留前 10% 的优秀解）
        int eliteCount = POP_SIZE / 10;
        for (int i = 0; i < eliteCount; i++){
            newPopulation.push_back(population[i]);
        }
        // 产生剩余个体
        while(newPopulation.size() < population.size()){
            int idx1 = rand() % (POP_SIZE / 2);
            int idx2 = rand() % (POP_SIZE / 2);
            Solution child = crossover(population[idx1], population[idx2], tagNet);
            if(rand() % 100 < 20) { // 20% 的变异概率
                mutate(child, tagNet);
            }
            newPopulation.push_back(child);
        }
        population = newPopulation;
    }
    // 返回最佳解
    sort(population.begin(), population.end(), [](const Solution& a, const Solution& b) {
        return a.fitness < b.fitness;
    });
    return population[0];
}

map<int,int> Tag::handle_tag(){
    srand(time(NULL));
    // 输入格式：第一行 nRounds M N V
    // cin >> nRounds >> M >> N >> V;
    nRounds = Global::tag_block;
    M = Global::tag_num;
    N = Global::disk_num;
    V = Global::disk_size;
    // 读取前 n 轮数据：每个 tag 对应 nRounds 个数（下标从0开始）
    // 先读删除数据
    freDel.resize(M, vector<int>(nRounds, 0));
    for (int i = 0; i < M; i++  ){
        for (int j = 0; j < nRounds; j++){
            freDel[i][j] = Global::fre_del[i][j];
        }
    }
    // 读写入数据
    freWrite.resize(M, vector<int>(nRounds, 0));
    for (int i = 0; i < M; i++){
        for (int j = 0; j < nRounds; j++){
            freWrite[i][j] = Global::fre_write[i][j];
        }
    }
    // 读读取数据（本题中不参与分配计算）
    freRead.resize(M, vector<int>(nRounds, 0));
    for (int i = 0; i < M; i++){
        for (int j = 0; j < nRounds; j++){
            freRead[i][j] = Global::fre_read[i][j];
        }
    }
    // 计算每个 tag 的累计净块数：∑(写入 - 删除)，负数归零
    vector<int> tagNet(M, 0);
    for (int i = 0; i < M; i++){
        int sum = 0, maxn = 0;
        for (int j = 0; j < nRounds; j++){
            // sum += freWrite[i][j] - freDel[i][j];
            maxn = max(maxn, freRead[i][j] + freWrite[i][j]); // 峰值读入
        }
        tagNet[i] = max(maxn, 0);
    }

    // 使用遗传算法搜索 tag→磁盘 的最佳映射方案
    Solution bestSolution = geneticAlgorithm(tagNet);

    // 将结果保存到 map 中： tag id -> 磁盘 id
    map<int, int> tagToDisk;
    for (int i = 0; i < M; i++){
        tagToDisk[i+1] = bestSolution.assignment[i];
    }

    // 输出映射结果
    for (const auto &p : tagToDisk) {
        cerr << "Tag " << p.first << " -> Disk " << p.second << endl;
    }

    return tagToDisk;
}
