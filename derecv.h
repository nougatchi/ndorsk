#ifndef DERECV_H
#define DERECV_H

#include <irrlicht.h>
#include <array>

class derecv : public irr::IEventReceiver
{
    public:
        virtual bool OnEvent(const irr::SEvent &event);
        void addReceiver(IEventReceiver* receiver);
        bool remReceiver(IEventReceiver* receiver);
    protected:
        irr::core::array<irr::IEventReceiver*> receivers;
    private:
};

#endif // DERECV_H
