#include "disk.h"

Disk Disk::disks[Global::MAX_DISK_NUM];

void Disk::init() {
    for (int i = 0; i < Global::disk_num; i++) {
        disks[i] = Disk(i, Global::disk_size);
    }
}

void Disk::store(int block_id, int obj_id, int obj_block_id) {
    stored_object[block_id] = ObjectBlock(obj_id, obj_block_id);
}

void Disk::delete_block(int block_id) {
    stored_object[block_id] = ObjectBlock();
}

void Disk::delete_objs(set<int> obj_ids) {
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
