#include "moderator.h"

void MostGreedModerator::init() {
    read_regulator = MostGreedReadRegulator();
    write_regulator = MostGreedWriteRegulator();
}

void MostGreedModerator::execute_frame() {
    int headTime = clock();
    cerr << "Head time:" << headTime << "\n";
    // cerr <
    totalOthers += headTime - lastHead;
    
    vector<int> deleted_obj_ids = read_regulator.handle_delete(); 
    int handleDelete = clock();{
        // cerr << "Handle Delete Time:" << headTime - handleDelete << "\n";
        handle_deleteTime += handleDelete - headTime;
    }

    
    write_regulator.update_deleted(deleted_obj_ids);
    int updateDelete = clock(); {
        update_deleteTime += updateDelete - handleDelete;
        // cerr << "Update Delete Time:" << updateDelete - handleDelete << '\n';
    }


    write_regulator.handle_write();
    int handleWrite = clock(); {
        writeTime += handleWrite - updateDelete;
    //     cerr << "Handle Write Time:" << handleWrite - updateDelete << "\n";
    }
    

    read_regulator.handle_read();
    int headleRead = clock();{
        readTime += headleRead - handleWrite;
    //     cerr << "Read Time:" << readTime << "\n";
    }
    
    cerr << "Total Other Time:" << totalOthers << "\n";
    cerr << "Total Handle Delete Time:" << handle_deleteTime << "\n";
    cerr << "Total Update Delete Time:" << update_deleteTime << "\n";
    cerr << "Total Write Time:" << writeTime << "\n";
    cerr << "Total Read Time:" << readTime << "\n";
    lastHead = clock();
}

