#include "read_regulator.h"
#include "write_regulator.h"

class Moderator {
public:
    virtual void execute_frame() = 0;
};

class MostGreedModerator: public Moderator {
public:
    MostGreedModerator() { init(); }
    void execute_frame() override;

private:
    MostGreedWriteRegulator write_regulator;
    MostGreedReadRegulator read_regulator;
    void init();
};
