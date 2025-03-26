#include "read_regulator.h"

void ReadRegulator::get_request_from_interaction() {
    int request_num;
    cin >> request_num;
    for (int i = 0 ; i < request_num; i++) {
        int task_id, obj_id;
        cin >> task_id >> obj_id;
        Task task = Task(Global::now_frame, obj_id, task_id);
        objid_tasks_map[obj_id].emplace_back(task);
    }
}

/**
 * @return deleted obj_id
 */
vector<int> ReadRegulator::handle_delete() {
    vector<int> deleted_obj_ids;
    set<int> deleted_obj_ids_set;
    vector<int> canceled_task;
    int request_nums;
    cin >> request_nums;
    for (int i = 0; i < request_nums; i++) {
        int obj_id;
        cin >> obj_id;
        deleted_obj_ids_set.insert(obj_id);
        deleted_obj_ids.emplace_back(obj_id);
        Object::object_map[obj_id].kill();
        if (objid_tasks_map.count(obj_id) == 0) continue;
        for (int i = 0; i < objid_tasks_map[obj_id].size(); i++) {
            Task task = objid_tasks_map[obj_id][i];
            if (task.is_done()) {
                objid_tasks_map[obj_id].erase(objid_tasks_map[obj_id].begin() + i);
                continue;
            }
            canceled_task.emplace_back(task.get_task_id());
        }
        objid_tasks_map.erase(obj_id);
    }
    cout << canceled_task.size() << '\n';
    for (auto task_id: canceled_task) {
        cout << task_id << '\n';
    }
    cout.flush();
    // for (int i = 0; i < Global::disk_num; i++) {
    //     Disk::disks[i].delete_objs(deleted_obj_ids_set);//change in 3.26 20:29
    // }
    for (auto it : deleted_obj_ids_set) {
        int blockSize = Object::object_map[it].blockPosition.size();
        for (int i = 0; i < blockSize; i++) {
            BlockPosition Oneblock = Object::object_map[it].blockPosition[i];
            Disk::disks[Oneblock.diskId].delete_block(Oneblock.blockId);
        }
    }

    return deleted_obj_ids;
}

