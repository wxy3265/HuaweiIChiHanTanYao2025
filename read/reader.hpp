#include "disk.hpp"

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
    Reader(int disk_id) : disk_id(disk_id) {}

    void execute() {
        while (!frame_operations_map[Global::now_frame].empty()) {
            read_operation op = frame_operations_map[Global::now_frame].front();
            act(op);
            frame_operations_map[Global::now_frame].pop();
        }
        cout << '\n';
    }

    virtual void reset_frame() {
        remain_tokens = Global::total_tokens;
    }

private:
    int disk_id;
    map<int, queue<read_operation>> frame_operations_map;
    int remain_tokens = 0;

    void act(read_operation op) {
        switch (op.type)
        {
        case READ:
            cout << "r";
            disk[disk_id].move_head(1);
            break;
        case PASS:
            cout << "p";
            disk[disk_id].move_head(1);
            break;
        case JUMP:
            cout << "j" << op.pos;
            disk[disk_id].jump_head(op.pos);
            break;
        default:
            cerr << "Invalid read operation" << endl;
            break;
        }
        cout.flush();
    }

};
