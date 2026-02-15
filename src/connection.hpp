#ifndef __CONNECTION_HPP__
#define __CONNECTION_HPP__

#include <memory>

template<class Base, class Derived>
concept isDerived = std::is_base_of<Base, Derived>::value;

namespace BareSignal 
{

class SignalObject;

template<class... ParamPack>
class Connection
{
public:
    typedef void(SignalObject::*Signal_t)(ParamPack...);
    typedef void(SignalObject::*Callback_t)(ParamPack...);

    template<class Sender,
             class SenderBase,
             class Receiver,
             class ReceiverBase>
    requires isDerived<SenderBase, Sender> &&
             isDerived<ReceiverBase, Receiver>
    Connection(std::shared_ptr<Sender> sender,
               void(SenderBase::*signal)(ParamPack...),
               std::shared_ptr<Receiver> receiver,
               void(ReceiverBase::*callback)(ParamPack...)
    ) : _sender(std::static_pointer_cast<SignalObject>(sender)),
        _signal(static_cast<Signal_t>(signal)),
        _receiver(std::static_pointer_cast<SignalObject>(receiver)),
        _callback(static_cast<Callback_t>(callback))
    {}

    void executeCallback(ParamPack... args) {
        if (_receiver && _callback)
            (*_receiver.*_callback)(args...);
    }

    bool isSender(const std::shared_ptr<SignalObject> sender) const {
        return sender == _sender;
    }

    bool isReceiver(const std::shared_ptr<SignalObject> receiver) const {
        return receiver == _receiver;
    }

    bool isSignal(const Signal_t& signal) const {
        return signal == _signal;
    }

    bool isCallback(const Callback_t& callback) const {
        return callback == _callback;
    }

private:
    std::shared_ptr<SignalObject> _sender;
    std::shared_ptr<SignalObject> _receiver;

    const Signal_t _signal;
    const Callback_t _callback;
};

}

#endif //__CONNECTION_HPP__
