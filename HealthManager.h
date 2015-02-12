#ifndef HEALTHMANAGER_H
#define HEALTHMANAGER_H

#include <entityx.h>
#include "Singleton.h"
#include "Messages.h"

class HealthManager : public Singleton<HealthManager>, public entityx::Receiver<HealthManager>
{
public:
    HealthManager();
    void init();
    void receive(const HitEvent &event);

protected:
    entityx::ptr<entityx::EntityManager> entities;
};

#endif // HEALTHMANAGER_H
