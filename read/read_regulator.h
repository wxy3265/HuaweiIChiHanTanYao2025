#include "reader.h"

class ReadRegulator {
private:
    queue<Task> requests;
    map<int, Task> objid_task_map;
    Reader readers[Global::MAX_DISK_NUM];
    
    void get_request_from_interaction();
    void init();
    void create_readers();
    void handle_delete();

};
