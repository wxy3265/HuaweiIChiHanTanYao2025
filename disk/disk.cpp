#include "disk.h"

Disk Disk::disks[Global::MAX_DISK_NUM];

void Disk::init() {
    for (int i = 0; i < Global::disk_num; i++) {
        disks[i] = Disk(i, Global::disk_size);
        disks[i].setRWAreaSize(Global::disk_size);
    }
}

void Disk::store(int block_id, int obj_id, int obj_block_id) {
    stored_object[block_id] = ObjectBlock(obj_id, obj_block_id);
}

void Disk::delete_block(int block_id) {
    stored_object[block_id] = ObjectBlock();
}

void Disk::delete_objs(set<int> obj_ids) {
    // for (auto it : obj_ids) {
    //     int blockSize = Object::object_map[it].blockPosition.size();
    //     for (int i = 0; i < blockSize; i++) {
    //         BlockPosition Oneblock = Object::object_map[it].blockPosition[i];
    //         stored_object[Oneblock.]
    //     }

    // }
    // ;
    for (int i = 0; i < Global::disk_size; i++) {
        if (obj_ids.count(stored_object[i].get_obj_id())) {
            stored_object[i] = ObjectBlock();
        }
    }
}

bool Disk::is_empty(int block_id) {
    if (block_id > Global::disk_size) {
        cerr << "ERROR: disk block id out of range at is_empty(" << block_id << ")\n";
    }
    return stored_object[block_id].is_null();
}

void Disk::move_head(int step) {
    head += step;
    head %= size;
}

void Disk::jump_head(int pos) {
    head = pos;
}

void Disk::move_block_cursor(int pos) {
    block_cursor = pos;
}

void Disk::moveBackupBlockCursor(int pos) {
    blockCursorInBackupArea = pos;
}

void Disk::moveRWAreaBlockCursor(int pos) {
    blockCursorInRWArea = pos;
}

void Disk::setRWAreaSize(int thisSize) {
    RWAreaSize = thisSize / 3 + 1;
    moveBackupBlockCursor(RWAreaSize);
}

void Disk::setJumpFlag() {
    if (jumpFlag == true) jumpFlag = false;
    else jumpFlag = true;
}

void Disk::setMaxBlocks() {
    maxBlocks += 1;
    maxBlocks = min(maxBlocks, RWAreaSize);
}