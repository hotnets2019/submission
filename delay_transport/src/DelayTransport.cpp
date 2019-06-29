#include <delay_transport/DelayTransport.h>
#include <DelayTransportImpl.h>

namespace delay_transport {
auto DelayTransport::create_instance(Environment *environment) -> std::unique_ptr<DelayTransport> {
    return std::make_unique<DelayTransportImpl>(environment);
}


}
