#include "global.hpp"

class Object {
public:
    Object() : id(-1), size(-1), tag(-1) {};
    Object(int id, int size, int tag) : id(id), size(size), tag(tag) {}
    static map<int, Object> object_map;
    int get_id() const { return id; }
    int get_size() const { return size; }
    int get_tag() const { return tag; }
private:
    int id;
    int size;
    int tag;
};

class ObjectBlock {
public:
    ObjectBlock() : object_id(-1), block_id(-1) {};
    ObjectBlock(int object_id, int block_id) : object_id(object_id), block_id(block_id) {};
    int get_object_id() { return object_id; }
    int get_block_id() { return block_id; }
private:
    int object_id;
    int block_id; 
};

class Task {
public:
    Task() : create_frame(-1), obj(Object()) {};
    Task(int create_frame, Object obj) : create_frame(create_frame), obj(obj), id(-1) {};
    Task(int create_frame, Object obj, int id) : create_frame(create_frame), obj(obj), id(id) {};
    int get_create_frame() { return create_frame; }
    Object get_obj() { return obj; }
    void cancel() {}
private:
    int create_frame;
    int id;
    Object obj;
};
