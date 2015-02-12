#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <entityx.h>
#include <OGRE/OgreVector3.h>
#include <OGRE/OgreSceneManager.h>
#include <OGRE/OgreSceneNode.h>
#include <OGRE/OgreMovableObject.h>
#include <OGRE/OgreQuaternion.h>
#include "RenderManager.h"
#include <OIS/OIS.h>
#include <OGRE/SdkTrays.h>
#include <map>



struct Collidable : entityx::Component<Collidable>
{
    Collidable(){}
};

struct OverHeating : entityx::Component<OverHeating>
{
    OverHeating(Ogre::Real overheating,Ogre::Real shootHeating,Ogre::Real maxheating) : overheating(overheating),shootHeating(shootHeating),maxheating(maxheating){}
    Ogre::Real overheating;
    Ogre::Real shootHeating;
    Ogre::Real maxheating;
};

struct AI : entityx::Component<AI>
{
    AI()  {}

};

struct Name : entityx::Component<Name>
{
    Name() {}

    Name(std::string name) : name(name) {}

    std::string name;
};

struct Position : entityx::Component<Position>
{
    Position() : Position(0.0, 0.0, 0.0) {}

    Position(Ogre::Vector3 position) : position(position) {}

    Position(float x, float y, float z) : position(x, y, z) {}

    Ogre::Vector3 position;
	
};

struct Velocity : entityx::Component<Velocity>
{
    Velocity() : Velocity(0.0, 0.0, 0.0) {}
    Velocity(Ogre::Vector3 velocity) : velocity(velocity), direction(0,0,0) {}
    Velocity(float x, float y, float z) : velocity(x, y, z), direction(0,0,0) {}

    Ogre::Vector3 direction;
    Ogre::Vector3 velocity;
};


struct Renderable : entityx::Component<Renderable>
{
    Renderable() : sceneNode(0) {}
    Renderable(std::string nodeName)
    {
        sceneNode = RenderManager::getPtr()->getSceneManager()->getSceneNode(nodeName);
    }

    Renderable(Ogre::SceneNode* node) : sceneNode(node) {}

    Ogre::SceneNode* sceneNode;
};

struct Orientation : entityx::Component<Orientation>
{
    Orientation() : Orientation(Ogre::Radian(0), Ogre::Vector3::UNIT_Y) {}

    Orientation(Ogre::Radian angle, Ogre::Vector3 axis) : orientation(angle, axis) {}

    Orientation(Ogre::Quaternion quaternion) : orientation(quaternion) {}

    Ogre::Quaternion orientation;

};

struct AngularVelocity : entityx::Component<AngularVelocity>
{
    AngularVelocity() : AngularVelocity(0, 0, 0) {}

    AngularVelocity(float x, float y, float z) : angularVelocity(x, y, z), direction(0,0,0) {}

    AngularVelocity(Ogre::Vector3 angularVelocity) : angularVelocity(angularVelocity), direction(0,0,0){}


    Ogre::Vector3 direction;
    Ogre::Vector3 angularVelocity;
};


struct Player : entityx::Component<Player>
{
    Player();
};

struct AIDriven : entityx::Component<AIDriven>
{
    AIDriven();
};

struct Destroyable : entityx::Component<Destroyable>
{
    //Destroyable() : Destroyable(100,100) {}
    Destroyable(int health) : health(health), maxHealth(health) {}
    Destroyable(int health,int maxHealth) : health(health), maxHealth(maxHealth) {}
    int health;
    int maxHealth;
};

struct Score : entityx::Component<Score>
{
    Score() : Score(0) {}

    Score(int score) : score(score) {}

    int score;
};

struct Children : entityx::Component<Children>
{
    Children() : children() {}
    std::map<std::string, entityx::Entity> children;
};

struct LightComponent : entityx::Component<LightComponent>
{
    LightComponent() : light(0){}
    LightComponent(Ogre::Light * l) : light(l) {}
    Ogre::Light *light;
};
#endif // COMPONENTS_H
