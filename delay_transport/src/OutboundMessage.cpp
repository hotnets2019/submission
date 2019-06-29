#include "OutboundMessage.h"
#include <delay_transport/Error.h>
#include <cmath>
#include <cassert>
#include <numeric>

namespace delay_transport {

OutboundMessage::OutboundMessage(Environment *environment, ApplicationMessageData data, int msg_id,
                                 ExpirationListener *expiration_listener)
        : app_message{std::move(data)}
        , msg_id{msg_id}
        , environment{environment}
        , expiration_listener{expiration_listener}
        , last_expiration_time{0.0}
        , has_expired{false} {
}

DelayPacketMetadata OutboundMessage::create_packet_metadata(int first_byte, int last_byte, int bytes_remaining) {
    check_expiration();
    return generate_packet_metadata(first_byte, last_byte, bytes_remaining);
}

double OutboundMessage::calc_expiration_time() const {
    if (has_expired) {
        return environment->get_current_time() + environment->get_maximum_slack();
    } else {
        return app_message.get_expiration_time();
    }
}

DelayPacketMetadata
OutboundMessage::generate_packet_metadata(int first_byte, int last_byte, int bytes_remaining) const {
    auto packet_metadata = DelayPacketMetadata{};
    packet_metadata.to_peer = app_message.to_peer;
    packet_metadata.from_peer = app_message.from_peer;

    packet_metadata.first_byte = first_byte;
    packet_metadata.last_byte = last_byte;


    packet_metadata.message_id = msg_id;
    packet_metadata.message_len = app_message.length_in_bytes;

    packet_metadata.message_creation_time = app_message.creation_time;
    packet_metadata.message_expiration_time = app_message.get_expiration_time();

    packet_metadata.set_expiration_time(calc_expiration_time());
    set_same_expiration_time(packet_metadata);

    packet_metadata.priority = calc_priority(bytes_remaining);

    return packet_metadata;
}

void OutboundMessage::handle_drop(const DelayPacketMetadata& packet, DropInfo const &info) {
}

void OutboundMessage::set_uniform_expiration_time(DelayPacketMetadata &packet) const {
    auto const per_hop_expiration = (packet.get_expiration_time() - environment->get_current_time()) / packet.expiration_times.size();
    std::fill(begin(packet.expiration_times), end(packet.expiration_times), per_hop_expiration);
    packet.expiration_times.front() += environment->get_current_time();
    std::partial_sum(begin(packet.expiration_times), end(packet.expiration_times), begin(packet.expiration_times));
}

int OutboundMessage::get_num_total_packets() {
    return (app_message.length_in_bytes + environment->get_max_bytes_in_packet() - 1) / environment->get_max_bytes_in_packet();
}

auto OutboundMessage::to_expiration_time(std::pair<double, double> const &time_slack) -> double {
    return time_slack.first + time_slack.second;
}

double OutboundMessage::calc_priority(int bytes_remaining) const {
    return has_expired ? 0.0 : 1.0;
}

void OutboundMessage::set_same_expiration_time(DelayPacketMetadata &packet) const {
    std::fill(begin(packet.expiration_times), end(packet.expiration_times), packet.get_expiration_time());
}

void OutboundMessage::check_expiration() {
    if (environment->get_current_time() >= app_message.get_expiration_time()) {
        has_expired = true;
    }
}

bool OutboundMessage::TimeSlackComparator::operator()(std::pair<double, double> const &rhs,
                                                      std::pair<double, double> const &lhs) const {
    return to_expiration_time(rhs) < to_expiration_time(lhs);
}
}
