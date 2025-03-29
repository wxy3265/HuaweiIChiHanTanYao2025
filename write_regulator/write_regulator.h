#include "writer.h"

// 跨盘写策略调节器
class WriteRegulator {
public:
    virtual void handle_write() = 0; // 处理写请求
    virtual void handleWriteWithTwoArea() = 0;
protected:
    queue<Task> requests; // 写请求队列（目前仅作为cin缓存使用）

    void get_request_from_interaction(); // 从判题器cin读取写请求
};

struct heap_node {
    int disk_id, diskNum;
    bool operator < (const heap_node &b) const {
        return diskNum < b.diskNum;
    }
};


// 纯轮着写
class MostGreedWriteRegulator: public WriteRegulator{
public:
    MostGreedWriteRegulator();
    virtual void handle_write() override;
    void handleWriteWithTwoArea() override;
    virtual void update_deleted(const vector<int>& vector1); // 策略强相关：由于删除操作需要更新磁盘剩余空间，所以需要提供删除的对象id
    void update_deleted_with_two_area(const vector<int>& vector1);

protected:
    map<int, int> disk_remain_map; // disk id -> remain blocks 每个磁盘剩余空间
    map<int, int> diskRWRemainMap;
    map<int, int> diskBackupRemainMap;
    set<int> disk_stored_obj_id_set[Global::MAX_DISK_NUM]; // 每个磁盘存储了哪些对象
    int disk_cursor = 0; // 轮到哪个磁盘了
    int RWAreaDiskCursor = 0;
    int BackupDiskCursor = 0;
    BruteWriter brute_writers[Global::MAX_DISK_NUM]; // 每个磁盘的writer
    BruteWriter RWAreaWriters[Global::MAX_DISK_NUM];
    BruteWriter BackupAreaWriters[Global::MAX_DISK_NUM];
};

class ObjectUnitWriteRegulator: public MostGreedWriteRegulator {
public:
    ObjectUnitWriteRegulator();
    void handle_write();
    void update_deleted(const vector<int>& vector1);
protected:
    ObjectWriter object_writers[Global::MAX_DISK_NUM];
};

// 三副本RAID
class TripleRaidWriteRegulator: public MostGreedWriteRegulator{
public:
    TripleRaidWriteRegulator();
    void handle_write() override;
};
