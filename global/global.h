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

    static int get_rw_area_size() {
        return disk_size / 3 + 1;
    }
    
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

template <typename T>
class LinkedListNode {
public:
    // 构造函数，初始化数据及前后指针
    LinkedListNode(const T& data, LinkedListNode* pre = nullptr, LinkedListNode* nxt = nullptr)
        : data(data), pre(pre), nxt(nxt) {}

    T get_data() const { return data; }
    LinkedListNode* get_pre() const { return pre; }
    LinkedListNode* get_nxt() const { return nxt; }
    bool is_head() const { return pre == nullptr; }
    void setData(const T& data) {
        this.data = data;
    } 
    /**
     * 在节点前面添加新节点 (不支持用于头节点)
     * @return 添加的新节点
     */
    LinkedListNode* insert_front(T new_data) {
        // 确保当前节点不是头节点
        if (is_head()) {
            throw std::logic_error("Cannot insert_front on head node");
        }

        LinkedListNode* new_node = new LinkedListNode(new_data, pre, this);
        pre->nxt = new_node;
        pre = new_node;
        return new_node;
    }

    /**
     * 在节点后面添加新节点
     * @return 添加的新节点
     */
    LinkedListNode* insert_back(T new_data) {
        LinkedListNode* new_node = new LinkedListNode(new_data, this, nxt);
        if (nxt) {
            nxt->pre = new_node;
        }
        nxt = new_node;
        return new_node;
    }

    /**
     * 删除当前节点
     * @return 下一个节点
     */
    LinkedListNode* remove_this() {
        LinkedListNode* next_node = nxt;
        if (pre) {
            pre->nxt = nxt;
        }
        if (nxt) {
            nxt->pre = pre;
        }
        delete this;
        return next_node;
    }

private:
    T data;
    LinkedListNode *pre, *nxt;
};

#endif
