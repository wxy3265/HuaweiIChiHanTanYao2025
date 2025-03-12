#include "read_regulator.h"
#include "write_regulator.h"

class Moderator {
public:
    virtual void execute_frame() = 0;
};

class DefaultModerator : public Moderator {
public:
    void execute_frame() override {
    }
};
