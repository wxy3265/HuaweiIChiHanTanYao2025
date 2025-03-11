#include "object.hpp"

class Disk {
public:
    Disk(int id, int size) : id(id), size(size){};
    int get_id() const { return id; }
    int get_size() const { return size; }
    int get_head() const { return head; }
    
private:
    int id;
    int size;
    int head;
    ObjectBlock stored_object[Global::MAX_DISK_SIZE];
} disk[Global::MAX_DISK_NUM];
