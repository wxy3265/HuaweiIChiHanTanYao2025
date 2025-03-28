#include "disk.h"

// 每个对象实际存储的位置（由于判题器的交互逻辑，不能直接在writer里cout，要在regulator中进行统筹）
struct write_result {
    int obj_id;
    vector<int> stored_block_ids;
};

// 每个磁盘的写策略
class Writer {
public:
    Writer() : disk_id(-1) {}
    explicit Writer(int disk_id) : disk_id(disk_id) {}

protected:
    int disk_id; // writer操作的 disk
};

class BruteWriter: public Writer {
public:
    BruteWriter() = default;
    explicit BruteWriter(int d_id) { disk_id = d_id; }
    void emplace_task(Task task); // 给这个writer添加一个任务（将一个任务分配给这个磁盘）
    virtual vector<write_result> get_write_results(); // 返回这个writer的任务完成情况（把每个任务object安排到磁盘哪个位置）
    virtual vector<write_result> getWriteResultsInRWArea(); // 返回从读写区wirter的任务完成情况。
    virtual vector<write_result> getWriteResultsInBackupArea(); 
private:
    queue<Task> task_queue; // 每个writer的任务队列
};
