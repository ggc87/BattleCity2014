#include "HealthManager.h"
#include "Components.h"
#include "MessageManager.h"
#include "ScreenManager.h"
#include "MapManager.h"

using namespace entityx;

HealthManager::HealthManager()
{
}

void HealthManager::init()
{
    MessageManager::getPtr()->subscribe<HitEvent>(*this);
}

void HealthManager::receive(const HitEvent &event)
{
    std::cout<<"ricevuto"<<std::endl;
    entityx::ptr<entityx::EntityManager> entities = ScreenManager::getPtr()->getCurrentEntities();
    for(auto entity : entities->entities_with_components<Destroyable, Renderable>())
    {
        ptr<Destroyable> points = entity.component<Destroyable>();
        std::string entName = entity.component<Name>()->name;
        Ogre::String name;

        if(entName != "Block"){
            ptr<Children> child = entity.component<Children>();
            name = child->children["body"].component<Renderable>()->sceneNode->getName();
        }else{
            ptr<Renderable> rend = entity.component<Renderable>();
            name = rend->sceneNode->getName();
        }

        std::cout<<"name: "<<name<<std::endl;


        int max = points->maxHealth;

        if(name.compare(event.targetName)==0)
        {
            points->health-=event.points;
            if(points->health<=0)
            {
                MessageManager::getPtr()->emit<ObjectDestroyed>(entity);
                if(entity.valid()){
                    if(entity.component<Name>()->name == "Block")
                    {
                        MapManager::getPtr()->deletePosition(entity.component<Position>()->position);
                        RenderManager::getPtr()->getSceneManager()->destroySceneNode(entity.component<Renderable>()->sceneNode);
                        entity.destroy();
                    }else{
                        entity.assign<Position>(MapManager::getPtr()->getFreePos());
                        entity.assign<Destroyable>(max,max);
                    }
                }
                return;
            }else{
                std::cout<<"punti"<<points->health<<std::endl;
                entity.assign<Destroyable>(points->health,max);
            }
            return;
        }

    }
    return;
}

