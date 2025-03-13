#include "reader.h"

class ReadRegulator {
public:
    virtual vector<int> handle_delete();
    virtual void handle_read() = 0;

protected:
    queue<Task> requests;
    map<int, vector<Task>> objid_tasks_map;
    
    void get_request_from_interaction();

};

class MostGreedReadRegulator: public ReadRegulator {
public:
    MostGreedReadRegulator();
    void handle_read() override;
private:
    ReadOnlyGreedReader readers[Global::MAX_DISK_NUM];
};
