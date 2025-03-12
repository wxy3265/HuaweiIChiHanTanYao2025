#ifndef OBJECT_H
#define OBJECT_H
#include "global.h"

class Object {
public:
    Object() : id(-1), size(-1), tag(-1) {};
    Object(int id, int size, int tag) : id(id), size(size), tag(tag) {}
    static map<int, Object> object_map;
    int get_size() const { return size; }
    int get_tag() const { return tag; }
    int get_id() const { return id; }
    void kill();
private:
    int id;
    int size;
    int tag;
    int alive = true;
};

class ObjectBlock {
public:
    ObjectBlock() : obj_id(-1), obj_block_id(-1) {};
    ObjectBlock(int object_id, int block_id) : obj_id(object_id), obj_block_id(block_id) {};
    int get_obj_id() const { return obj_id; }
    int get_obj_block_id() const { return obj_block_id; }
    bool is_null() const { return obj_id == -1; }
private:
    int obj_id;
    int obj_block_id;
};

class Task {
public:
    Task() : create_frame(-1), obj(Object()), id(-1) {};
    Task(int create_frame, Object obj) : create_frame(create_frame), obj(obj), id(-1) {};
    Task(int create_frame, Object obj, int id) : create_frame(create_frame), obj(obj), id(id) {};
    int get_task_id() const { return id; }
    int get_create_frame() const { return create_frame; }
    Object get_obj() { return obj; }

private:
    int create_frame;
    int id;
    Object obj;
};
#endif