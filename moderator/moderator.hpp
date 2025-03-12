#include "read_regulator.hpp"
#include "write_regulator.hpp"

class Moderator {
public:
    virtual void execute_frame() = 0;
};

class DefaultModerator : public Moderator {
public:
    void execute_frame() override {
    }
};
