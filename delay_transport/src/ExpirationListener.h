#ifndef DELAY_TRANSPORT_EXPIRATIONLISTENER_H
#define DELAY_TRANSPORT_EXPIRATIONLISTENER_H

#include <delay_transport/ApplicationMessageData.h>

namespace delay_transport {

struct ExpirationListener {
    virtual void report_expiration(ApplicationMessageData const& app, double data_fraction, double slack_fraction) = 0;

    ExpirationListener() = default;
    ExpirationListener(ExpirationListener const&) = delete;
    ExpirationListener const& operator=(ExpirationListener const&) = delete;

    virtual ~ExpirationListener() = default;
};

}

#endif //DELAY_TRANSPORT_EXPIRATIONLISTENER_H
