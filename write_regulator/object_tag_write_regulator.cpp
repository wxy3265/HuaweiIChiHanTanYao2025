#include "write_regulator.h"

ObjectTagWriteRegulator::ObjectTagWriteRegulator() {

    for (int i = 0; i < Global::MAX_DISK_NUM; i++) {
        BackupAreaWriters[i] = BruteWriter(i);
        object_writers[i] = ObjectWriter(i);
    }
    for (int i = 0; i < Global::disk_num; i++) {
        diskBackupRemainMap[i] = Global::disk_size - Disk::disks[i].getRWAreaSize();
        disk_stored_obj_id_set[i] = set<int>();
    }
    tag_disk_map = Tag::disk_tag.handle_tag();
}

void ObjectTagWriteRegulator::handle_write() {
    get_request_from_interaction();
    map<int,int> obj_disk_position[3]; // [replica id][obj id] -> disk id
    map<int,vector<int>> obj_block_position[3]; // [replica id][obj id] -> block position in disk
    map<int,int> obj_cnt; // obj id -> replica cnt

    while(!requests.empty()) {
        Task task = requests.front(); requests.pop();
        vector <int> target_disks;
        // bool used[10];
        // for (int i = 0; i < 10; i++) used[i] = false;

        // handle rw
        int RWDisk = -1;
        int start_pos = -1;
        int obj_tag_in_disk = tag_disk_map[Object::object_map[task.get_obj_id_in_task()].get_tag() + 1] - 1;
        RWAreaDiskCursor = obj_tag_in_disk;
        // cerr << "CheckCursor and tag " << RWAreaDiskCursor << " " << obj_tag_in_disk << " " << task.get_obj_id_in_task() << "\n";
        while (start_pos == -1) {
            start_pos = object_writers[RWAreaDiskCursor].write_and_get_start_position(
                Object::object_map[task.get_obj_id_in_task()].get_size());
            if (start_pos != -1) RWDisk = RWAreaDiskCursor;
            RWAreaDiskCursor++;
            RWAreaDiskCursor %= Global::disk_num;
            if (RWAreaDiskCursor == obj_tag_in_disk) {
              RWAreaDiskCursor++;
              RWAreaDiskCursor %= Global::disk_num;
            }
            // cerr << start_pos << "\n";
        }
        // for (RWDisk = 0; RWDisk < Global::disk_num && start_pos == -1; RWDisk++) {
        //     start_pos  = object_writers[RWDisk].write_and_get_start_position(
        //         Object::object_map[task.get_obj_id_in_task()].get_size());
        // }
        // RWDisk--;    
        if (start_pos == -1) throw logic_error("cannot find enough blocks");
        // cerr << "!1\n";
        // cerr << RWDisk << '\n';
        disk_stored_obj_id_set[RWDisk].insert(task.get_obj_id_in_task());
        // cerr << "!2\n";
        obj_disk_position[0][task.get_obj_id_in_task()] = RWDisk;
        for (int i = 0; i < Object::object_map[task.get_obj_id_in_task()].get_size(); i++) {
            const int now_pos = (start_pos + i) % Global::get_rw_area_size();
            obj_block_position[0][task.get_obj_id_in_task()].emplace_back(now_pos);
            Disk::disks[RWDisk].store(now_pos, task.get_obj_id_in_task(), i);
            Object::object_map[task.get_obj_id_in_task()].blockPosition.emplace_back(BlockPosition{RWDisk, now_pos});
        }
        obj_cnt[task.get_obj_id_in_task()] = 1;

        // handle backup
        priority_queue<heap_node> checkA;
        for(int i = 0; i < Global::disk_num; i++) {
            if (i == RWDisk) continue;
            heap_node num;
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
    // cerr << "in for" <<"\n";
    for (int i = 0; i < Global::disk_num; i++) {
        // cerr << "for1\n";
        // cerr << "remain size"<<diskRWRemainMap[i] << "\n";
        vector<write_result> results = BackupAreaWriters[i].getWriteResultsInBackupArea();
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

void ObjectTagWriteRegulator::update_deleted(const vector<int>& deleted_obj_ids) {
    for (auto obj_id: deleted_obj_ids) {
        Object obj = Object::object_map[obj_id];
//        cerr << "del obj size:" << obj.get_size() << '\n';
        bool rw_handled = false;
        for (int i = 0; i < obj.blockPosition.size(); i++) {
            if (obj.blockPosition[i].blockId >= Disk::disks[obj.blockPosition[i].diskId].getRWAreaSize()) {
                diskBackupRemainMap[obj.blockPosition[i].diskId] += 1;
//                cerr << "release backup:" << obj.blockPosition[i].diskId << '\n';
            } else if (!rw_handled) {
//                cerr << "release RW: " << obj.blockPosition[i].diskId << " " << obj.get_size() << " " << obj.blockPosition[i].blockId << "\n";
                object_writers[obj.blockPosition[i].diskId].update_delete(
                    obj.blockPosition[i].blockId,
                    obj.get_size()
                    );
                rw_handled = true;
            }
            if (disk_stored_obj_id_set[obj.blockPosition[i].diskId].count(obj_id) > 0) 
                disk_stored_obj_id_set[obj.blockPosition[i].diskId].erase(obj_id);
        }

    }
}
