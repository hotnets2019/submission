#ifndef DELAY_TRANSPORT_DELAYTRANSPORT_H
#define DELAY_TRANSPORT_DELAYTRANSPORT_H

#include <delay_transport/ApplicationMessageData.h>
#include <delay_transport/DelayPacketMetadata.h>
#include <delay_transport/Environment.h>
#include <memory>
#include <string>
#include "DropInfo.h"

namespace delay_transport {
struct DelayTransport {
    using message_id_t = int;

    static auto create_instance(Environment *environment) -> std::unique_ptr<DelayTransport>;

    virtual auto on_new_app_message(ApplicationMessageData const &msg) -> message_id_t = 0;

    virtual void on_message_delivered(message_id_t msg_id) = 0;

    virtual void on_packet_drop(DelayPacketMetadata metadata, DropInfo info) = 0;

    virtual DelayPacketMetadata
    create_packet_metadata(message_id_t msg_id, int first_byte, int last_byte, int bytes_remaining) = 0;

    virtual void on_packet_received(const DelayPacketMetadata &metadata) = 0;

    DelayTransport() = default;

    DelayTransport(DelayTransport const &) = delete;

    DelayTransport const &operator=(DelayTransport const &) = delete;

    virtual ~DelayTransport() = default;
};
}

#endif //DELAY_TRANSPORT_DELAYTRANSPORT_H
