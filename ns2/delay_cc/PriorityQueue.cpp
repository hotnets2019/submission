#include "PriorityQueue.h"
#include "hdr_delay.h"

static class PriorityQueueClass : public TclClass {
public:
    PriorityQueueClass() : TclClass("Queue/PriorityEDF") {}
    TclObject* create(int, const char * const *) {
        return new PriorityQueue();
    }
} class_priqrity_queue;

bool PriorityQueue::EDFComparator::operator()(Packet *p1, Packet *p2) const {
    auto const dh1 = hdr_delay::access(p1);
    auto const dh2 = hdr_delay::access(p2);
    return dh1->expiration_time() < dh2->expiration_time()
        || (dh1->expiration_time() == dh2->expiration_time() && p1 < p2);
}

bool PriorityQueue::PriorityEDFComparator::operator()(Packet *rhs, Packet *lhs) const {
    auto const dh1 = hdr_delay::access(rhs);
    auto const dh2 = hdr_delay::access(lhs);
    return dh1->priority() > dh2->priority()
        || (dh1->priority() == dh2->priority() && dh1->expiration_time() < dh2->expiration_time())
        || (dh1->priority() == dh2->priority() && dh1->expiration_time() == dh2->expiration_time() && rhs < lhs);
}

Packet *PriorityQueue::deque() {
    if (!non_delay_pkts_.empty()) {
        auto result = non_delay_pkts_.front();
        non_delay_pkts_.pop_front();
        last_deque_time = Scheduler::instance().clock();
        return result;
    }

    while (!expiration_queue_.empty()) {
        auto hol = *begin(expiration_queue_);
        auto const dh = hdr_delay::access(hol);
        if (!dh->is_expired()) {
            break;
        }
        expiration_queue_.erase(begin(expiration_queue_));
        priority_queue_.erase(hol);
        drop_w_info(hol, false);
    }

    if (priority_queue_.empty()) {
        return nullptr;
    }

    auto const packet = *begin(priority_queue_);
    priority_queue_.erase(begin(priority_queue_));
    expiration_queue_.erase(packet);
    hdr_delay::access(packet)->hop_count()++;

    last_deque_time = Scheduler::instance().clock();

    return packet;
}

void PriorityQueue::enque(Packet *packet) {
    if (expiration_queue_.empty()) {
        last_deque_time = -1.0;
    }

    auto const dh = hdr_delay::access(packet);
    if (dh->is_present()) {
        if (dh->is_expired()) {
            drop_w_info(packet, true);
        } else {
            dh->update_last_slack();
            expiration_queue_.insert(packet);
            priority_queue_.insert(packet);
            if (priority_queue_.size() > qlim_) {
                auto const eol = *(--end(priority_queue_));
                priority_queue_.erase(eol);
                expiration_queue_.erase(eol);
                drop_w_info(eol, true);
            }
        }
    } else {
        non_delay_pkts_.push_back(packet);
    }
}

void PriorityQueue::drop_w_info(Packet *p, bool on_enque) {
    auto const expiration_time = last_deque_time < 0 ? -1.0 : Scheduler::instance().clock() - last_deque_time;
    hdr_delay::access(p)->transport()->on_packet_drop(p, expiration_time, on_enque);
    Connector::drop(p);
}

PriorityQueue::~PriorityQueue() = default;

PriorityQueue::PriorityQueue()
    : last_deque_time{-1.0} {

}
