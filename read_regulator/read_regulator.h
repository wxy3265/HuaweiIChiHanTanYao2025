#include "reader.h"

// 跨盘读策略
class ReadRegulator {
public:
    virtual vector<int> handle_delete(); // 处理删请求
    virtual void handle_read() = 0; // 处理读请求

protected:
    queue<Task> requests; // 所有object的读请求队列（目前仅作为判题器读入的缓存）
    map<int, vector<Task>> objid_tasks_map; // 每个object的读请求队列
    
    void get_request_from_interaction();

};

class MostGreedReadRegulator: public ReadRegulator {
public:
    MostGreedReadRegulator(); // 构造函数，做一些初始化工作
    void handle_read() override;
private:
    ReadOnlyGreedReader readers[Global::MAX_DISK_NUM]; // 每个盘的reader
};
