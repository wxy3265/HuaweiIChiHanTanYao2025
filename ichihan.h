#ifndef ICHIHAN_H
#define ICHIHAN_H
#include <unordered_map>
#include <algorithm>
#include <iostream>
#include <cstring>
#include <numeric>
#include <string>
#include <cstdio>
#include <vector>
#include <random>
#include <limits>
#include <queue>
#include <stack>
#include <cmath>
#include <ctime>
#include <list>
#include <map>
#include <set>
#define map unordered_map
using namespace std;

inline int read_Fast() {
  int s = 0, f = 0; char ch = getchar();
  while (!isdigit(ch)) f |= (ch == '-'), ch = getchar();
  while (isdigit(ch)) s = s * 10 + (ch ^ 48), ch = getchar();
  return f ? -s : s;
}

// 手操配置
class Config {
};
#endif
