#include "read_regulator.h"

MostGreedReadRegulator::MostGreedReadRegulator() {
    for (int i = 0; i < Global::disk_num; i++) {
        readers[i] = ReadOnlyGreedReader(i);
    }
}

void MostGreedReadRegulator::handle_read() {
    get_request_from_interaction();
    vector<int> completed_task_ids;
    int flag = 0;
    for (int i = 0; i < Global::disk_num; i++) {
        vector<int> ids = readers[i].read(&objid_tasks_map);
        for (auto id : ids) {
            completed_task_ids.emplace_back(id);
            // if (completed_task_ids.size() >= 400) {
                // flag = 1;
                // break;
            // }
        }
        // if (flag) break;
    }
    cout << completed_task_ids.size() << '\n';
    // cerr << completed_task_ids.size() << '\n';
    for (int completed_task_id : completed_task_ids) {
        // cerr << "complete read" << completed_task_id << '\n';
        cout << completed_task_id << '\n';
    }
    cout.flush();
}
