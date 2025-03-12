#ifndef DISK_H
#define DISK_H
#include "object.h"

class Disk {
public:
    Disk() : id(-1), size(-1) {};
    Disk(int id, int size) : id(id), size(size){};

    static Disk disks[Global::MAX_DISK_NUM];

    int get_id() const { return id; }
    int get_size() const { return size; }
    int get_head() const { return head; }

    void store(int block_id, int obj_id, int obj_block_id);
    void delete_(int block_id);
    bool is_empty(int block_id);
    void move_head(int step);
    void jump_head(int pos);
    
private:
    int id;
    int size;
    int head{};
    ObjectBlock stored_object[Global::MAX_DISK_SIZE];

};

#endif