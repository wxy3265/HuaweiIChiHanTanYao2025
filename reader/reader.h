#include "disk.h"

// 刺头操作类型
enum operate_type {
    READ,
    PASS,
    JUMP
};

// 磁头操作
struct head_operation {
    operate_type type; // 操作类型
    int pos; // 读取位置
};

// 每个盘的读策略
class Reader {
public:
    Reader() : disk_id(-1) {}
    explicit Reader(int disk_id) : disk_id(disk_id) {}

    void execute(); // 执行读取操作（只写和判题器交互的cout，不写策略逻辑）
    virtual void init_per_frame(); // 每一帧的初始化
    int get_remain_tokens() const { return remain_tokens; } // 获取剩余token数

protected:
    int disk_id; // 当前 reader 操作的 disk
    map<int, queue<head_operation>> frame_operations_map; // 每一帧的实际操作队列
    int remain_tokens = 0; // 剩余 token 数

    void act(head_operation op) const; // 执行一个操作（只写cout）

};

class ReadOnlyGreedReader: public Reader{
public:
    ReadOnlyGreedReader() { disk_id = -1; }
    explicit ReadOnlyGreedReader(int disk_id) : Reader(disk_id) {}
    vector<int> read(map<int, vector<Task>> *objid_tasks_map); // 读取操作（和策略强相关，不是一定要有的函数）

};
