#include "moderator.h"

void MostGreedModerator::init() {
    read_regulator = MostGreedReadRegulator();
    write_regulator = MostGreedWriteRegulator();
}

void MostGreedModerator::execute_frame() {
    vector<int> deleted_obj_ids = read_regulator.handle_delete();
    write_regulator.update_deleted(deleted_obj_ids);
    write_regulator.handle_write();
}

