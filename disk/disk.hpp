#include "object.hpp"

class Disk {
public:
    Disk(int id, int size) : id(id), size(size){};

    int get_id() const { return id; }
    int get_size() const { return size; }
    int get_head() const { return head; }

    void store(int id, int obj_id) {
        stored_object[id] = ObjectBlock(obj_id, id);
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
    int head;
    ObjectBlock stored_object[Global::MAX_DISK_SIZE];

} disk[Global::MAX_DISK_NUM];
