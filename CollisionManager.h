#ifndef COLLISIONMANAGER_H
#define COLLISIONMANAGER_H

#include "Singleton.h"
#include "Components.h"
#include <entityx.h>

class CollisionManager :  public Singleton<CollisionManager>
{
public:
    CollisionManager();
    static bool collide(Position pos,Ogre::Vector3 delta,Orientation ori,Ogre::String name);

};

#endif // COLLISIONMANAGER_H
