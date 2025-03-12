#include "write_regulator.h"

void WriteRegulator::get_request_from_interaction() {
    int request_num;
    cin >> request_num;
    for (int i = 0 ; i < request_num; i++) {
        int id, size, tag;
        Object obj = Object(id, size, tag);
        Object::object_map[id] = obj;
        Task task = Task(Global::now_frame, obj);
        requests.push(task);
    }
}

MostGreedWriteRegulator::MostGreedWriteRegulator() {
    for (int i = 0; i < Global::MAX_DISK_NUM; i++) {
        writers[i] = BruteWriter(i);
    }
    for (int i = 0; i < Global::disk_num; i++) {
        disk_remain_map[i] = Global::disk_size;
        disk_stored_obj_id_set[i] = set<int>();
    }
}

void MostGreedWriteRegulator::handle_write() {
    get_request_from_interaction();
    while(!requests.empty()) {
        Task task = requests.front(); requests.pop();
        vector <int> target_disks;
        while (target_disks.size() < 3) {
            if (disk_remain_map[disk_cursor] < task.get_obj().get_size()) continue;
            target_disks.emplace_back(disk_cursor++);
        }
        for (auto disk_id: target_disks) {
            writers[disk_id].emplace_task(task);
        }
    }
    // 按对象重新整合
    map<int,int> obj_disk_position[3]; // obj id -> disk position [replica id]
    for (int i = 0; i < Global::disk_num; i++) {
        vector<write_result> results = writers[i].get_write_results();
        for (auto result: results) {
            ;
        }
    }
}

void MostGreedWriteRegulator::update_deleted(const vector<int>& deleted_obj_ids) {
    for (auto obj_id: deleted_obj_ids) {
        Object obj = Object::object_map[obj_id];
        for (int i = 0; i < Global::disk_num; i++) {
            if (disk_stored_obj_id_set[i].count(obj_id) > 0) {
                disk_remain_map[i] -= obj.get_size();
                disk_stored_obj_id_set[i].erase(obj_id);
            }
        }
    }
}
