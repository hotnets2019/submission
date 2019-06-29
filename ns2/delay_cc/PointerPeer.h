#ifndef NS2_POINTERPEER_H
#define NS2_POINTERPEER_H

#include <delay_transport/Peer.h>
#include "DelayTransport.h"

class PointerPeer : public delay_transport::Peer {
public:
    explicit PointerPeer(DelayTransport *transport = nullptr);

    bool equal(Peer const &other) const override;

    DelayTransport *get_transport() const;

private:
    DelayTransport *transport;
};


#endif //NS2_POINTERPEER_H
