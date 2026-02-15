#include <catch2/catch_test_macros.hpp>
#include "../src/signalobject.hpp"

class HelperClassA : public BareSignal::SignalObject, 
                     public std::enable_shared_from_this<HelperClassA>
{
public:
    void someSignal() {
        calledFlag = true;
        executeAllCallbacks(shared_from_this(), &HelperClassA::someSignal);
    }

    void handleSignal() {
        signalReceivedFlag = true;
    }

    bool signalReceivedFlag = false;
    bool calledFlag = false;
};

class HelperClassB : public BareSignal::SignalObject
{
public:
    void handleSignal() {
        signalReceivedFlag = true;
    }

    bool signalReceivedFlag = false;
};


TEST_CASE("Signal received within same class", "[signal]") {
    auto sender = std::make_shared<HelperClassA>();
    auto receiver = std::make_shared<HelperClassA>();

    SECTION("Connected") {
        BareSignal::SignalObject::connect(sender, 
                                          &HelperClassA::someSignal, 
                                          receiver, 
                                          &HelperClassA::handleSignal);
        
        sender->someSignal();

        REQUIRE(sender->calledFlag);
        REQUIRE(receiver->signalReceivedFlag);

        BareSignal::SignalObject::disconnect(sender, 
                                             &HelperClassA::someSignal, 
                                             receiver, 
                                             &HelperClassA::handleSignal);
    }

    SECTION("Disconnected") {
        BareSignal::SignalObject::connect(sender, 
                                          &HelperClassA::someSignal, 
                                          receiver, 
                                          &HelperClassA::handleSignal);

        BareSignal::SignalObject::disconnect(sender, 
                                             &HelperClassA::someSignal, 
                                             receiver, 
                                             &HelperClassA::handleSignal);

        sender->someSignal();

        REQUIRE(sender->calledFlag);
        REQUIRE_FALSE(receiver->signalReceivedFlag);
    }
}

TEST_CASE("Signal received across different classes", "[signal]") {
    auto sender = std::make_shared<HelperClassA>();
    auto receiver = std::make_shared<HelperClassB>();

    SECTION("Connected") {
        BareSignal::SignalObject::connect(sender, 
                                          &HelperClassA::someSignal, 
                                          receiver, 
                                          &HelperClassB::handleSignal);
        
        sender->someSignal();

        REQUIRE(sender->calledFlag);
        REQUIRE(receiver->signalReceivedFlag);

        BareSignal::SignalObject::disconnect(sender, 
                                             &HelperClassA::someSignal, 
                                             receiver, 
                                             &HelperClassB::handleSignal);
    }

    SECTION("Disconnected") {
        BareSignal::SignalObject::connect(sender, 
                                          &HelperClassA::someSignal, 
                                          receiver, 
                                          &HelperClassB::handleSignal);

        BareSignal::SignalObject::disconnect(sender, 
                                             &HelperClassA::someSignal, 
                                             receiver, 
                                             &HelperClassB::handleSignal);

        sender->someSignal();

        REQUIRE(sender->calledFlag);
        REQUIRE_FALSE(receiver->signalReceivedFlag);
    }
}
