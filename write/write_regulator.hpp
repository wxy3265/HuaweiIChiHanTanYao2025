#include "writer.hpp"

class WriteRegulator {
public:
private:
    queue<Task> requests;
    Writer writers[Global::MAX_DISK_NUM];
    
    void get_request_from_interaction();
    virtual void init();
    void create_writers();
};
