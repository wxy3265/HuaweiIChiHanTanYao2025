#include "writer.h"

ObjectWriter::ObjectWriter(int disk_id) {
    space_list_head = new LinkedListNode<SpaceNode>(SpaceNode{-1, -1}, nullptr, nullptr);
    space_list_head->insert_back(SpaceNode{Disk::disks[disk_id].getRWAreaSize(), 0});
    space_head_map[Global::get_rw_area_size()] = new LinkedListNode<int>(0, nullptr, nullptr);
    this->disk_id = disk_id;
}

void ObjectWriter::insert_to_map(int size, int start_pos) {
    if (space_head_map.count(size) > 0) {
        space_head_map[size]->insert_back(start_pos);
    } else {
        space_head_map[size] = new LinkedListNode<int>(start_pos, nullptr, nullptr);
    }
}

int ObjectWriter::write_and_get_start_position(int size) {
    // find position
    for (auto it = space_head_map.begin(); it != space_head_map.end(); ++it) {
        const int space = it->first;
        cerr << "space:" << space << '\n';
        LinkedListNode<int> *space_head_node = it->second;
        cerr << "disk position: " << space_head_node->get_data() << "  \n";
        if (space >= size) {
            const int start_pos = space_head_node->get_data();
            if (space_head_node->get_nxt() == nullptr) {
                space_head_map.erase(space);
            }
//            if (size == 3 && (start_pos + size) % Global::get_rw_area_size() == 1) {
//                cerr << "Maybe in there:" << size << " " << start_pos << " ";
//                cerr << Global::get_rw_area_size() << "\n";
//            }
            insert_to_map(space - size, (start_pos + size) % Global::get_rw_area_size());
            space_head_node->remove_this();
            LinkedListNode<SpaceNode> *space_list_node = space_list_head;
            while (space_list_node->get_data().start_pos != start_pos) {
                space_list_node = space_list_node->get_nxt();
            }
            SpaceNode space_node = space_list_node->get_data();
            space_list_node->insert_back(
                SpaceNode {
                    space_node.space - size,
                    (space_node.start_pos + size) % Disk::disks[disk_id].getRWAreaSize()
                }
            );
            space_list_node->remove_this();
            return start_pos;
        }
    }
    return -1;
}

void ObjectWriter::update_delete(int start_pos, int size) {
    LinkedListNode<SpaceNode> *space_list_node = space_list_head;
    bool flag = false;
    while (space_list_node->get_data().start_pos < start_pos) {
        if (space_list_node->get_nxt() == nullptr) {
            flag = true;
            break;
        }
        space_list_node = space_list_node->get_nxt();
        // flag = true;
    }
    if (flag == false) {
        space_list_node = space_list_node->get_pre();
    }
    if (size + start_pos > Disk::disks[disk_id].getRWAreaSize()) {
        int otherSize = size + start_pos - Disk::disks[disk_id].getRWAreaSize();
        size -= otherSize;
        space_list_head->insert_back(
            SpaceNode {
                otherSize,
                0
            }
        );
        insert_to_map(otherSize, 0);
        LinkedListNode<SpaceNode> *space_list_at_zero = space_list_head->get_nxt();
        space_list_at_zero = space_list_at_zero->get_nxt();
        while ((space_list_at_zero->get_pre()->get_data().space + 
                space_list_at_zero->get_pre()->get_data().start_pos) == 
                space_list_at_zero->get_data().start_pos) {
                int insetNum = 0;
                    LinkedListNode<int> *need_delete_node = space_head_map[space_list_at_zero->get_data().space];
                    while (need_delete_node->get_data() != space_list_at_zero->get_data().start_pos) {
                        need_delete_node->get_nxt();
                    }
                    need_delete_node->remove_this();
                    need_delete_node = space_head_map[space_list_at_zero->get_pre()->get_data().space];
                    while (need_delete_node->get_data() != space_list_at_zero->get_pre()->get_data().start_pos) {
                        need_delete_node->get_nxt();
                    }
                    need_delete_node->remove_this();
                    space_list_at_zero->get_pre()->setData(
                        SpaceNode {
                            space_list_at_zero->get_data().space + space_list_at_zero->get_pre()->get_data().space,
                            space_list_at_zero->get_pre()->get_data().start_pos
                        }
                    );
                    insert_to_map(space_list_at_zero->get_data().space + 
                                  space_list_at_zero->get_pre()->get_data().space, 
                                  space_list_at_zero->get_pre()->get_data().start_pos);
                    space_list_at_zero = space_list_at_zero->remove_this();
                }
    }
    space_list_node->insert_back(
        SpaceNode {
            size,
            start_pos
        }
    );
    insert_to_map(size, start_pos);
    space_list_node = space_list_node->get_nxt();
    while ((space_list_node->get_pre()->get_data().space + 
            space_list_node->get_pre()->get_data().start_pos) == 
            space_list_node->get_data().start_pos) {
                LinkedListNode<int> *need_delete_node = space_head_map[space_list_node->get_data().space];
                    while (need_delete_node->get_data() != space_list_node->get_data().start_pos) {
                        need_delete_node->get_nxt();
                    }
                    need_delete_node->remove_this();
                    need_delete_node = space_head_map[space_list_node->get_pre()->get_data().space];
                    while (need_delete_node->get_data() != space_list_node->get_pre()->get_data().start_pos) {
                        need_delete_node->get_nxt();
                    }
                    need_delete_node->remove_this();
                space_list_node->get_pre()->setData(
                    SpaceNode {
                        space_list_node->get_data().space + space_list_node->get_pre()->get_data().space,
                        space_list_node->get_pre()->get_data().start_pos
                    }
                );

                insert_to_map(space_list_node->get_data().space + 
                            space_list_node->get_pre()->get_data().space, 
                            space_list_node->get_pre()->get_data().start_pos);
                space_list_node = space_list_node->remove_this();
            }   
}
