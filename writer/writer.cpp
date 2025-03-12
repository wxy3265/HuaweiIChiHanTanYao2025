#include "writer.h"

void Writer::execute() {
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

void Writer::add_operation(int frame, const write_operation& op) {
    frame_operations_map[frame].push(op);
}
