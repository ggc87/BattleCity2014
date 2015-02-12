#include "ShootManager.h"
#include "RenderManager.h"
#include "Components.h"
#include "Messages.h"
#include "MessageManager.h"
#include "Factory.h"
#include "ScreenManager.h"
//using namespace entityx;

ShootManager::ShootManager()
{
}


ShootManager::~ShootManager(){
    delete mRaySceneQuery;
}


void ShootManager::init()
{
    MessageManager::getPtr()->subscribe<ShootEvent>(*this);
    mRaySceneQuery = RenderManager::getPtr()->getSceneManager()->createRayQuery(Ogre::Ray());
}

void ShootManager::receive(const ShootEvent &event){
    entityx::ptr<Position> start = event.start;
    entityx::ptr<Orientation> bdirection =  event.bodyDirection;
    entityx::ptr<Orientation> tdirection =  event.turretDirection;
    Ogre::Quaternion ori= bdirection->orientation * tdirection->orientation;
    Ogre::Ray ray(start->position,-(ori).zAxis());
    mRaySceneQuery->setRay(ray);
    Ogre::RaySceneQueryResult &result = mRaySceneQuery->execute();
    Ogre::RaySceneQueryResult::iterator itr;
    Ogre::Real min = 100000;
    Ogre::String minName="";
    for (itr = result.begin(); itr != result.end(); itr++){
        if(min>itr->distance && itr->distance>0){
            min=itr->distance;
            minName=itr->movable->getParentSceneNode()->getName();
        }
      //  std::cout<<itr->movable->getParentSceneNode()->getName()<<" "<<itr->distance<<" ";
    }
    entityx::ptr<entityx::EntityManager> entities = ScreenManager::getPtr()->getCurrentEntities();
    std::string materialName;
    entityx::Entity e = event.entity;
    entityx::ptr<AI> isAi = e.component<AI>();
    if(isAi)
        materialName = "RedLaser";
    else
        materialName = "Laser";

    Factory::createProjectile(entities , start->position, ori, 600, materialName);
    std::cout<<minName<<std::endl;
   // if(minName.find(event.target)==0){
        MessageManager::getPtr()->emit<HitEvent>(minName,100);
     //   return;
    //}
    return;
}
