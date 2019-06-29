#ifndef NS2_PRIORITYQUEUE_H
#define NS2_PRIORITYQUEUE_H


#include <queue/queue.h>
#include <queue>
#include <set>

class PriorityQueue : public Queue {
public:
    PriorityQueue();
    ~PriorityQueue() override;

public:
    void enque(Packet * packet) override;

    void drop_w_info(Packet *p, bool on_enque);

    Packet *deque() override;

private:
    struct EDFComparator {
        bool operator()(Packet * rhs, Packet * lhs) const;
    };
    struct PriorityEDFComparator {
        bool operator()(Packet * rhs, Packet * lhs) const;
    };

private:
    double last_deque_time{};

    std::deque<Packet *> non_delay_pkts_;
    std::set<Packet *, PriorityEDFComparator> priority_queue_;
    std::set<Packet *, EDFComparator> expiration_queue_;
};


#endif //NS2_PRIORITYQUEUE_H
