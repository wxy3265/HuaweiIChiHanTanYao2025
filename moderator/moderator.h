#include "read_regulator.h"
#include "write_regulator.h"

class Moderator {
public:
    virtual void execute_frame() = 0;
};

class NullModerator : public Moderator {
public:
    void execute_frame() override {}
};

class MostGreedModerator: public Moderator {
public:
    MostGreedModerator() { init(); }
    void execute_frame() override;

private:
    ReadRegulator readRegulator;
    WriteRegulator writeRegulator;
    void init();
};
