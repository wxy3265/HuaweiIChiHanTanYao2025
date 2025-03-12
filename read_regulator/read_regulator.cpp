#include "read_regulator.h"

void ReadRegulator::get_request_from_interaction() {
    int request_num;
    cin >> request_num;
    for (int i = 0 ; i < request_num; i++) {
        int task_id, obj_id;
        Task task = Task(Global::now_frame, Object::object_map[obj_id], task_id);
        objid_task_map[obj_id] = task;
    }
}

void ReadRegulator::init() {
    create_readers();
}

void ReadRegulator::create_readers() {
    for (int i = 0; i < Global::disk_num; i++) {
        readers[i] = Reader(i);
    }
}

/**
 * @return deleted obj_id
 */
vector<int> ReadRegulator::handle_delete() {
    vector<int> deleted_obj_ids;
    vector<int> canceled_task;
    int request_nums;
    cin >> request_nums;
    for (int i = 0; i < request_nums; i++) {
        int obj_id;
        cin >> obj_id;
        deleted_obj_ids.emplace_back(obj_id);
        Object::object_map[obj_id].kill();
        if (objid_task_map.count(obj_id) == 0) continue;
        canceled_task.emplace_back(objid_task_map[obj_id].get_task_id());
        objid_task_map.erase(obj_id);
    }
    cout << canceled_task.size() << '\n';
    for (auto task_id: canceled_task) {
        cout << task_id << '\n';
    }
    cout.flush();
    return deleted_obj_ids;
}

