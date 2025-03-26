#include "reader.h"

/**
 * @return completed task ids
 */
vector<int> ReadOnlyGreedReader::read(map<int, vector<Task>> *tarMap) {
    map<int,vector<Task>> &objid_tasks_map = *tarMap;
    // map<int, multiset<Task> >newTaskMap;
    // for (auto it : objid_tasks_map) {
    //     // cerr << it.first << "\n";
    //     for (int i = 0; i < objid_tasks_map[it.first].size(); i++) {
    //         // cerr << objid_tasks_map[it.first][i].get_obj_id_in_task() << "\n";
    //         newTaskMap[it.first].insert(objid_tasks_map[it.first][i]);
    //     }
    // }
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
            // set<Task> opratorSetTask;
            // for (auto it : newTaskMap[block.get_obj_block_id()]) {
            //     Task task = it;
            //     newTaskMap[block.get_obj_block_id()].erase(task);
            //     task.complete(block.get_obj_block_id());
            //     if (task.is_done()) {
            //         completed_task_ids.emplace_back(task.get_task_id());
            //         continue;
            //     } else opratorSetTask.insert(task);
            // }
            // newTaskMap[block.get_obj_block_id()] = opratorSetTask;
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
    return completed_task_ids;
}
