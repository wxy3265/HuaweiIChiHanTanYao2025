#include "write_regulator.h"

void WriteRegulator::get_request_from_interaction() {
    int request_num = read_Fast();
    // cin >> request_num;
    for (int i = 0 ; i < request_num; i++) {
        int id = read_Fast(), size = read_Fast(), tag = read_Fast();
        // cin >> id >> size >> tag;
        Object obj = Object(id, size, tag);
        Object::object_map[id] = obj;
        Task task = Task(Global::now_frame, obj.get_id());
        requests.push(task);
    }
}
