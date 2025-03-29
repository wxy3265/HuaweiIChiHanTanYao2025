#include "writer.h"

ObjectWriter::ObjectWriter(int disk_id) {
    space_list_head = new LinkedListNode<SpaceNode>(SpaceNode{-1, -1}, nullptr, nullptr);
    space_list_head->insert_back(SpaceNode{Disk::disks[disk_id].getRWAreaSize(), 0});
}

int ObjectWriter::write_and_get_start_position(int size) {
    // find position
    for (auto it = space_head_map.begin(); it != space_head_map.end(); ++it) {
        const int space = it->first;
        LinkedListNode<int> *space_head_node = it->second;
        if (space >= size) {
            const int start_pos = space_head_node->get_data();
            if (space_head_node->get_nxt() == nullptr) {
                space_head_map.erase(space);
            }
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
        LinkedListNode<SpaceNode> *space_list_at_zero = space_list_head->get_nxt();
        space_list_at_zero = space_list_at_zero->get_nxt();
        while ((space_list_at_zero->get_pre()->get_data().space + 
                space_list_at_zero->get_pre()->get_data().start_pos) == 
                space_list_at_zero->get_data().start_pos) {
                    space_list_at_zero->get_pre()->setData(
                        SpaceNode {
                            space_list_at_zero->get_data().space + space_list_at_zero->get_pre()->get_data().space,
                            space_list_at_zero->get_pre()->get_data().start_pos
                        }
                    );
                }
    }
    space_list_node->insert_back(
        SpaceNode {
            size,
            start_pos
        }
    );
    space_list_node = space_list_node->get_nxt();
    while ((space_list_node->get_pre()->get_data().space + 
            space_list_node->get_pre()->get_data().start_pos) == 
            space_list_node->get_data().start_pos) {
                space_list_node->get_pre()->setData(
                    SpaceNode {
                        space_list_node->get_data().space + space_list_node->get_pre()->get_data().space,
                        space_list_node->get_pre()->get_data().start_pos
                    }
                );
            space_list_node = space_list_node->remove_this();
            }   
}
