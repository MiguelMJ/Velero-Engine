#include "Event.hpp"
#include "EventListener.hpp"
#include "EventSystem.hpp"
#include <iostream>

using namespace ge;
using namespace std;

class SumEvent : public Event{
public:
    int op1, op2;
};
class DiffEvent : public Event{
public:
    int op1, op2;
};
class EchoEvent : public Event{
public:
    std::string msg;
};
class Echoer : public EventListener{
public:
    Echoer(){
        EventListener::m_channels.insert("mathchannel");
        EventListener::m_channels.insert("echochannel");
    }
    void notify(const Event* e, std::type_index ti, size_t ch){
        if(ti == typeid(SumEvent)){
            auto se = static_cast<const SumEvent*>(e);
            cout << "Echoer: SumEvent " << se->op1 << "+" << se->op2 << endl;
        }else if(ti == typeid(DiffEvent)){
            auto se = static_cast<const DiffEvent*>(e);
            cout << "Echoer: DiffEvent " << se->op1 << "-" << se->op2 << endl;
        }else{
            auto ee = static_cast<const EchoEvent*>(e);
            cout << "Echoer: EchoEvent " << ee->msg << endl;
        }
    }
};
class Mather : public EventListener{
public:
    Mather(){
        EventListener::m_channels.insert("mathchannel");
    }
    void notify(const Event* e, std::type_index ti, size_t ch){
        if(ti == typeid(SumEvent)){
            auto se = static_cast<const SumEvent*>(e);
            cout << "Mather: SumEvent " << se->op1 + se->op2 << endl;
        }else if(ti == typeid(DiffEvent)){
            auto se = static_cast<const DiffEvent*>(e);
            cout << "Mather: DiffEvent " << se->op1 - se->op2 << endl;
        }
    }
};

int main(int argc, char** argv){
    size_t mch = EventSystem::getChannelId("mathchannel");
    size_t ech = EventSystem::getChannelId("echochannel");
    
    Mather m;
    Echoer e;
    Echoer er;
    
    EventSystem::registerListener(&m);
    EventSystem::registerListener(&e);
    EventSystem::registerListener(&er);
    EventSystem::unregisterListener(&er);
    EventSystem::subscribe(ech, &er);
    
    SumEvent se;
    DiffEvent de;
    EchoEvent ee;
    
    EventSystem::publish(mch, se);
    EventSystem::publish(mch, de);
    EventSystem::publish(ech, ee);
    
    se.op1 = de.op1 = 9;
    se.op2 = de.op2 = 4;
    ee.msg = "Hola evento";
    
    EventSystem::dispatch();
    
    
    
    
    
}
