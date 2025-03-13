#ifndef GLOBAL_H
#define GLOBAL_H
#include "ichihan.h"

class Global {
public:
    static const int MAX_FRAME = 86400;
    static const int MAX_TAG_NUM = 16;
    static const int MAX_DISK_NUM = 10;
    static const int MAX_DISK_SIZE = 16384;
    static const int MAX_TOKEN_NUM = 1000;
    static const int MAX_TAG_BLOCK = (int)((MAX_FRAME + 1799) / 1800);
    static const int MAX_OBJ_SIZE = 5;

    static int total_frame;
    static int disk_num;
    static int tag_num;
    static int disk_size;
    static int total_tokens;
    static int tag_block;
    static int fre_del[MAX_TAG_NUM][MAX_TAG_BLOCK];
    static int fre_write[MAX_TAG_NUM][MAX_TAG_BLOCK];
    static int fre_read[MAX_TAG_NUM][MAX_TAG_BLOCK];
    static int now_frame;
    
    static void init() {
        now_frame = 1;
        init_from_interaction();
    }

private:
    static void init_from_interaction() {
        scanf("%d%d%d%d%d", &total_frame, &tag_num, &disk_num, &disk_size, &total_tokens);
        total_frame += 105;
        tag_block = ceil(total_frame / 1800);
        for (int i = 0; i < tag_num; i++) {
            for (int j = 0; j < tag_block; j++) {
                cin >> fre_del[i][j];
            }
        }
        for (int i = 0; i < tag_num; i++) {
            for (int j = 0; j < tag_block; j++) {
                cin >> fre_write[i][j];
            }
        }
        for (int i = 0; i < tag_num; i++) {
            for (int j = 0; j < tag_block; j++) {
                cin >> fre_read[i][j];
            }
        }
        cout << "OK\n";
        cout.flush();
    }
};
#endif
