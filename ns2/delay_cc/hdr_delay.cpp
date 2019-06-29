#include "hdr_delay.h"

int hdr_delay::offset_;

static class DelayHeaderClass : public PacketHeaderClass {
public:
    DelayHeaderClass() : PacketHeaderClass("PacketHeader/Delay", sizeof(hdr_delay)) {
        bind_offset(&hdr_delay::offset_);
    }

} class_delayhdr;

bool hdr_delay::is_expired() const {
    return Scheduler::instance().clock() > expiration_time();
}

double hdr_delay::current_slack() const {
    return expiration_time() - Scheduler::instance().clock();
}

void hdr_delay::update_last_slack() {
    last_slack_ = current_slack();
}
