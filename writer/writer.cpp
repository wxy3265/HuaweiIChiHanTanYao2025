#include "writer.h"

void BruteWriter::emplace_task(Task task) {
    task_queue.emplace(task);
}

vector<write_result> BruteWriter::get_write_results() {
    vector<write_result> res;
    int block_cursor = 0;
    while (!task_queue.empty()) {
        Task task = task_queue.front(); task_queue.pop();
        write_result result;
        result.obj_id = task.get_obj().get_id();
        for (int now_obj_block_id = 0; now_obj_block_id < task.get_obj().get_size(); now_obj_block_id++) {
            while (!Disk::disks[disk_id].is_empty(now_obj_block_id)) block_cursor++;
            result.stored_block_ids.emplace_back(block_cursor++);
        }
        res.emplace_back(result);
    }
    return res;
}
