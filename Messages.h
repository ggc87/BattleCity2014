#ifndef MESSAGES_H
#define MESSAGES_H
#include <OIS.h>
#include <entityx.h>
#include "Components.h"


struct HitEvent : public entityx::Event<HitEvent>
{
    HitEvent(Ogre::String name,int points) : targetName(name),points(points){}
    Ogre::String targetName;
    int points;
};

struct ShootEvent : public entityx::Event<ShootEvent>
{
    ShootEvent(entityx::Entity entity,entityx::ptr<Position> pos,entityx::ptr<Orientation> bdir,entityx::ptr<Orientation> tdir,Ogre::String target) :entity(entity),start(pos),bodyDirection(bdir),turretDirection(tdir), target(target){}

    entityx::ptr<Position> start;
    entityx::ptr<Orientation> bodyDirection;
    entityx::ptr<Orientation> turretDirection;
    entityx::Entity entity;
    Ogre::String target;
};

struct KeyPressedEvent : public entityx::Event<KeyPressedEvent>
{
    KeyPressedEvent(OIS::KeyEvent event) : event(event){}

    OIS::KeyEvent event;
};

struct KeyReleasedEvent : public entityx::Event<KeyReleasedEvent>
{
    KeyReleasedEvent(OIS::KeyEvent event) : event(event){}

    OIS::KeyEvent event;
};


struct MouseMovedEvent : public entityx::Event<MouseMovedEvent>
{
    MouseMovedEvent(OIS::MouseEvent event) : event(event){}
    OIS::MouseEvent event;
};

struct MousePressedEvent : public entityx::Event<MousePressedEvent>
{
    MousePressedEvent(OIS::MouseEvent event, OIS::MouseButtonID buttonID) : event(event), buttonID(buttonID){}

    OIS::MouseButtonID buttonID;
    OIS::MouseEvent event;
};

struct MouseReleasedEvent : public entityx::Event<MouseReleasedEvent>
{
    MouseReleasedEvent(OIS::MouseEvent event, OIS::MouseButtonID buttonID) : event(event), buttonID(buttonID){}

    OIS::MouseButtonID buttonID;
    OIS::MouseEvent event;
};

struct MoveEvent : public entityx::Event<MoveEvent>
{
    MoveEvent(int x, int y, int z) : x(x), y(y), z(z) {}
    int x,y,z;
};

struct ShutDownEvent : public entityx::Event<ShutDownEvent>
{
    ShutDownEvent() {}
};

struct ObjectDestroyed : public entityx::Event<ObjectDestroyed>
{
    ObjectDestroyed(entityx::Entity obj) : object(obj){}

    entityx::Entity object;
};

struct GameStarted : public entityx::Event<GameStarted>
{
    GameStarted() {}
};

#endif // MESSAGES_H
