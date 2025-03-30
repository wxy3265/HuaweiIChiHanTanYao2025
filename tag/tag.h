//
// Created by Lenovo on 25-3-29.
//

#ifndef TAG_H
#define TAG_H

#include "object.h"
class Tag {
public:
    Tag() : id(-1), del(0), write(0), read(0), weight(0) {};
    Tag(int id, int del, int write, int read) : id(id), del(del), write(write), read(read), weight(0), occupied(write * 3){};

    static Tag tags[Global::MAX_TAG_NUM]; // 所有磁盘的实例
    static void init();

    int get_occupied()const{ return occupied;}
    int get_weight()const{ return weight;}
    int get_write()const{ return write;}
    int get_read()const{ return read;}
    int get_id()const{ return id;}

    void set_occupied(int op){occupied += op;}
    void set_weight(){ weight = del + write + read;}
    void set_write(int wr){write += wr;}
    void set_read(int rd){read += rd;}
    void set_del(int de){del += de;}


private:
    int id;         // tag 编号
    int del;        // 删除量
    int write;      // 写入量（同时作为所需区域长度）
    int read;       // 读取量
    int weight;     // 权重
    int occupied;   // 实际占据大小
};



#endif //TAG_H
