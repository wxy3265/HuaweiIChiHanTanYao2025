#ifndef DISK_H
#define DISK_H
#include "object.hpp"

class Disk {
public:
    Disk() : id(-1), size(-1) {};
    Disk(int id, int size) : id(id), size(size){};

    int get_id() const { return id; }
    int get_size() const { return size; }
    int get_head() const { return head; }

    void store(int block_id, int obj_id) {
        stored_object[block_id] = ObjectBlock(obj_id, block_id);
    }

    void move_head(int step) {
        head += step;
        head %= size;
    }

    void jump_head(int pos) {
        head = pos;
    }
    
private:
    int id;
    int size;
    int head{};
    ObjectBlock stored_object[Global::MAX_DISK_SIZE];

} disk[Global::MAX_DISK_NUM];

#endif