#include "disk.h"

Disk Disk::disks[Global::MAX_DISK_NUM];

void Disk::store(int block_id, int obj_id, int obj_block_id) {
    stored_object[block_id] = ObjectBlock(obj_id, obj_block_id);
}

void Disk::delete_(int block_id) {
    stored_object[block_id] = ObjectBlock();
}

bool Disk::is_empty(int block_id) {
    return stored_object[block_id].is_null();
}

void Disk::move_head(int step) {
    head += step;
    head %= size;
}

void Disk::jump_head(int pos) {
    head = pos;
}
