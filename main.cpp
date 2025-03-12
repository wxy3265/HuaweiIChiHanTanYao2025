#include "moderator.hpp"

bool check_frame();

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    freopen("code_craft.log", "w", stderr);

    Global::init();

    // choose one moderator
    Moderator *moderator = new DefaultModerator();

    while (Global::now_frame < Global::total_frame) {
        if (!check_frame()) {
            cerr << "Error: Frame check failed." << endl;
            break;
        }
        cin >> Global::now_frame;
        moderator->execute_frame();
    }

    return 0;
}

inline bool check_frame() {
    string check_str;
    cin >> check_str;
    return check_str == "TIMESTAMP";
}
