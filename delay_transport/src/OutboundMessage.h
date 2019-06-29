#ifndef DELAY_TRANSPORT_OUTBOUNDMESSAGE_H
#define DELAY_TRANSPORT_OUTBOUNDMESSAGE_H

#include <delay_transport/Environment.h>
#include <delay_transport/ApplicationMessageData.h>
#include <delay_transport/DelayPacketMetadata.h>
#include <delay_transport/DropInfo.h>
#include <set>
#include <map>
#include <optional>
#include "ExpirationListener.h"


namespace delay_transport {

class OutboundMessage {
public:
    OutboundMessage(Environment *environment, ApplicationMessageData data, int msg_id,
                    ExpirationListener *expiration_listener);

    DelayPacketMetadata create_packet_metadata(int first_byte, int last_byte, int bytes_remaining);
    void handle_drop(const DelayPacketMetadata& packet, DropInfo const &info);

    int get_msg_id() const { return msg_id; }
    auto get_app_message() const -> ApplicationMessageData const& { return app_message; }

private:
    struct TimeSlackComparator {
        bool operator()(std::pair<double, double> const& rhs, std::pair<double, double> const& lhs) const;
    };

private:
    static auto to_expiration_time(std::pair<double, double> const &time_slack) -> double;

private:
    int get_num_total_packets();

    DelayPacketMetadata generate_packet_metadata(int first_byte, int last_byte, int bytes_remaining) const;

    void set_uniform_expiration_time(DelayPacketMetadata &packet) const;
    void set_same_expiration_time(DelayPacketMetadata &packet) const;
    void check_expiration();

    double calc_expiration_time() const;
    double calc_priority(int bytes_remaining) const;

private:
    bool has_expired;

    int const msg_id;
    ApplicationMessageData const app_message;
    Environment * const environment;
    ExpirationListener * const expiration_listener;

    double last_expiration_time;
};

}

#endif //DELAY_TRANSPORT_OUTBOUNDMESSAGE_H
