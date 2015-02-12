#ifndef MESSAGEMANAGER_H
#define MESSAGEMANAGER_H

#include <entityx.h>
#include "Singleton.h"

class MessageManager : public Singleton<MessageManager>
{
public:
    MessageManager()
    {
        events = entityx::EventManager::make();
    }

    entityx::ptr<entityx::EventManager> getEventMgr() { return events; }

    template <typename E, typename Receiver>
    void subscribe(Receiver &receiver)
    {
        events->subscribe<E>(receiver);
    }

    template <typename E, typename ... Args>
    void emit(Args && ... args) {
        events->emit<E>(args ...);
    }

private:
    entityx::ptr<entityx::EventManager> events;
};

#endif // MESSAGEMANAGER_H
