#include "reader.h"

/**
 * @return completed task ids
 */
vector<int> ReadOnlyGreedReader::read(map<int, vector<Task>> *tarMap) {
    map<int,vector<Task>> &objid_tasks_map = *tarMap;
    remain_tokens = Global::total_tokens;
    int pre_tokens = 64;
    int pre_head = Disk::disks[disk_id].get_head();
    vector<int> completed_task_ids;
    while (remain_tokens > pre_tokens) {
        remain_tokens -= pre_tokens;
        pre_tokens = max(16.0, ceil(pre_tokens * 0.8));
        Disk::disks[disk_id].move_head(1);
        cout << "r";
        ObjectBlock block = Disk::disks[disk_id].pick_block(pre_head);
        if (!block.is_null()) {
            for (int i = 0; i < objid_tasks_map[block.get_obj_id()].size(); i++) {
                Task task = objid_tasks_map[block.get_obj_id()][i];
                if (task.is_done()) continue;
                task.complete(block.get_obj_block_id());
                if (task.is_done()) {
                    completed_task_ids.emplace_back(task.get_task_id());
                }
                objid_tasks_map[block.get_obj_id()][i] = task;
            }
        }
        pre_head = Disk::disks[disk_id].get_head();
    }
    cout << "#\n";
    cout.flush();
    return completed_task_ids;
}
