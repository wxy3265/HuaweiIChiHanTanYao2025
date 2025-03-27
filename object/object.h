#ifndef OBJECT_H
#define OBJECT_H
#include "global.h"
struct BlockPosition {
    int diskId;
    int blockId;
};
// 被存储的对象
class Object {
public:
    Object() : id(-1), size(-1), tag(-1) {};
    Object(int id, int size, int tag) : id(id), size(size), tag(tag) {}
    static map<int, Object> object_map; // object_id -> object对象实例
    int get_size() const { return size; }
    int get_tag() const { return tag; }
    int get_id() const { return id; }
    void kill();
    vector<BlockPosition> blockPosition;
    // vector<ObjectBlock> Myblocks;
private:
    int id; // 对象id，-1表示空对象
    int size; // 对象大小
    int tag; // 对象标签
    bool alive = true; // 对象是否存活（没被删除）
};

// 对象块
class ObjectBlock {
public:
    ObjectBlock() : obj_id(-1), obj_block_id(-1) {};
    ObjectBlock(int object_id, int block_id) : obj_id(object_id), obj_block_id(block_id) {};
    int get_obj_id() const { return obj_id; }
    int get_obj_block_id() const { return obj_block_id; }
    bool is_null() const { return obj_id == -1; } // 是否是空对象块
private:
    int obj_id = -1; // 是哪个对象的块
    int obj_block_id = -1; // 是对象的哪个块
};

// 任务（主要是读任务，与对象是一对一关系，但对象对任务可能是一对多）
class Task {
public:
    Task() : create_frame(-1), obj_id_in_task(-1), id(-1) {};
    Task(int create_frame, int obj_id) : create_frame(create_frame), obj_id_in_task(obj_id), id(-1) {
        // obj_id_in_task = obj.get_id();
    };
    Task(int create_frame, int obj_id, int id) : create_frame(create_frame), obj_id_in_task(obj_id), id(id) {
        // obj_id_in_task = obj.get_id();
    };
    int get_task_id() const { return id; }
    int get_create_frame() const { return create_frame; }
    // Object get_obj() { return obj; }
    int get_obj_id_in_task() {return obj_id_in_task;}
    void complete(int block_id); // 将对象的某个块标记为完成（已读）
    bool is_done() const; // 对象的所有块是否读完
    bool block_done(int block_id) const; // 对象的某一块是否读完

private:
    int create_frame; // 任务创建时的帧号（用于计算读操作收益）
    int id = -1;
    // Object obj; // 任务对应的对象 TODO:这里不应该存对象实例，应该改成对象id，交给你了
    int obj_id_in_task;
    bool done[Global::MAX_OBJ_SIZE] = {false}; // 对象的每个块是否读完
};
#endif