#include "PointerPeer.h"

PointerPeer::PointerPeer(DelayTransport *transport) : transport{transport} {

}

DelayTransport *PointerPeer::get_transport() const {
    return transport;
}

bool PointerPeer::equal(delay_transport::Peer const &other) const {
    // in NS2 there is only a single peer per agent
    return true;
}
