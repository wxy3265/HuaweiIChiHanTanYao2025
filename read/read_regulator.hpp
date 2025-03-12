#include "reader.hpp"

class ReadRegulator {
private:
    queue<Task> requests;
    map<int, Task> objid_task_map;
    Reader readers[Global::MAX_DISK_NUM];
    
    void get_request_from_interaction() {
        int request_num;
        cin >> request_num;
        for (int i = 0 ; i < request_num; i++) {
            int task_id, obj_id;
            Task task = Task(Global::now_frame, Object::object_map[obj_id], task_id);
            objid_task_map[obj_id] = task;
        }
    }

    void init() {
        create_readers();
    }

    void create_readers() {
        for (int i = 0; i < Global::disk_num; i++) {
            readers[i] = Reader(i);
        }
    }
};
