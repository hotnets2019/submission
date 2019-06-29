#ifndef NS2_DELAYTRANSPORT_H
#define NS2_DELAYTRANSPORT_H

#include <memory>
#include <delay_transport/DelayTransport.h>
#include <delay_transport/Environment.h>
#include <common/agent.h>
#include <tcp/tcp-full.h>
#include "DelayAssigner.h"

class DelayTransport : public MinTcpAgent, public delay_transport::Environment {
public:
    DelayTransport();

    void on_packet_drop(Packet *packet, double expiration_time, bool on_enque);

    int get_max_bytes_in_packet() const override;

    double get_current_time() const override;

    auto create_timer() -> shared_ptr<delay_transport::Timer> override;

    double get_maximum_slack() const override;

    int link_speed_in_gbps() const override;

public:
    ~DelayTransport() override ;

    void recv(Packet * packet, Handler * handler) override;

    int command(int argc, const char *const *argv) override;

protected:
    void delay_bind_init_all() override;

    void sendpacket(int seq, int ack, int flags, int dlen, int why, Packet *p) override;

    void bufferempty() override;

    int delay_bind_dispatch(const char *varName, const char *localName, TclObject *tracer) override;

private:
    delay_transport::DelayPacketMetadata extract_metadata(Packet *packet);

    delay_transport::ApplicationMessageData create_app_data(int num_bytes);

    int get_num_buffered_bytes();

private:
    void advance_bytes(int num_bytes) override;

private:
    std::unique_ptr<delay_transport::DelayTransport> transport_;
    std::shared_ptr<DelayAssigner> delay_assigner_;

    delay_transport::DelayTransport::message_id_t  last_message_id_;

    int start_iss_;
    double maximum_slack_;
    int link_rate_;
};


#endif //NS2_DELAYTRANSPORT_H
