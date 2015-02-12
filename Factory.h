#ifndef FACTORY_H
#define FACTORY_H
#include <entityx.h>
#include <Ogre.h>
#include "Components.h"

class Factory
{
public:
    static entityx::Entity createTank(entityx::ptr<entityx::EntityManager> where, std::string prefix,Ogre::Real velocity,Ogre::Real angularVelocity ,Ogre::Vector3 overHeating,int health,bool ai);
    static entityx::Entity createBlock(entityx::ptr<entityx::EntityManager> where, int x, int y, int z, std::string material);
    static void createFloor(entityx::ptr<entityx::EntityManager> where, int sizex, int sizey, int repetition_x, int repetition_y, std::string material);
    static entityx::Entity createProjectile(entityx::ptr<entityx::EntityManager> where, Ogre::Vector3 pos, Ogre::Quaternion ori, Ogre::Real velocity, std::string materialName);
};

#endif // FACTORY_H
