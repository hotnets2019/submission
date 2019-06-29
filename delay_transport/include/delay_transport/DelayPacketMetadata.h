#ifndef DELAY_TRANSPORT_DELAYPACKETMETADATA_H
#define DELAY_TRANSPORT_DELAYPACKETMETADATA_H

#include <delay_transport/Peer.h>
#include <memory>
#include <array>

namespace delay_transport {
struct DelayPacketMetadata {
    std::shared_ptr<Peer> from_peer;
    std::shared_ptr<Peer> to_peer;
    int first_byte;
    int last_byte;

    int message_id;
    int message_len;

    double message_creation_time;
    double message_expiration_time;
    std::array<double, 4> expiration_times;

    double priority;
    int retransmission_idx;

    auto get_num_message_bytes() const -> int {
        return last_byte - first_byte + 1;
    }

    double get_expiration_time() const {
        return expiration_times[expiration_times.size()- 1];
    }

    void set_expiration_time(double expiration_time) {
        expiration_times[expiration_times.size() - 1] = expiration_time;
    }

    bool is_retransmission() const {
        return retransmission_idx > 0;
    }
};
}

#endif //DELAY_TRANSPORT_DELAYPACKETMETADATA_H
