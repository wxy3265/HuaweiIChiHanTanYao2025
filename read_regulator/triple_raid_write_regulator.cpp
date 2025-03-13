#include "write_regulator.h"

TripleRaidWriteRegulator::TripleRaidWriteRegulator() {
    for (int i = 0; i < Global::MAX_DISK_NUM; i++) {
        writers[i] = BruteWriter(i);
    }
    for (int i = 0; i < Global::disk_num; i++) {
        disk_remain_map[i] = Global::disk_size;
        disk_stored_obj_id_set[i] = set<int>();
    }
}

void TripleRaidWriteRegulator::handle_write() {
    get_request_from_interaction();
    while(!requests.empty()) {
        Task task = requests.front(); requests.pop();
        vector <int> target_disks;
        while (target_disks.size() < 3) {
            if (disk_remain_map[disk_cursor] < task.get_obj().get_size()) continue;
            target_disks.emplace_back(disk_cursor++);
            disk_cursor %= Global::disk_num;
        }
        for (auto disk_id: target_disks) {
            writers[disk_id].emplace_task(task);
        }
    }
    // arrange to object
    map<int,int> obj_disk_position[3]; // [replica id][obj id] -> disk id
    map<int,vector<int>> obj_block_position[3]; // [replica id][obj id] -> block position in disk
    map<int,int> obj_cnt; // obj id -> replica cnt
    for (int i = 0; i < Global::disk_num; i++) {
        vector<write_result> results = writers[i].get_write_results();
        for (auto result: results) {
            if (!obj_cnt.count(result.obj_id)) obj_cnt[result.obj_id] = 0;
            obj_disk_position[obj_cnt[result.obj_id]][result.obj_id] = i;
            for (auto id : result.stored_block_ids) {
                obj_block_position[obj_cnt[result.obj_id]][result.obj_id].emplace_back(id);
            }
            obj_cnt[result.obj_id]++;
        }
    }
    for (auto it = obj_cnt.begin(); it != obj_cnt.end(); it++) {
        int obj_id = it->first;
        cout << obj_id << '\n';
        // write replicas
        for (int i = 0; i < 3; i++) {
            cout << obj_disk_position[i][obj_id] + 1 << ' ';
            for (auto block_id: obj_block_position[i][obj_id]) {
                cout << block_id + 1 << ' ';
            }
            cout << '\n';
        }
        cout.flush();
    }
}
