#include "write_regulator.hpp"

void WriteRegulator::get_request_from_interaction() {
    int request_num;
    cin >> request_num;
    for (int i = 0 ; i < request_num; i++) {
        int id, size, tag;
        Object obj = Object(id, size, tag);
        Object::object_map[id] = obj;
        Task task = Task(Global::now_frame, obj);
    }
}

void WriteRegulator::create_writers() {
    for (int i = 0; i < Global::disk_num; i++) {
        writers[i] = Writer(i);
    }
}

void WriteRegulator::init() {
    create_writers();
}
