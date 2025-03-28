#ifndef DISK_H
#define DISK_H
#include "object.h"

// 磁盘
class Disk {
public:
    Disk() : id(-1), size(-1) {};
    Disk(int id, int size) : id(id), size(size){};

    static Disk disks[Global::MAX_DISK_NUM]; // 所有磁盘的实例
    static void init();
    

    int get_id() const { return id; }
    int get_size() const { return size; }
    int get_head() const { return head; }
    int get_block_cursor() {
        return block_cursor;
    }
    int getRWAreaBlockCursor() {
        return blockCursorInRWArea;
    }
    int getBackupBlockCursor() {
        return blockCursorInBackupArea;
    }
    int getRWAreaSize() {
        return RWAreaSize;
    }
    bool getJumpFlag() {
        return jumpFlag;
    }
    void store(int block_id, int obj_id, int obj_block_id); // 将对象的某一块存储到磁盘的某个位置
    void delete_block(int block_id); // 删除磁盘上的某个块里的数据
    void delete_objs(set<int> obj_ids); // 删除磁盘上的某些对象
    bool is_empty(int block_id); // 判断磁盘上的某个块是否为空
    void move_head(int step); // 移动磁头
    void jump_head(int pos); // 跳转磁头到某个位置
    void move_block_cursor(int pos);
    void moveRWAreaBlockCursor(int pos);
    void moveBackupBlcckCursor(int pos);
    void setJumpFlag();
    void setRWAreaSize(int thisSize);

    ObjectBlock pick_block(int block_id) { return stored_object[block_id]; } // 获取磁盘上某个块的存储内容
    
private:
    bool jumpFlag = false;
    int id; // 磁盘的编号
    int size; // 磁盘的大小
    int head = 0; // 磁头的位置
    int block_cursor = 0;
    int blockCursorInRWArea = 0;
    int blockCursorInBackupArea = 0;
    int RWAreaSize = 0;
    ObjectBlock stored_object[Global::MAX_DISK_SIZE]; // 每个位置存储的内容
    
};

#endif