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
    for (auto space_list_node = space_list_head; space_list_node->get_nxt() != nullptr;) {
        space_list_node = space_list_node->get_nxt();
        const int space = space_list_node->get_data().space;
        const int space_start_pos = space_list_node->get_data().start_pos;
        cerr << "disk:" << disk_id << "space:" << space << " " << space_list_node->get_nxt() << ' ' << space_list_node << ' ' << size << '\n';
        if (space >= size) {
            if (space > size) {
                space_list_node->insert_back(
                    SpaceNode {
                        space - size,
                        (space_start_pos + size) % Disk::disks[disk_id].getRWAreaSize()
                    }
                );
            }
            space_list_node->remove_this();
            return space_start_pos;
        }
    }
    return -1;
}

void ObjectWriter::update_delete(int start_pos, int size) {
    cerr << "!!q1:\n";
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
    cerr << "!!q2:\n";
//    cerr << "IS insert ?\n";
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
                    // LinkedListNode<int> *need_delete_node = space_head_map[space_list_at_zero->get_data().space];
                    // while (need_delete_node->get_data() != space_list_at_zero->get_data().start_pos) {
                    //     need_delete_node->get_nxt();
                    // }
                    // need_delete_node->remove_this();
                    // need_delete_node = space_head_map[space_list_at_zero->get_pre()->get_data().space];
                    // while (need_delete_node->get_data() != space_list_at_zero->get_pre()->get_data().start_pos) {
                    //     need_delete_node->get_nxt();
                    // }
                    // need_delete_node->remove_this();
                    space_list_at_zero->get_pre()->setData(
                        SpaceNode {
                            space_list_at_zero->get_data().space + space_list_at_zero->get_pre()->get_data().space,
                            space_list_at_zero->get_pre()->get_data().start_pos
                        }
                    );
//                    int insetNum = space_list_at_zero->get_data().space +
//                                   space_list_at_zero->get_pre()->get_data().space;
//                    if (insetNum == 1 && space_list_at_zero->get_pre()->get_data().start_pos == 1) {
//                        cerr << "IN Zero Area \n";
//                    }
                    // insert_to_map(space_list_at_zero->get_data().space + 
                    //               space_list_at_zero->get_pre()->get_data().space, 
                    //               space_list_at_zero->get_pre()->get_data().start_pos);
                    space_list_at_zero = space_list_at_zero->remove_this();
                }
    }
    cerr << "!!q3:\n";
    space_list_node->insert_back(
        SpaceNode {
            size,
            start_pos
        }
    );
    // insert_to_map(size, start_pos);
//    if (size == 1 && start_pos == 1) {
//        cerr << "IN normal Area\n";
//    }
    cerr << "!!q4:\n";
    space_list_node = space_list_node->get_nxt();
    while ((space_list_node->get_pre()->get_data().space + 
            space_list_node->get_pre()->get_data().start_pos) == 
            space_list_node->get_data().start_pos) {
                // LinkedListNode<int> *need_delete_node = space_head_map[space_list_node->get_data().space];
                //     while (need_delete_node->get_data() != space_list_node->get_data().start_pos) {
                //         need_delete_node->get_nxt();
                //     }
                //     need_delete_node->remove_this();
                //     need_delete_node = space_head_map[space_list_node->get_pre()->get_data().space];
                //     while (need_delete_node->get_data() != space_list_node->get_pre()->get_data().start_pos) {
                //         need_delete_node->get_nxt();
                //     }
                //     need_delete_node->remove_this();
                cerr <<"sizeNum:" <<space_list_node->get_pre()->get_data().space <<" " <<  
                space_list_node->get_pre()->get_data().start_pos << " " << 
                space_list_node->get_data().start_pos << " " << Disk::disks[disk_id].getRWAreaSize() << "\n";
                space_list_node->get_pre()->setData(
                    SpaceNode {
                        space_list_node->get_data().space + space_list_node->get_pre()->get_data().space,
                        space_list_node->get_pre()->get_data().start_pos
                    }
                );
                cerr << "Finished\n";
//                int insetNum = space_list_node->get_data().space +
//                        space_list_node->get_pre()->get_data().space;
//                if (insetNum == 1 && space_list_node->get_pre()->get_data().start_pos == 1) {
//                    cerr << "IN many Area \n";
//                }
                // insert_to_map(spacesist_node->get_pre()->get_data().start_pos);
                // cerr << space_list_node->get_nxt() << "\n";

                space_list_node = space_list_node->remove_this();
                if (space_list_node == nullptr)
                    break;
                // cerr << space_list_node->get_data().space << " " << space_list_node->get_data().start_pos << "\n";
    }   
    cerr << "!!q5:\n";
}
