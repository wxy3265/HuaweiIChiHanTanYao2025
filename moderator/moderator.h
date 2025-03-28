#include "read_regulator.h"
#include "write_regulator.h"

// 总体策略（跨操作的整体策略层）
class Moderator {
public:
    virtual void execute_frame() = 0; // 每一帧操作
};

class MostGreedModerator: public Moderator {
public:
    MostGreedModerator() { init(); }
    void execute_frame() override;
    int readTime = 0;
    int handle_deleteTime = 0;
    int update_deleteTime = 0;
    int writeTime = 0;
    int lastHead = 0;
    int totalOthers = 0;
private:
    MostGreedWriteRegulator write_regulator; // 使用的读操作策略
    MostGreedReadRegulator read_regulator; // 使用的写操作策略
    void init();
};
