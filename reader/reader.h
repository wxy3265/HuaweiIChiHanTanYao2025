#include "disk.h"

enum operate_type {
    READ,
    PASS,
    JUMP
};

struct read_operation {
    operate_type type;
    int pos;
};

class Reader {
public:
    Reader() : disk_id(-1) {}
    explicit Reader(int disk_id) : disk_id(disk_id) {}

    void execute();
    virtual void reset_frame();
    int get_remain_tokens() const { return remain_tokens; }

protected:
    int disk_id;
    map<int, queue<read_operation>> frame_operations_map;
    int remain_tokens = 0;

    void act(read_operation op) const;

};

class ReadOnlyGreedReader: public Reader{
public:
    ReadOnlyGreedReader() { disk_id = -1; }
    explicit ReadOnlyGreedReader(int disk_id) : Reader(disk_id) {}
    vector<int> read(map<int, vector<Task>> *objid_tasks_map);

};
