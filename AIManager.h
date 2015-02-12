#ifndef AIMANAGER_H
#define AIMANAGER_H
#include <entityx.h>
#include "Singleton.h"
#include "Messages.h"
#include "Components.h"
#include "Ogre.h"

class AIManager : public Singleton<AIManager>
{
public:
    AIManager();
    void init();
    void update(double dt);

private:
    entityx::Entity tankPlayer;
    entityx::EventManager* events;

    void seek(entityx::ptr<Velocity> vel,entityx::ptr<AngularVelocity> angVel,entityx::ptr<Orientation> ori,Ogre::Vector3 diff,Ogre::Radian theta,double dt);
    void walk(entityx::ptr<Velocity> vel,entityx::ptr<Orientation> ori,entityx::ptr<AngularVelocity> angVel,entityx::ptr<Position> pos,double dt);
    void fire(entityx::Entity start,entityx::Entity end);

};

#endif // AIMANAGER_H
