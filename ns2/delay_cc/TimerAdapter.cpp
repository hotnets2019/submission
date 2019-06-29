#include "TimerAdapter.h"

bool TimerAdapter::is_scheduled() const {
    return const_cast<TimerAdapter*>(this)->status() == TIMER_PENDING;
}

void TimerAdapter::reschedule(double time, std::function<void()> callback) {
    callback_ = callback;
    TimerHandler::resched(time);
}

void TimerAdapter::expire(Event *event) {
    callback_();
}
