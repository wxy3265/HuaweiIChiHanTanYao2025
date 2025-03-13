#include "disk.h"

struct write_result {
    int obj_id;
    vector<int> stored_block_ids;
};

class Writer {
public:
    Writer() : disk_id(-1) {}
    explicit Writer(int disk_id) : disk_id(disk_id) {}

protected:
    int disk_id;
};

class BruteWriter: public Writer {
public:
    BruteWriter() = default;
    explicit BruteWriter(int d_id) { disk_id = d_id; }
    void emplace_task(Task task);
    virtual vector<write_result> get_write_results();
private:
    queue<Task> task_queue;
};
