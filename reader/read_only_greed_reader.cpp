#include "reader.h"

/**
 * @return completed task ids
 */

vector<int> ReadOnlyGreedReader::read(map<int, vector<Task>> *tarMap) {
    // cerr << "Tag in read1:" << clock() << "\n";
    map<int,vector<Task>> &objid_tasks_map = *tarMap;
    
    remain_tokens = Global::total_tokens;
    int pre_tokens = 64;
    int pre_head = Disk::disks[disk_id].get_head();
    vector<int> completed_task_ids;
    int flag = 0;
    while (remain_tokens > pre_tokens) {
        remain_tokens -= pre_tokens;
        pre_tokens = max(16.0, ceil(pre_tokens * 0.8));
        Disk::disks[disk_id].move_head(1);
        cout << "r";
        ObjectBlock block = Disk::disks[disk_id].pick_block(pre_head);
        if (!block.is_null()) {
            // for (auto it = newTaskMap[block.get_obj_block_id()].begin(); it != newTaskMap[block.get_obj_block_id()].end(); ) {
            //     Task task = *it;
            //     task.complete(block.get_obj_block_id());
            //     if (task.is_done()) {
            //         completed_task_ids.emplace_back(task.get_task_id());
            //         it = newTaskMap[block.get_obj_id()].erase(it);
            //         continue;
            //     }
            //     *it = task;
            // }
            for (int i = 0; i < objid_tasks_map[block.get_obj_id()].size(); i++) {
                Task task = objid_tasks_map[block.get_obj_id()][i];
                // if (task.is_done()) continue;
                task.complete(block.get_obj_block_id());
                if (task.is_done()) {
                    completed_task_ids.emplace_back(task.get_task_id());
                    objid_tasks_map[block.get_obj_id()].erase(objid_tasks_map[block.get_obj_id()].begin() + i);
                    continue;
                }
                objid_tasks_map[block.get_obj_id()][i] = task;
            }
        }
        pre_head = Disk::disks[disk_id].get_head();
    }
    cout << "#\n";
    cout.flush();
    // cerr << "Tag in read2:" << clock() << "\n";
    return completed_task_ids;
}
