#include "disk.hpp"

struct write_operation {
    int obj_id;
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
                disk[disk_id].store(block_id, op.obj_id);
            }
            cout << '\n';
            frame_operations_map[Global::now_frame].pop();
        }
        cout.flush();
    }

    void add_operation(int frame, write_operation op) {
        frame_operations_map[frame].push(op);
    }

private:
    int disk_id;
    map<int, queue<write_operation>> frame_operations_map;
};
