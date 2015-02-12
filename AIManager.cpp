#include "AIManager.h"
#include "MapManager.h"
#include "Components.h"
#include "PlayerManager.h"
#include "MessageManager.h"
#include "CollisionManager.h"
#include<time.h>

using namespace entityx;


AIManager::AIManager()
{
}


void AIManager::init()
{
        tankPlayer=PlayerManager::getPtr()->getPlayerEntity();
}


void  AIManager::update(double dt)
{
    tankPlayer=PlayerManager::getPtr()->getPlayerEntity();
    srand(time(0));
    if(tankPlayer.valid()){
        ptr<EntityManager> entities = ScreenManager::getPtr()->getCurrentEntities();
        for(auto entity : entities->entities_with_components<Position, AI,Orientation, Velocity, AngularVelocity>()){
            ptr<Position> pos = entity.component<Position>();
            ptr<Orientation> ori = entity.component<Orientation>();
            ptr<Velocity> vel =entity.component<Velocity>();
            ptr<AngularVelocity> angVel = entity.component<AngularVelocity>();

            //delta= (vel->velocity * vel->direction);

            ptr<Position> playerPos = tankPlayer.component<Position>();

            Ogre::Real dist = playerPos->position.distance(pos->position);
            Ogre::Vector3 diff = pos->position-playerPos->position;
            Ogre::Radian theta = (ori->orientation.zAxis()).angleBetween(diff);

            //se la distanza dal nemico è < di una soglia data e il nemico non è sotto tiro
            //punta il nemico

            if(abs(dist)<70 && theta.valueDegrees()>1){
                seek(vel,angVel,ori,diff,theta,dt);
                //return;
            }

            //se il nemico è sottotiro spara
            if(abs(dist)<36 && theta.valueDegrees()<=1){
                fire(entity,tankPlayer);
                return;
            }

            if(abs(dist)>36)
                walk(vel,ori,angVel,pos,dt);

        }
    }
}

void AIManager::seek(ptr<Velocity> vel,ptr<AngularVelocity> angleVel,ptr<Orientation> ori,Ogre::Vector3 diff,Ogre::Radian theta,double dt){
    Ogre::Quaternion newOri;
    Ogre::Quaternion qy;

    qy=Ogre::Quaternion(Ogre::Degree(theta*dt), Ogre::Vector3::UNIT_Y);
    if((ori->orientation*qy).zAxis().angleBetween(diff)>theta)
        //qy=Ogre::Quaternion(Ogre::Degree(-theta*dt), Ogre::Vector3::UNIT_Y);
        angleVel->direction=-1;
    else
        angleVel->direction=1;
    vel->direction.z=0;

    //newOri = ori->orientation*qy;

    //ori->orientation = newOri;

    return;
}

void AIManager::walk(ptr<Velocity> vel,ptr<Orientation> ori,ptr<AngularVelocity> angVel,ptr<Position> pos,double dt){
    Ogre::Vector3 delta(0,0,-1);
    Ogre::Vector3 delta2(0,0,-1);
    delta = ((vel->velocity * Ogre::Vector3(0,0,-1)) * dt);
    delta = ori->orientation * delta;
    delta2 = (((vel->velocity*10) * delta2) * dt);
    delta2 = ori->orientation * delta2;
    MapManager *map = MapManager::getPtr();

    //std::cout<<map->collide(*(pos.get()),delta,*(ori.get()),"haha")<<std::endl;
    if((map->collide(*(pos.get()),delta,*(ori.get()))==0)&&(map->collide(*(pos.get()),delta2,*(ori.get()))==0)){
        vel->direction=Ogre::Vector3(0,0,-1);
      //  angVel->direction=Ogre::Vector3(0,0,0);

        if(rand()%1000<10)
        {
            int dir=rand()%2;
            if(dir==0)
                dir=-1;
            if(angVel->direction.y==0)
                angVel->direction.y=dir;
        }
        return;
    }
    if(angVel->direction.y==0){
        vel->direction=Ogre::Vector3(0,0,0);

        int dir=rand()%10;
        if(dir<5){
            dir=-1;
        }
        else{
            dir=1;
        }

        angVel->direction.y=dir;
    }
}

void AIManager::fire(Entity start,Entity end){
    ptr<OverHeating> overH = start.component<OverHeating>();
    start.component<Velocity>()->direction.z=0;
    start.component<AngularVelocity>()->direction.y=0;
    if(overH->overheating<=0)
        return;
    else {
        start.assign<OverHeating>(overH->overheating-overH->shootHeating,overH->shootHeating,overH->maxheating);

        MessageManager::getPtr()->emit<ShootEvent>(start,start.component<Position>(),start.component<Orientation>(),start.component<Children>()->children["turret"].component<Orientation>(),"Player");

    }// if(MapManager::getPtr()->fireCollision(start.component<Position>(),start.component<Orientation>(),"PlayerTankBody"))
   //     std::cout<<"Colpito!"<<std::endl;
    return;
}
