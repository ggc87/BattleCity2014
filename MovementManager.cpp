#include "MovementManager.h"
#include "CollisionManager.h"
#include "Components.h"
#include "ScreenManager.h"
#include "MessageManager.h"

using namespace entityx;

MovementManager::MovementManager()
{
}


void MovementManager::init()
{

}

void MovementManager::update(double dt)
{
    Ogre::Vector3 delta;
    CollisionManager *colMgr=CollisionManager::getPtr();
    ptr<EntityManager> entities = ScreenManager::getPtr()->getCurrentEntities();
    for( auto entity : entities->entities_with_components<Position, Orientation, Velocity, AngularVelocity>()){
        ptr<Position> pos = entity.component<Position>();
        ptr<Orientation> ori = entity.component<Orientation>();
        ptr<Velocity> vel = entity.component<Velocity>();
        ptr<AngularVelocity> angVel = entity.component<AngularVelocity>();
       // ptr<Renderable> rend = entity.component<Renderable>();
        Ogre::String name = entity.component<Name>()->name;

        MessageManager::getPtr()->emit<MoveEvent>(pos->position.x, pos->position.y, pos->position.z);
        if(angVel){
            delta = (angVel->angularVelocity * angVel->direction) * dt;
            Ogre::Quaternion qx(Ogre::Degree(delta.x), Ogre::Vector3::UNIT_X);
            Ogre::Quaternion qy(Ogre::Degree(delta.y), Ogre::Vector3::UNIT_Y);
            Ogre::Quaternion qz(Ogre::Degree(delta.z), Ogre::Vector3::UNIT_Z);

            //Orientation newOri=ori->orientation * qx * qy * qz;
            //if(colMgr->collide(*(pos.get()),delta,newOri,name)==false)

                   ori->orientation = (ori->orientation * qx * qy * qz);
            //std::cout << "Arrotamento: " << delta << std::endl;
            //std::cout << "Quaterniono: " << ori->orientation << std::endl;
        }


        if(vel){
            delta = ((vel->velocity * vel->direction) * dt);
            delta = ori->orientation * delta;
            //sposta se non ci sono collisioni

            bool collided = colMgr->collide(*(pos.get()),delta,*(ori.get()),name);

            if(!collided)
                pos->position = pos->position + delta;

            else if(name == "proiettile"){
                    ptr<Renderable> rend = entity.component<Renderable>();

                    Ogre::MovableObject* light = entity.component<LightComponent>()->light;
                    RenderManager::getPtr()->getSceneManager()->destroyMovableObject(light);

                    RenderManager::getPtr()->getSceneManager()->destroySceneNode(rend->sceneNode->getName());
                    entity.destroy();
            }
        }
        //std::cout << "Spostamento: " << delta << std::endl;


    }
}

