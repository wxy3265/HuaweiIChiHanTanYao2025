#include "writer.h"

void BruteWriter::emplace_task(Task task) {
    task_queue.emplace(task);
}

vector<write_result> BruteWriter::get_write_results() {
    vector<write_result> res;
    // int block_cursor = 0;
    while (!task_queue.empty()) {
        Task task = task_queue.front(); task_queue.pop();
        write_result result;
        result.obj_id = task.get_obj_id_in_task();
        for (int now_obj_block_id = 0; now_obj_block_id < Object::object_map[task.get_obj_id_in_task()].get_size(); now_obj_block_id++) {
            while (!Disk::disks[disk_id].is_empty(Disk::disks[disk_id].get_block_cursor())) {
                Disk::disks[disk_id].move_block_cursor((Disk::disks[disk_id].get_block_cursor() + 1) % Global::disk_size);
                // block_cursor++;
            }
                
            // block_cursor %= Global::disk_size;
            Disk::disks[disk_id].store(Disk::disks[disk_id].get_block_cursor(), task.get_obj_id_in_task(), now_obj_block_id);
            
            BlockPosition newPosition;
            newPosition.blockId = Disk::disks[disk_id].get_block_cursor();
            newPosition.diskId = disk_id;

            Object::object_map[task.get_obj_id_in_task()].blockPosition.push_back(newPosition);
            
            result.stored_block_ids.emplace_back(Disk::disks[disk_id].get_block_cursor());
            Disk::disks[disk_id].move_block_cursor((Disk::disks[disk_id].get_block_cursor() + 1) % Global::disk_size);
            // block_cursor %= Global::disk_size;
        }
        res.emplace_back(result);
    }
    return res;
}

vector<write_result> BruteWriter::getWriteResultsInRWArea() {
    cerr << "in rw area\n";
    vector<write_result> res;
    while (!task_queue.empty()) {
        Task task = task_queue.front(); task_queue.pop();
        write_result result;
        result.obj_id = task.get_obj_id_in_task();
        for (int now_obj_block_id = 0; now_obj_block_id < Object::object_map[task.get_obj_id_in_task()].get_size(); now_obj_block_id++) {
            while (!Disk::disks[disk_id].is_empty(Disk::disks[disk_id].getRWAreaBlockCursor())) {
                cerr << "NOT EMPTY" << Disk::disks[disk_id].getRWAreaBlockCursor()
                     << " already:" << Disk::disks[disk_id].pick_block(Disk::disks[disk_id].getRWAreaBlockCursor()).get_obj_id()
                     << "." << Disk::disks[disk_id].pick_block(Disk::disks[disk_id].getRWAreaBlockCursor()).get_obj_block_id()
                     << "when write:" << task.get_obj_id_in_task()
                     << "frame:" << Global::now_frame
                     << "\n";
                Disk::disks[disk_id].moveRWAreaBlockCursor(
                    (Disk::disks[disk_id].getRWAreaBlockCursor() + 1) % Disk::disks[disk_id].getRWAreaSize()
                );
                // block_cursor++;
            }
            // block_cursor %= Global::disk_size;
            Disk::disks[disk_id].store(
                Disk::disks[disk_id].getRWAreaBlockCursor(), task.get_obj_id_in_task(), now_obj_block_id
            );
            
            BlockPosition newPosition;
            newPosition.blockId = Disk::disks[disk_id].getRWAreaBlockCursor();
            newPosition.diskId = disk_id;

            Object::object_map[task.get_obj_id_in_task()].blockPosition.push_back(newPosition);
            
            result.stored_block_ids.emplace_back(Disk::disks[disk_id].getRWAreaBlockCursor());
            Disk::disks[disk_id].moveRWAreaBlockCursor(
                (Disk::disks[disk_id].getRWAreaBlockCursor() + 1) % Disk::disks[disk_id].getRWAreaSize()
            );
            // block_cursor %= Global::disk_size;
        }
        res.emplace_back(result);
    }
    cerr << "Is Finish RW Area\n";
    return res;
}

vector<write_result> BruteWriter::getWriteResultsInBackupArea() {
    // cerr << "is in backup area\n";
    vector<write_result> res;
    // int block_cursor = 0;
    int tims = 0;
    while (!task_queue.empty()) {
        Task task = task_queue.front(); task_queue.pop();
        write_result result;
        result.obj_id = task.get_obj_id_in_task();
        // cerr << tims++ << "\n";
        for (int now_obj_block_id = 0; now_obj_block_id < Object::object_map[task.get_obj_id_in_task()].get_size(); now_obj_block_id++) {
            while (!Disk::disks[disk_id].is_empty(Disk::disks[disk_id].getBackupBlockCursor())) {
                int positionCursor = Disk::disks[disk_id].getBackupBlockCursor() + 1;
                if (positionCursor == Global::disk_size) positionCursor = Disk::disks[disk_id].getRWAreaSize();

                Disk::disks[disk_id].moveBackupBlockCursor(positionCursor);
                // block_cursor++;
            }
                
            // block_cursor %= Global::disk_size;
            Disk::disks[disk_id].store(
                Disk::disks[disk_id].getBackupBlockCursor(), task.get_obj_id_in_task(), now_obj_block_id
            );
            
            BlockPosition newPosition;
            newPosition.blockId = Disk::disks[disk_id].getBackupBlockCursor();
            newPosition.diskId = disk_id;

            Object::object_map[task.get_obj_id_in_task()].blockPosition.push_back(newPosition);
            
            result.stored_block_ids.emplace_back(Disk::disks[disk_id].getBackupBlockCursor());

            int positionCursor = Disk::disks[disk_id].getBackupBlockCursor();
            if (positionCursor + 1 == Global::disk_size) positionCursor = Disk::disks[disk_id].getRWAreaSize();

            Disk::disks[disk_id].moveBackupBlockCursor(positionCursor);
            // Disk::disks[disk_id].move_block_cursor((Disk::disks[disk_id].get_block_cursor() + 1) % Global::disk_size);
            // block_cursor %= Global::disk_size;
        }
        res.emplace_back(result);
    }
    // cerr << "is Finish Backup Area\n";
    return res;
}