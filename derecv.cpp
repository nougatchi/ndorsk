#include "derecv.h"

bool derecv::OnEvent(const irr::SEvent &event) {
    for (unsigned int i = 0; i < receivers.size(); ++i) {
        if(receivers[i]->OnEvent(event)) {
            return true;
        }
    }
    return false;
}

void derecv::addReceiver(IEventReceiver* receiver) {
    receivers.push_back(receiver);
}

bool derecv::remReceiver(IEventReceiver* receiver) {
    for (unsigned int i = 0; i < receivers.size(); ++i)
    {
        if (receivers[i] == receiver)
        {
            receivers.erase(i);
            return true;
        }
    }
    return false;
}
