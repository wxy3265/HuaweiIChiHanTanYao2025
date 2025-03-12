#include "disk.h"

struct write_operation {
    int obj_id;
    int obj_block_id;
    vector<int> block_ids;
};

class Writer {
public:
    Writer() : disk_id(-1) {}
    explicit Writer(int disk_id) : disk_id(disk_id) {}

    void execute();
    void add_operation(int frame, const write_operation& op);

private:
    int disk_id;
    map<int, queue<write_operation>> frame_operations_map;
};
