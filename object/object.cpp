#include "object.h"

map<int, Object> Object::object_map = map<int, Object>();

void Object::kill() {
    alive = false;
}
