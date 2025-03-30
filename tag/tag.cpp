//
// Created by Lenovo on 25-3-29.
//
#include "tag.h"

Tag Tag::tags[Global::MAX_TAG_NUM];

random_device rd;
mt19937 rng(rd());

const double PENALTY = 1e6; // 重叠或越界惩罚

struct Gene {
    int disk;       // 分配的磁盘编号，范围：0 ~ diskCount-1
    int pos;        // 分配的起始位置（1-based）
};

struct Chromosome {
    // 对于 tag i（i=0..M-1），assignment[i] 表示其分配情况
    vector<Gene> assignment;
};

// 适应度函数：计算染色体的总成本，越低越好
// 每个 tag 的区域长度由 tags[i].occupied 给出
double fitness(const Chromosome &chrom, const vector<Tag> &tag, int diskSize, int diskCount) {
    double cost = 0;
    int M = tag.size();
    // 每个 tag 的位置成本： (pos-1) * weight
    for (int i = 0; i < M; i++) {
        cost += (chrom.assignment[i].pos - 1) * tag[i].get_weight();
    }

    // 对每个磁盘检查分配区域是否有重叠或超出磁盘范围
    for (int d = 0; d < diskCount; d++) {
        // 收集分配到磁盘 d 的 tag 区域，区域 = [start, end]
        vector<pair<int,int>> regions;
        for (int i = 0; i < M; i++) {
            if (chrom.assignment[i].disk == d) {
                int start = chrom.assignment[i].pos;
                int regionLength = tag[i].get_occupied(); // 使用 occupied 而非 write
                int end = start + regionLength - 1;
                regions.push_back({start, end});
                // 如果区域超出磁盘范围，增加惩罚
                if (end > diskSize) {
                    cost += PENALTY * (end - diskSize);
                }
            }
        }
        // 按起始位置排序，检查是否有重叠
        sort(regions.begin(), regions.end());
        for (size_t i = 1; i < regions.size(); i++) {
            if (regions[i-1].second >= regions[i].first) {
                cost += PENALTY;
            }
        }
    }
    return cost;
}

// 初始化一个随机染色体：随机为每个 tag 分配一个磁盘和合法的起始位置
Chromosome randomChromosome(const vector<Tag> &tag, int diskSize, int diskCount) {
    int M = tag.size();
    Chromosome chrom;
    chrom.assignment.resize(M);
    for (int i = 0; i < M; i++) {
        int disk = uniform_int_distribution<int>(0, diskCount - 1)(rng);
        // 合法起始位置范围：1 到 (diskSize - occupied + 1)
        int maxPos = max(1, diskSize - tag[i].get_occupied() + 1);
        int pos = uniform_int_distribution<int>(1, maxPos)(rng);
        chrom.assignment[i] = {disk, pos};
    }
    return chrom;
}

// 锦标赛选择
Chromosome tournamentSelection(const vector<Chromosome> &population,
                               const vector<Tag> &tag, int diskSize, int diskCount,
                               int tournamentSize = 3) {
    int popSize = population.size();
    double bestFit = numeric_limits<double>::max();
    int bestIndex = -1;
    for (int i = 0; i < tournamentSize; i++) {
        int idx = uniform_int_distribution<int>(0, popSize - 1)(rng);
        double f = fitness(population[idx], tag, diskSize, diskCount);
        if (f < bestFit) {
            bestFit = f;
            bestIndex = idx;
        }
    }
    return population[bestIndex];
}

// 均匀交叉：对每个 tag 随机选择父代中的一个基因
Chromosome crossover(const Chromosome &parent1, const Chromosome &parent2,
                     const vector<Tag> &tag, int diskSize, int diskCount) {
    int M = parent1.assignment.size();
    Chromosome child;
    child.assignment.resize(M);
    for (int i = 0; i < M; i++) {
        if (uniform_real_distribution<double>(0.0, 1.0)(rng) < 0.5)
            child.assignment[i] = parent1.assignment[i];
        else
            child.assignment[i] = parent2.assignment[i];

        // 保证基因合法：如果超出范围则重新生成
        int maxPos = max(1, diskSize - tag[i].get_occupied() + 1);
        if (child.assignment[i].pos < 1 || child.assignment[i].pos > maxPos) {
            child.assignment[i].pos = uniform_int_distribution<int>(1, maxPos)(rng);
        }
        if (child.assignment[i].disk < 0 || child.assignment[i].disk >= diskCount) {
            child.assignment[i].disk = uniform_int_distribution<int>(0, diskCount - 1)(rng);
        }
    }
    return child;
}

// 变异：随机修改某个 tag 的磁盘或起始位置
void mutate(Chromosome &chrom, const vector<Tag> &tag, int diskSize, int diskCount, double mutationRate) {
    int M = chrom.assignment.size();
    for (int i = 0; i < M; i++) {
        if (uniform_real_distribution<double>(0.0, 1.0)(rng) < mutationRate) {
            if (uniform_real_distribution<double>(0.0, 1.0)(rng) < 0.5) {
                chrom.assignment[i].disk = uniform_int_distribution<int>(0, diskCount - 1)(rng);
            } else {
                int maxPos = max(1, diskSize - tag[i].get_occupied() + 1);
                chrom.assignment[i].pos = uniform_int_distribution<int>(1, maxPos)(rng);
            }
        }
    }
}

Chromosome handle_tag() {
    static int round = 0;
    const int popSize = 100;
    const int generations = 500;
    const double mutationRate = 0.1;

    cerr << "\n==== 第 " << ++round << " 轮 ====\n";
    // 输入本轮 tag 数据：删除、写入、读取量
    if (round == 1) {
        for (int i = 0; i < Global::MAX_TAG_NUM; i++) {
            Tag::tags[i].set_del()
        }
    }
    for (int i = 0; i < Global::MAX_TAG_NUM; i++) {
        Tag::tags[i].set_weight();
    }

    // 本轮规划时，规划数据中 tag 的 occupied 需要按轮次区分：
    // 对于第1轮：估计 occupied = write * 3
    // 对于后续轮次：要求用户输入上一轮实际占据的磁盘空间数（这里直接输入）
    vector<Tag> tags_plan(Global::MAX_TAG_NUM);
    if (round == 1) {
        for (int i = 0; i < Global::MAX_TAG_NUM; i++) {
            tags_plan[i] = tags_new[i];
            tags_plan[i].occupied = tags_new[i].write * 3;
        }
    } else {
        // 使用累计数据 tags_cum 进行规划（不使用本轮新数据），
        // 并要求用户输入上一轮实际占用的磁盘空间数，作为 tags_plan[i].occupied
        for (int i = 0; i < M; i++) {
            tags_plan[i] = tags_cum[i];
            int occ;
            cout << "请输入 tag " << i + 1 << " 上一轮实际占用的磁盘空间数: ";
            cin >> occ;
            tags_plan[i].occupied = occ;
        }
    }

    // 运行遗传算法，根据 tags_plan 规划分配方案
    vector<Chromosome> population;
    for (int i = 0; i < popSize; i++) {
        population.push_back(randomChromosome(tags_plan, Global::disk_size / 3, Global::disk_num));
    }

    Chromosome bestChrom;
    double bestFit = numeric_limits<double>::max();
    for (int gen = 0; gen < generations; gen++) {
        vector<Chromosome> newPopulation;
        for (int i = 0; i < popSize; i++) {
            Chromosome parent1 = tournamentSelection(population, tags_plan, diskSize, diskCount);
            Chromosome parent2 = tournamentSelection(population, tags_plan, diskSize, diskCount);
            Chromosome child = crossover(parent1, parent2, tags_plan, diskSize, diskCount);
            mutate(child, tags_plan, diskSize, diskCount, mutationRate);
            newPopulation.push_back(child);
            double f = fitness(child, tags_plan, diskSize, diskCount);
            if (f < bestFit) {
                bestFit = f;
                bestChrom = child;
            }
        }
        population = newPopulation;
        // 可选：输出每代最优适应度
        // cout << "Generation " << gen << ", best fitness: " << bestFit << endl;
    }

    // 输出本轮规划方案
    cout << "\n第 " << round << " 轮规划方案：" << endl;
    for (size_t i = 0; i < bestChrom.assignment.size(); i++) {
        int disk = bestChrom.assignment[i].disk;
        int pos = bestChrom.assignment[i].pos;
        cout << "Tag " << i + 1 << ": 分配到磁盘 " << disk + 1 << ", 起始位置 " << pos
            << ", 占用长度 " << tags_plan[i].occupied
            << ", 权重 " << tags_plan[i].weight << endl;
    }
    cout << "适应度值: " << bestFit << endl;

    // 更新累计数据 tags_cum：将本轮新数据累加（累计，不用于当前规划，但用于后续轮次）
    for (int i = 0; i < M; i++) {
        tags_cum[i].del   += tags_new[i].del;
        tags_cum[i].write += tags_new[i].write;
        tags_cum[i].read  += tags_new[i].read;
        tags_cum[i].weight = tags_cum[i].del + tags_cum[i].write + tags_cum[i].read;
        // 对于累计数据的 occupied，这里可以选择保留上一轮的实际占用数据，
        // 或者重新估计，目前不作更新，由下一轮输入实际占用数据更新
    }
}
