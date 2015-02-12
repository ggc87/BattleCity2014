#include "Factory.h"
#include "Components.h"
#include "DotSceneLoader.h"
using namespace entityx;


Entity Factory::createTank(entityx::ptr<entityx::EntityManager> entityMgr, std::string prefix,Ogre::Real velocity,Ogre::Real angularVelocity ,Ogre::Vector3 overHating,int health,bool ai)
{
    DotSceneLoader loader;
    Ogre::SceneManager* sceneMgr = RenderManager::getPtr()->getSceneManager();

    loader.parseDotScene("tank.scene", "General", sceneMgr, 0, prefix);

    Ogre::SceneNode* ctl = sceneMgr->getSceneNode(prefix + "TankControl");
    Ogre::SceneNode* body = sceneMgr->getSceneNode(prefix + "TankBody");
    Ogre::SceneNode* turret = sceneMgr->getSceneNode(prefix + "TankTurret");
    Ogre::SceneNode* cannon = sceneMgr->getSceneNode(prefix +"TankCannon");

    Entity tankEmptyControl = entityMgr->create();
    Entity tankTurret = entityMgr->create();
    Entity tankBody = entityMgr->create();
    Entity tankCannon = entityMgr->create();

    tankEmptyControl.assign<Position>(ctl->getPosition());
    tankEmptyControl.assign<Orientation>(ctl->getOrientation());
    tankEmptyControl.assign<Velocity>(0, 0, velocity);
    tankEmptyControl.assign<AngularVelocity>(0, angularVelocity, 0);
    tankEmptyControl.assign<Renderable>(ctl);
    tankEmptyControl.assign<OverHeating>(overHating.x,overHating.y,overHating.z);
    tankEmptyControl.assign<Destroyable>(health,health);
    tankEmptyControl.assign<Collidable>();
    tankEmptyControl.assign<Name>(prefix);
    if(ai){
        tankEmptyControl.assign<AI>();
        Ogre::Entity *model = static_cast<Ogre::Entity*>(body->getAttachedObject(0));
        model->getSubEntity(1)->setMaterialName("Red");

        model = static_cast<Ogre::Entity*>(turret->getAttachedObject(0));
        model->getSubEntity(1)->setMaterialName("Red");

        model = static_cast<Ogre::Entity*>(cannon->getAttachedObject(0));
        model->getSubEntity(1)->setMaterialName("Red");

    }

    ptr<Children> child = tankEmptyControl.assign<Children>();
    child->children["body"] = tankBody;
    child->children["turret"] = tankTurret;
    //child->children.push_back(tankBody);
    //child->children.push_back(tankTurret);

    tankTurret.assign<Position>(turret->getPosition());
    tankTurret.assign<Orientation>(turret->getOrientation());
    tankTurret.assign<Renderable>(turret);
    child = tankTurret.assign<Children>();
    child->children["cannon"] = tankCannon;

    tankBody.assign<Position>(body->getPosition());
    tankBody.assign<Orientation>(body->getOrientation());
    tankBody.assign<Renderable>(body);

    tankCannon.assign<Position>(cannon->getPosition());
    tankCannon.assign<Renderable>(cannon);
    tankCannon.assign<Orientation>(cannon->getOrientation());


    ctl->scale(.35, .55, .35);
    return tankEmptyControl;
}


Entity Factory::createBlock(entityx::ptr<entityx::EntityManager> entityMgr, int x, int y, int z, std::string material)
{
    Entity block = entityMgr->create();
    Ogre::SceneManager* sceneMgr = RenderManager::getPtr()->getSceneManager();
    Ogre::Entity* wall = sceneMgr->createEntity("Cube.mesh");
    wall->setMaterialName(material);
    Ogre::SceneNode* wallNode = sceneMgr->getRootSceneNode()->createChildSceneNode();

    wallNode->attachObject(wall);

    block.assign<Position>(x,y,z);
    wallNode->setPosition(x,y,z);
    block.assign<Orientation>(wallNode->getOrientation());
    block.assign<Renderable>(wallNode);
    block.assign<Name>("Block");
    if(material == "Wall"){
        block.assign<Destroyable>(200);
    }

    return block;
}

entityx::Entity Factory::createProjectile(entityx::ptr<EntityManager> where, Ogre::Vector3 pos, Ogre::Quaternion ori, Ogre::Real velocity, std::string materialName)
{
    Ogre::Entity *projMesh;
    Ogre::SceneManager *sceneMgr = RenderManager::getPtr()->getSceneManager();

    projMesh = sceneMgr->createEntity("ProjectileMesh.mesh");
    projMesh->setMaterialName(materialName);

    Entity proj = where->create();

    Ogre::SceneNode *projNode = sceneMgr->getRootSceneNode()->createChildSceneNode();
    projNode->attachObject(projMesh);

    Ogre::Light *light = sceneMgr->createLight();
    if(materialName == "RedLaser")
        light->setDiffuseColour(Ogre::ColourValue(.8, .2, .2));
    else
        light->setDiffuseColour(Ogre::ColourValue(.2, .2, .8));

    light->setType(Ogre::Light::LT_POINT);
    projNode->attachObject(light);



    projNode->setPosition(pos);
    projNode->setOrientation(ori);

    //projNode->translate(0, 0, -2, Ogre::SceneNode::TS_LOCAL);

    proj.assign<Position>(projNode->getPosition());
    proj.assign<Orientation>(ori);

    proj.assign<Velocity>(0, 0, velocity);
    proj.component<Velocity>()->direction.z = -1;
    proj.assign<Renderable>(projNode);
    proj.assign<AngularVelocity>(0, 0, 10);
    proj.assign<Name>("proiettile");
    proj.assign<LightComponent>(light);
    return proj;
}

void Factory::createFloor(entityx::ptr<entityx::EntityManager> entityMgr, int sizex, int sizey, int repetition_x, int repetition_y, std::string material)
{
    Ogre::Plane plane(Ogre::Vector3::UNIT_Y, 0);
    Ogre::MeshManager::getSingletonPtr()->createPlane("floorPlane",Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
                                                      plane, sizex, sizey, 1, 1, true, 1, repetition_x, repetition_y, Ogre::Vector3::UNIT_Z);
    Ogre::SceneManager* sceneMgr = RenderManager::getPtr()->getSceneManager();

    Ogre::Entity* floorEntity = sceneMgr->createEntity("floorPlane");

    floorEntity->setMaterialName(material);

    sceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(floorEntity);
}
