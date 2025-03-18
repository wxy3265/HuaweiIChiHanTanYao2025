#include "moderator.h"

bool check_frame();

// 仅作为程序入口使用
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    freopen("code_craft.log", "w", stderr);

    Global::init();
    Disk::init();

    // choose one moderator
    Moderator *moderator = new MostGreedModerator();

    while (Global::now_frame < Global::total_frame) {
        if (!check_frame()) {
            cerr << "Error: Frame check failed." << endl;
            // while (true) {
            //     string s;
            //     cin >> s;
            //     cerr << "next:" << s << '\n';
            // }
            break;
        }
        cin >> Global::now_frame;
        cout << "TIMESTAMP " << Global::now_frame << '\n';
        cout.flush();
        // cerr << "start execute frame " << Global::now_frame << "\n";
        moderator->execute_frame();
    }

    return 0;
}

inline bool check_frame() {
    string check_str;
    cin >> check_str;
    // cerr << "check_str:" << check_str << '\n';
    return check_str == "TIMESTAMP";
}
