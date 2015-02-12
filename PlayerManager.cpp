
#include "PlayerManager.h"
#include "MessageManager.h"
#include "MapManager.h"
#include "Components.h"
#include <sstream>
#include <OIS.h>

using namespace entityx;
PlayerManager::PlayerManager()
{
}

void PlayerManager::init()
{

}

void PlayerManager::setPlayerEntity(Entity player)
{
    this->player = player;
}

entityx::Entity PlayerManager::getPlayerEntity()
{
    return player;
}

void PlayerManager::handleKeyPressed(const OIS::KeyEvent &event)
{
    if(player.valid()){
        ptr<Velocity> vel = player.component<Velocity>();
        ptr<AngularVelocity> aVel = player.component<AngularVelocity>();

        switch(event.key)
        {
        case OIS::KC_W:
            vel->direction.z -= 1;
            break;
        case OIS::KC_S:
            vel->direction.z += 1;
            break;
        case OIS::KC_A:
            aVel->direction.y += 1;
            break;
        case OIS::KC_D:
            aVel->direction.y -= 1;
            break;
        }
    }
}

void PlayerManager::handleKeyReleased(const OIS::KeyEvent &event)
{
    if(player.valid()){
        ptr<Velocity> vel = player.component<Velocity>();
        ptr<AngularVelocity> aVel = player.component<AngularVelocity>();
        switch(event.key)
        {
        case OIS::KC_W:
            vel->direction.z += 1;
            break;
        case OIS::KC_S:
            vel->direction.z -= 1;
            break;
        case OIS::KC_A:
            aVel->direction.y -= 1;
            break;
        case OIS::KC_D:
            aVel->direction.y += 1;
            break;
        }
    }
}


void PlayerManager::handleMouseMoved(const OIS::MouseEvent &event)
{
    if(player.valid()){
        //Componenti di orientamento del corpo e della torretta
        ptr<Orientation> bodyOri, ori;

        //Conterrà la rotazione da bodyOri a ori
        Ogre::Quaternion rotation;
        Ogre::Quaternion newOri;
        // utilizzate per Quaternion::toAngleAxis
        Ogre::Vector3 axis;
        Ogre::Radian angle;

        // creo i quaternioni per la rotazione della torretta e del cannone
        Ogre::Quaternion qy(Ogre::Degree(-event.state.X.rel * .05), Ogre::Vector3::UNIT_Y);
        //Ogre::Quaternion qx(Ogre::Degree(-event.state.Y.rel * .05), Ogre::Vector3::UNIT_X);

        //Recupero le entità e le componenti necessarie
        Entity turret= player.component<Children>()->children["turret"];
        //Entity cannon = turret.component<Children>()->children["cannon"];

        bodyOri = player.component<Children>()->children["body"].component<Orientation>();

        ori = turret.component<Orientation>();


        //calcolo il nuovo orientamento della torretta

        newOri = ori->orientation * qy;
        // rotazione da bodyOri a ori = bodyOri^-1 * ori
        rotation = bodyOri->orientation.Inverse() * newOri;
        rotation.ToAngleAxis(angle, axis);

        // se compreso fra -30 e +30 gradi, esegui la rotazione della torretta
        if(Ogre::Math::Abs(angle.valueDegrees()) < 75)
        {
            ori->orientation = newOri;
        }

        //esegui la rotazione del cannone

        //ori = cannon.component<Orientation>();
        //ori->orientation = ori->orientation * qx;
    }
}

void PlayerManager::handleMousePressed(const OIS::MouseEvent &event, OIS::MouseButtonID id)
{

}

void PlayerManager::handleMouseReleased(const OIS::MouseEvent &event, OIS::MouseButtonID id)
{
    if(player.valid() && id == OIS::MB_Left)
        fire();
}

void PlayerManager::fire()
{
    ptr<OverHeating> overH = player.component<OverHeating>();
    if(overH->overheating<=0)
        return;
    else {
        player.assign<OverHeating>(overH->overheating-overH->shootHeating,overH->shootHeating,overH->maxheating);

        //ptr<Orientation> oriP = player.component<Orientation>();
       //ori->orientation=
       // Ogre::Quaternion quat(oriP->orientation*player.component<Children>()->children["turret"].component<Orientation>()->orientation);
        //ptr<Orientation> ori;
        //ori.

        MessageManager::getPtr()->emit<ShootEvent>(player,player.component<Position>(),player.component<Orientation>(),player.component<Children>()->children["turret"].component<Orientation>(),"CPU");

    }// if(MapManager::getPtr()->fireCollision(start.component<Position>(),start.component<Orientation>(),"PlayerTankBody"))
   //     std::cout<<"Colpito!"<<std::endl;
    return;
}
