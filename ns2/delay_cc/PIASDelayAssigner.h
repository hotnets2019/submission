#ifndef NS2_PIASDELAYASSIGNER_H
#define NS2_PIASDELAYASSIGNER_H


#include "DelayAssigner.h"

class PIASDelayAssigner : public DelayAssigner {
public:
    explicit PIASDelayAssigner(int const *link_rate);
    double assign_delay(int msg_len) override;
private:
    int const * const link_rate_;
};


#endif //NS2_PIASDELAYASSIGNER_H
