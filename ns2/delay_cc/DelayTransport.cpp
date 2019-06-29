#include <tclcl.h>
#include <common/simulator.h>
#include <iostream>
#include "TimerAdapter.h"
#include "DelayTransport.h"
#include "PointerPeer.h"
#include "hdr_delay.h"
#include "FileDelayAssigner.h"
#include "PIASDelayAssigner.h"

static class DelayTransportClass : public TclClass {
public:
    DelayTransportClass() : TclClass("Agent/TCP/FullTcp/Sack/MinTCP/DelayTransport") {}
    TclObject * create(int , char const * const *) override {
        return new DelayTransport();
    }
} class_delay;

DelayTransport::DelayTransport()
    : MinTcpAgent{}
    , Environment{}
    , transport_{delay_transport::DelayTransport::create_instance(this)}
    , delay_assigner_{make_shared<PIASDelayAssigner>(&link_rate_)}
    , maximum_slack_{0.0} {
}

void DelayTransport::recv(Packet * packet, Handler * handler) {
    auto dh = hdr_delay::access(packet);
    if (dh->is_present()) {
        transport_->on_packet_received(extract_metadata(packet));
    }
    MinTcpAgent::recv(packet, handler);
}

delay_transport::DelayPacketMetadata DelayTransport::extract_metadata(Packet *packet) {
    delay_transport::DelayPacketMetadata packet_data;
    auto dh = hdr_delay::access(packet);
    assert(dh->is_present());

    copy(begin(dh->expiration_times()), end(dh->expiration_times()), begin(packet_data.expiration_times));
    packet_data.message_expiration_time = dh->msg_expiration_time();
    packet_data.message_creation_time = dh->msg_creation_time();
    packet_data.first_byte = dh->first_byte();
    packet_data.last_byte = dh->last_byte();
    packet_data.message_id = dh->message_id();
    packet_data.message_len = dh->message_length();
    packet_data.from_peer = make_shared<PointerPeer>(dh->transport());
    packet_data.to_peer = make_shared<PointerPeer>(this);
    packet_data.retransmission_idx = dh->retransmission_idx();
    packet_data.priority = dh->priority();
    return packet_data;
}

void DelayTransport::advance_bytes(int num_bytes) {
    start_iss_ = t_seqno_ ? t_seqno_ - 1 : 0;
    last_message_id_ = transport_->on_new_app_message(create_app_data(num_bytes));
    MinTcpAgent::advance_bytes(num_bytes);
}

delay_transport::ApplicationMessageData DelayTransport::create_app_data(int num_bytes) {
    delay_transport::ApplicationMessageData app_data;
    app_data.length_in_bytes = num_bytes;
    app_data.creation_time = Scheduler::instance().clock();
    app_data.from_peer = make_shared<PointerPeer>(this);

    app_data.slack = delay_assigner_->assign_delay(num_bytes);

    return app_data;
}

DelayTransport::~DelayTransport() = default;

void DelayTransport::on_packet_drop(Packet *packet, double expiration_time, bool on_enque) {
    delay_transport::DropInfo info {
        on_enque ? delay_transport::DropReason ::EXPIRED_ENQUE : delay_transport::DropReason ::EXPIRED_DEQUE,
        hdr_delay::access(packet)->hop_count(),
        hdr_delay::access(packet)->last_slack(),
        expiration_time
    };
    transport_->on_packet_drop(extract_metadata(packet), info);
}

double DelayTransport::get_current_time() const {
    return Scheduler::instance().clock();
}

auto DelayTransport::create_timer() -> shared_ptr<delay_transport::Timer> {
    return make_shared<TimerAdapter>();
}

void DelayTransport::delay_bind_init_all() {
    delay_bind_init_one("maximum_slack_");
    delay_bind_init_one("link_speed_");
    MinTcpAgent::delay_bind_init_all();
}

int DelayTransport::delay_bind_dispatch(const char *varName, const char *localName, TclObject *tracer) {
    if (delay_bind(varName, localName, "maximum_slack_", &maximum_slack_, tracer)) return TCL_OK;
    if (delay_bind(varName, localName, "link_speed_", &link_rate_, tracer)) return TCL_OK;
    return MinTcpAgent::delay_bind_dispatch(varName, localName, tracer);
}

double DelayTransport::get_maximum_slack() const {
    return maximum_slack_;
}

int DelayTransport::get_max_bytes_in_packet() const {
    return maxseg_;
}

void DelayTransport::bufferempty() {
    transport_->on_message_delivered(last_message_id_);
    MinTcpAgent::bufferempty();
}

void DelayTransport::sendpacket(int seq, int ack, int flags, int dlen, int why, Packet *p) {
    if (!p) {
        p = allocpkt();
    }

    auto const packet_metadata = transport_->create_packet_metadata(
            last_message_id_, seq - start_iss_, seq - start_iss_ + dlen - 1, get_num_buffered_bytes());

    auto const dh = hdr_delay::access(p);
    dh->msg_creation_time() = packet_metadata.message_creation_time;
    copy(begin(packet_metadata.expiration_times), end(packet_metadata.expiration_times), begin(dh->expiration_times()));
    dh->msg_expiration_time() = packet_metadata.message_expiration_time;
    dh->message_id() = packet_metadata.message_id;
    dh->message_length() = packet_metadata.message_len;
    dh->first_byte() = packet_metadata.first_byte;
    dh->last_byte() = packet_metadata.last_byte;
    dh->retransmission_idx() = packet_metadata.retransmission_idx;
    dh->priority() = packet_metadata.priority;
    dh->transport() = this;

    MinTcpAgent::sendpacket(seq, ack, flags, dlen, why, p);
}

int DelayTransport::command(int argc, const char *const *argv) {
    if (argc == 3) {
        if (strcmp(argv[1], "set-file-delays") == 0) {
            delay_assigner_ = make_shared<FileDelayAssigner>(argv[2]);
            return TCL_OK;
        }
    }
    return MinTcpAgent::command(argc, argv);
}

int DelayTransport::get_num_buffered_bytes() {
    return highest_ack_ < 0 ? curseq_ - iss_ : curseq_ - highest_ack_ + 1;
}

int DelayTransport::link_speed_in_gbps() const {
    return link_rate_;
}


