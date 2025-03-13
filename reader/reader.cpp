#include "reader.h"

void Reader::execute() {
    while (!frame_operations_map[Global::now_frame].empty()) {
        read_operation op = frame_operations_map[Global::now_frame].front();
        act(op);
        frame_operations_map[Global::now_frame].pop();
    }
    cout << '\n';
}

void Reader::reset_frame() {
    remain_tokens = Global::total_tokens;
}

void Reader::act(read_operation op) const {
    switch (op.type)
    {
        case READ:
            cout << "r";
            Disk::disks[disk_id].move_head(1);
            break;
        case PASS:
            cout << "p";
            Disk::disks[disk_id].move_head(1);
            break;
        case JUMP:
            cout << "j" << op.pos;
            Disk::disks[disk_id].jump_head(op.pos);
            break;
        default:
            cerr << "Invalid read operation" << endl;
            break;
    }
    cout.flush();
}
