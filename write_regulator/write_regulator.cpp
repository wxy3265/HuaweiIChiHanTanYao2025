#include "write_regulator.h"

void WriteRegulator::get_request_from_interaction() {
    int request_num;
    cin >> request_num;
    for (int i = 0 ; i < request_num; i++) {
        int id, size, tag;
        Object obj = Object(id, size, tag);
        Object::object_map[id] = obj;
        Task task = Task(Global::now_frame, obj);
        requests.push(task);
    }
}
