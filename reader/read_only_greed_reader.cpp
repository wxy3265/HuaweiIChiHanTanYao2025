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
    cerr <<"DDDiskID:" << disk_id << " " << pre_head << "max:" << Disk::disks[disk_id].getMaxBlocks() << " " << Global::get_rw_area_size() << "\n";
    // int tmp_head = (pre_head + 1) % Global::get_rw_area_size(), tmp_pre_tokens = max(16.0, ceil(pre_tokens * 0.8)), tmp_remain_tokens = remain_tokens - 64;
    // int max_read_times = 1, latest_read_task_block = -1, cnt_times = 1, latest_times = -1;
    // while (tmp_head != pre_head && latest_read_task_block != -1 || tmp_remain_tokens > tmp_pre_tokens) {
    //     cnt_times++;
    //     if (tmp_remain_tokens > tmp_pre_tokens) max_read_times++;
    //     cerr << "tokens:" << tmp_remain_tokens << " " << tmp_pre_tokens << "head:" << tmp_head << '\n';
    //     tmp_remain_tokens -= tmp_pre_tokens;
    //     tmp_pre_tokens = max(16.0, ceil(tmp_pre_tokens * 0.8));
    //     if (latest_read_task_block == -1 && 
    //         objid_tasks_map[Disk::disks[disk_id].pick_block(tmp_head).get_obj_id()].size() != 0){
    //             latest_read_task_block = tmp_head;
    //             latest_times = cnt_times;
    //     }
    //     tmp_head = (tmp_head + 1) % Global::get_rw_area_size();
    // }
    // cerr << "check jump:" << latest_times << ' ' << max_read_times << " " << latest_read_task_block << '\n';
    // if (latest_times > max_read_times * 10 && latest_read_task_block != -1) {
    //     cerr << disk_id << "jump to" << latest_read_task_block << '\n';
    //     cout << "j " << latest_read_task_block + 1 << '\n';
    //     Disk::disks[disk_id].jump_head(latest_read_task_block);
    //     cout.flush();
    //     Disk::disks[disk_id].setJumpFlag(false);
    //     return vector<int>();
    // }
    int next_read_task_block = -1;
    vector<int> completed_task_ids;
    int flag = 0;
    if (Disk::disks[disk_id].getJumpFlag() == true) {
        cout << "j 1\n";
        Disk::disks[disk_id].jump_head(0);
        cout.flush();
        Disk::disks[disk_id].setJumpFlag(false);
        return vector<int>();
    }
    while (remain_tokens > pre_tokens) {
        remain_tokens -= pre_tokens;
        pre_tokens = max(16.0, ceil(pre_tokens * 0.8));
        // cerr << "real:" << remain_tokens << " " << pre_tokens << '\n';
        if (pre_head >= Disk::disks[disk_id].getMaxBlocks()) {
            Disk::disks[disk_id].setJumpFlag(true);
            // if (Disk::disks[disk_id].getMaxBlocks() != Disk::disks[disk_id].getRWAreaSize()) break;
        }
        Disk::disks[disk_id].move_head(1);
        cout << "r";
        ObjectBlock block = Disk::disks[disk_id].pick_block(pre_head);
        if (!block.is_null()) {
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
