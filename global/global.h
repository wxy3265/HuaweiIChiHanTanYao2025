#ifndef GLOBAL_H
#define GLOBAL_H
#include "ichihan.h"

// 全局变量
class Global {
public:
    static const int MAX_FRAME = 86400; // 最大帧数
    static const int MAX_TAG_NUM = 16; // 最大标签数
    static const int MAX_DISK_NUM = 10; // 最大磁盘数
    static const int MAX_DISK_SIZE = 16384; // 最大磁盘大小
    static const int MAX_TOKEN_NUM = 1000; // 最大令牌数
    static const int MAX_TAG_BLOCK = (int)((MAX_FRAME + 1799) / 1800); // 最大tag块数
    static const int MAX_OBJ_SIZE = 5; // 最大对象大小

    static int total_frame; // 总帧数   
    static int disk_num; // 磁盘数
    static int tag_num; // 标签数
    static int disk_size; // 磁盘大小
    static int total_tokens; // 总令牌数
    static int tag_block; // tag块数
    static int fre_del[MAX_TAG_NUM][MAX_TAG_BLOCK]; // tag删除频率
    static int fre_write[MAX_TAG_NUM][MAX_TAG_BLOCK]; // tag写频率
    static int fre_read[MAX_TAG_NUM][MAX_TAG_BLOCK]; // tag读频率
    static int now_frame; // 当前帧数
    
    // 初始化全局变量
    static void init() {
        now_frame = 1;
        init_from_interaction();
    }

private:
    // 判题器输入初始化
    static void init_from_interaction() {
        scanf("%d%d%d%d%d", &total_frame, &tag_num, &disk_num, &disk_size, &total_tokens);
        tag_block = ceil((total_frame + 1799) / 1800);
        total_frame += 105;
        // cerr << tag_num << " " << tag_block << "\n";
        for (int i = 0; i < tag_num; i++) {
            for (int j = 0; j < tag_block; j++) {
                fre_del[i][j] = read_Fast();
                // cerr << fre_del[i][j] << "\n";
            }
        }
        for (int i = 0; i < tag_num; i++) {
            for (int j = 0; j < tag_block; j++) {
                fre_write[i][j] = read_Fast();
                // cerr << fre_write[i][j] << "\n";
            }
        }
        for (int i = 0; i < tag_num; i++) {
            for (int j = 0; j < tag_block; j++) {
                fre_read[i][j] = read_Fast();
                // cerr << fre_read[i][j] << "\n";
            }
        }
        cout << "OK\n";
        // cerr << "OK\n";
        cout.flush();
    }
};
#endif
