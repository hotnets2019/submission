#ifndef NS2_HDR_DELAY_H
#define NS2_HDR_DELAY_H

#include "packet.h"
#include "DelayTransport.h"

class hdr_delay {
public:
    inline static hdr_delay * access(Packet * p) {
        return reinterpret_cast<hdr_delay *>(p->access(offset_));
    }

    DelayTransport*& transport() { return transport_; }
    double expiration_time() const { return expiration_times_[hop_count_]; }
    array<double, 4>& expiration_times() { return expiration_times_; }
    double& msg_creation_time() { return msg_creation_time_; }
    double& msg_expiration_time() { return msg_expiration_time_; }
    int& message_id() { return message_id_; }
    int& message_length() { return message_length_; }
    int& first_byte() { return first_byte_; }
    int& last_byte() { return last_byte_; }
    int& hop_count() { return hop_count_; }
    int& retransmission_idx() { return retransmission_idx_; }
    double& priority() { return priority_; }
    double last_slack() const { return last_slack_; }
    
    void update_last_slack();
    double current_slack() const;
    bool is_expired() const;
    bool is_present() const { return transport_; }

private:
    inline static int const& offset() { return offset_; };
    friend class DelayHeaderClass;

private:
    static int offset_;

private:
    double last_slack_;
    array<double, 4> expiration_times_;
    double msg_creation_time_;
    double msg_expiration_time_;
    int message_id_;
    int message_length_;

    int first_byte_;
    int last_byte_;

    double priority_;
    int retransmission_idx_;

    DelayTransport * transport_;
    int hop_count_;
};


#endif //NS2_HDR_DELAY_H
