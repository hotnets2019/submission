#include <algorithm>
#include "PIASDelayAssigner.h"

double PIASDelayAssigner::assign_delay(int msg_len) {
    //auto const min_time = 2 * msg_len * 8.0 * 1.e-9 / *link_rate_;
    if (msg_len <= 100 * 1024) {
        return 4.e-4;
    } else if (msg_len <= 10 * 1024 * 1024) {
        return 6e-2;
    } else {
        return 0.150;
    }
}

PIASDelayAssigner::PIASDelayAssigner(int const *link_rate) : link_rate_{link_rate} {

}
