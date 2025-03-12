#include "global.hpp"

int Global::total_frame = 0;
int Global::disk_num;
int Global::tag_num;
int Global::disk_size;
int Global::total_tokens;
int Global::tag_block;
int Global::fre_del[MAX_TAG_NUM][MAX_TAG_BLOCK];
int Global::fre_write[MAX_TAG_NUM][MAX_TAG_BLOCK];
int Global::fre_read[MAX_TAG_NUM][MAX_TAG_BLOCK];
int Global::now_frame;
