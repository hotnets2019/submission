#ifndef NS2_DELAYASSIGNER_H
#define NS2_DELAYASSIGNER_H

struct DelayAssigner {
    virtual double assign_delay(int msg_len) = 0;

    DelayAssigner() = default;
    DelayAssigner(DelayAssigner const&) = delete;
    DelayAssigner const& operator=(DelayAssigner const&) = delete;

    virtual ~DelayAssigner() = default;
};
#endif //NS2_DELAYASSIGNER_H
