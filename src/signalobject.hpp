#ifndef __SIGNAL_OBJECT_HPP__
#define __SIGNAL_OBJECT_HPP__

#include <vector>
#include <type_traits>

#include "connection.hpp"

namespace BareSignal 
{

#define emit

class SignalObject
{
public:
    template<class Sender,
             class SenderBase,
             class Receiver,
             class ReceiverBase,
             class... ParamPack>
    requires isDerived<SenderBase, Sender> &&
             isDerived<ReceiverBase, Receiver>
    static void connect(const std::shared_ptr<Sender> sender,
                        void(SenderBase::*signal)(ParamPack...),
                        const std::shared_ptr<Receiver> receiver,
                        void(ReceiverBase::*callback)(ParamPack...)
    ) {
        std::shared_ptr<Connection<ParamPack...>> connection = 
            std::make_shared<Connection<ParamPack...>>(
                sender,
                signal,
                receiver,
                callback
            );
        
        _connections<ParamPack...>.push_back(connection);
    }

    template<class Sender,
             class SenderBase,
             class Receiver,
             class ReceiverBase,
             class... ParamPack>
    requires isDerived<SenderBase, Sender> &&
             isDerived<ReceiverBase, Receiver>
    static void disconnect(const std::shared_ptr<Sender> sender,
                           void(SenderBase::*signal)(ParamPack...),
                           const std::shared_ptr<Receiver> receiver,
                           void(ReceiverBase::*callback)(ParamPack...)
    ) {
        std::erase_if(_connections<ParamPack...>,
            [&](const std::shared_ptr<Connection<ParamPack...>>& connection)
            {
                return connection->isSender(sender) &&
                       connection->isSignal(static_cast<typename Connection<ParamPack...>::Signal_t>(signal)) &&
                       connection->isReceiver(receiver) &&
                       connection->isCallback(static_cast<typename Connection<ParamPack...>::Callback_t>(callback));
        });
    }

    template<class Sender,
             class SenderBase,
             class... ParamPack>
    requires isDerived<SenderBase, Sender>
    static void executeAllCallbacks(std::shared_ptr<Sender> sender,
                                    void(SenderBase::*signal)(ParamPack...),
                                    ParamPack... args
    ) {
        for (const std::shared_ptr<Connection<ParamPack...>> connection : _connections<ParamPack...>) {
            if (sender && connection &&
                connection->isSender(static_pointer_cast<SignalObject>(sender)) &&
                connection->isSignal(static_cast<Connection<ParamPack...>::Signal_t>(signal))
            ) {
                connection->executeCallback(args...);
            }
        }
    }

private:
    template<class... ParamPack>
    inline static std::vector<std::shared_ptr<Connection<ParamPack...>>> _connections;
};

}

#endif //__SIGNAL_OBJECT_HPP__
