#include "write_regulator.h"

MostGreedWriteRegulator::MostGreedWriteRegulator() {
    for (int i = 0; i < Global::MAX_DISK_NUM; i++) {
        writers[i] = BruteWriter(i);
        RWAreaWriters[i] = BruteWriter(i);
        BackupAreaWriters[i] = BruteWriter(i);
    }
    for (int i = 0; i < Global::disk_num; i++) {
        disk_remain_map[i] = Global::disk_size;
        diskRWRemainMap[i] = Disk::disks[i].getRWAreaSize();
        diskBackupRemainMap[i] = Global::disk_size - Disk::disks[i].getRWAreaSize();
        disk_stored_obj_id_set[i] = set<int>();
    }
}

void MostGreedWriteRegulator::handle_write() {
    get_request_from_interaction();
    while(!requests.empty()) {
        Task task = requests.front(); requests.pop();
        vector <int> target_disks;
        bool used[10];
        for (int i = 0; i < 10; i++) used[i] = false;
        while (target_disks.size() < 3) {
            if (used[disk_cursor] == true || disk_remain_map[disk_cursor] < Object::object_map[task.get_obj_id_in_task()].get_size()) {
                disk_cursor++;
                disk_cursor %= Global::disk_num;
                cerr << "Could not find a normal disk: diskID:"<< disk_cursor<< ':' << disk_remain_map[disk_cursor] << "\n";
                continue;
            }
            disk_remain_map[disk_cursor] -= Object::object_map[task.get_obj_id_in_task()].get_size();
            used[disk_cursor] = true;
            disk_stored_obj_id_set[disk_cursor].insert(task.get_obj_id_in_task());
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

struct node {
    int disk_id, diskNum;
    bool operator < (const node &b) const {
        return diskNum < b.diskNum;
    }
};

void MostGreedWriteRegulator::handleWriteWithTwoArea() {
    get_request_from_interaction();
    int tims = 0;
    while(!requests.empty()) {
        tims++;
        Task task = requests.front(); requests.pop();
        vector <int> target_disks;
        // bool used[10];
        // for (int i = 0; i < 10; i++) used[i] = false;
        
        int RWDisk = -1;
        priority_queue<node> checkA;
        for (int i = 0; i < Global::disk_num; i++) {
            node num;
            num.disk_id = i, num.diskNum = diskRWRemainMap[i];
            checkA.push(num);
        }
        
        while (RWDisk == -1) {
            RWAreaDiskCursor = checkA.top().disk_id;
            checkA.pop();
            if (diskRWRemainMap[RWAreaDiskCursor] < Object::object_map[task.get_obj_id_in_task()].get_size()) {
                // cerr << RWAreaDiskCursor << " " << diskRWRemainMap[RWAreaDiskCursor] << " " << Object::object_map[task.get_obj_id_in_task()].get_size() << "\n";
                // cerr << task.get_obj_id_in_task() << " " << diskBackupRemainMap[RWAreaDiskCursor] << "\n";
                // RWAreaDiskCursor++;
                // RWAreaDiskCursor %= Global::disk_num;
                continue;
            }
            diskRWRemainMap[RWAreaDiskCursor] -= Object::object_map[task.get_obj_id_in_task()].get_size();
            // used[RWAreaDiskCursor] = true;
            disk_stored_obj_id_set[RWAreaDiskCursor].insert(task.get_obj_id_in_task());
            RWDisk = RWAreaDiskCursor;
            RWAreaWriters[RWAreaDiskCursor].emplace_task(task);
        }

        while (!checkA.empty()) checkA.pop();
        for(int i = 0; i < Global::disk_num; i++) {
            if (i == RWDisk) continue;

            node num;
            num.disk_id = i;
            num.diskNum = diskBackupRemainMap[i];
            checkA.push(num);
        }
        while (target_disks.size() < 2) {
            BackupDiskCursor = checkA.top().disk_id;
            checkA.pop();
            
            if (diskBackupRemainMap[BackupDiskCursor] < Object::object_map[task.get_obj_id_in_task()].get_size()) {
                
                // BackupDiskCursor++;
                // BackupDiskCursor %= Global::disk_num;
                continue;
            }
            diskBackupRemainMap[BackupDiskCursor] -= Object::object_map[task.get_obj_id_in_task()].get_size();
            // used[BackupDiskCursor] = true;
            disk_stored_obj_id_set[BackupDiskCursor].insert(task.get_obj_id_in_task());
            target_disks.emplace_back(BackupDiskCursor++);
            BackupDiskCursor %= Global::disk_num;
        }
        for (auto disk_id: target_disks) {
            BackupAreaWriters[disk_id].emplace_task(task);
        }
    }
    // cerr << "Finish RW and Backup Area" << "\n";
    // arrange to object
    map<int,int> obj_disk_position[3]; // [replica id][obj id] -> disk id
    map<int,vector<int>> obj_block_position[3]; // [replica id][obj id] -> block position in disk
    map<int,int> obj_cnt; // obj id -> replica cnt
    // cerr << "in for" <<"\n";
    for (int i = 0; i < Global::disk_num; i++) {
        // cerr << "for1\n";
        // cerr << "remain size"<<diskRWRemainMap[i] << "\n";
        vector<write_result> results = RWAreaWriters[i].getWriteResultsInRWArea();
        for (auto result: results) {
            if (!obj_cnt.count(result.obj_id)) obj_cnt[result.obj_id] = 0;
            obj_disk_position[obj_cnt[result.obj_id]][result.obj_id] = i;
            for (auto id : result.stored_block_ids) {
                obj_block_position[obj_cnt[result.obj_id]][result.obj_id].emplace_back(id);
            }
            obj_cnt[result.obj_id]++;
        }
        // cerr << "for2\n";
        results = BackupAreaWriters[i].getWriteResultsInBackupArea();
        for (auto result: results) {
            if (!obj_cnt.count(result.obj_id)) obj_cnt[result.obj_id] = 0;
            obj_disk_position[obj_cnt[result.obj_id]][result.obj_id] = i;
            for (auto id : result.stored_block_ids) {
                obj_block_position[obj_cnt[result.obj_id]][result.obj_id].emplace_back(id);
            }
            obj_cnt[result.obj_id]++;
        }
        // cerr << "for3\n";
    }
    // cerr << "Finish for\n";
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

void MostGreedWriteRegulator::update_deleted(const vector<int>& deleted_obj_ids) {
    for (auto obj_id: deleted_obj_ids) {
        Object obj = Object::object_map[obj_id];
        for (int i = 0; i < Global::disk_num; i++) {
            if (disk_stored_obj_id_set[i].count(obj_id) > 0) {
                disk_remain_map[i] += obj.get_size();
                // if (obj.blockPosition[0].blockId >= Disk::disks[i].getRWAreaSize()) 
                //     diskBackupRemainMap[i] += obj.get_size();
                // else diskRWRemainMap[i] += obj.get_size();

                disk_stored_obj_id_set[i].erase(obj_id);
            }
        }
    }
}

void MostGreedWriteRegulator::update_deleted_with_two_area(const vector<int>& deleted_obj_ids) {
    for (auto obj_id: deleted_obj_ids) {
        Object obj = Object::object_map[obj_id];
        cerr << "del obj size:" << obj.get_size() << '\n';
        for (int i = 0; i < obj.blockPosition.size(); i++) {
            if (obj.blockPosition[i].blockId >= Disk::disks[obj.blockPosition[i].diskId].getRWAreaSize()) {
                diskBackupRemainMap[obj.blockPosition[i].diskId] += 1;
                cerr << "release backup:" << obj.blockPosition[i].diskId << '\n';
            } else {
                diskRWRemainMap[obj.blockPosition[i].diskId] += 1;
                cerr << "release rw:" << obj.blockPosition[i].diskId << '\n';
            }
            if (disk_stored_obj_id_set[obj.blockPosition[i].diskId].count(obj_id) > 0) 
                disk_stored_obj_id_set[obj.blockPosition[i].diskId].erase(obj_id);
        }

    }
}