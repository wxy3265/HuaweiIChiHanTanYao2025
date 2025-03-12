#include "writer.h"

class WriteRegulator {
public:
    virtual void handle_write() = 0;

protected:
    queue<Task> requests;

    void get_request_from_interaction();
};

class MostGreedWriteRegulator: public WriteRegulator{
public:
    MostGreedWriteRegulator();
    void handle_write() override;
    void update_deleted(const vector<int>& vector1);

private:
    map<int, int> disk_remain_map; // disk id -> remain blocks
    set<int> disk_stored_obj_id_set[Global::MAX_DISK_NUM];
    int disk_cursor = 0;
    BruteWriter writers[Global::MAX_DISK_NUM];

};
