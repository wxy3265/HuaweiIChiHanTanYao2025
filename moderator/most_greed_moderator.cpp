#include "moderator.h"

void MostGreedModerator::init() {
    read_regulator = MostGreedReadRegulator();
    write_regulator = MostGreedWriteRegulator();
}

void MostGreedModerator::execute_frame() {
    vector<int> deleted_obj_ids = read_regulator.handle_delete();
    write_regulator.update_deleted(deleted_obj_ids);
    cerr << "delete complete time:" << clock() << '\n';
    write_regulator.handle_write();
    cerr << "write complete time:" << clock() << '\n';
    read_regulator.handle_read();
    cerr << "read complete time:" << clock() << '\n';
}

