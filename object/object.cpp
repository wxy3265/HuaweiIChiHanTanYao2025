#include "object.h"

map<int, Object> Object::object_map = map<int, Object>();

void Object::kill() {
    alive = false;
}

void Task::complete(int block_id) {
    done[block_id] = true;
}

bool Task::is_done() const {
    for (int i = 0; i < obj.get_size(); i++) {
        if (!done[i]) {
            return false;
        }
    }
    return true;
}

bool Task::block_done(int block_id) const {
    return done[block_id];
}
