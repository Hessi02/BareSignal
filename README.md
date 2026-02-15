# BareSignal

A header-only implemenation of Signals & Slot mechanism.

## Usage

```cpp
class A : public BareSignal::SignalObject, std::enable_shared_from_this<A>
{
public:
    void calculate(void) {
        emit someSignal(1 + 1);
    }

private:
    void someSignal(const int number) {
       executeAllCallbacks(shared_from_this(), &A::someSignal, number);
    }
};

class B : public SignalObject
{
public:
    void handleSomeSignal(const int number) {
        std::cout << "Object B received " << number << std::endl; 
    }
};

int main(int argc, char** argv) {   
    std::shared_ptr<A> objA = std::make_shared<A>();
    std::shared_ptr<B> objB = std::make_shared<B>();

    SignalObject::connect(objA, &A::someSignal, objB, &B::handleSomeSignal);
    objA->calculate();
    SignalObject::disconnect(objA, &A::numberAdded, objC, &C::onAddNumber);
    
    return 0;
}
```
