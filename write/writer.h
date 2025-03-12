#include "disk.h"

struct write_operation {
    int obj_id;
    int obj_block_id;
    vector<int> block_ids;
};

class Writer {
public:
    Writer() : disk_id(-1) {}
    Writer(int disk_id) : disk_id(disk_id) {}

    void execute() {
        while (!frame_operations_map[Global::now_frame].empty()) {
            write_operation op = frame_operations_map[Global::now_frame].front();
            for (int block_id : op.block_ids) {
                cout << block_id << ' ';
                Disk::disks[disk_id].store(block_id, op.obj_id, op.obj_block_id);
            }
            cout << '\n';
            frame_operations_map[Global::now_frame].pop();
        }
        cout.flush();
    }

    void add_operation(int frame, const write_operation& op) {
        frame_operations_map[frame].push(op);
    }

private:
    int disk_id;
    map<int, queue<write_operation>> frame_operations_map;
};
