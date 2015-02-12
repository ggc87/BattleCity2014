#include "HeatingManager.h"
#include "ScreenManager.h"

HeatingManager::HeatingManager()
{
}


void HeatingManager::init()
{

}

void HeatingManager::update()
{
    entityx::ptr<entityx::EntityManager> entities = ScreenManager::getPtr()->getCurrentEntities();
    for(auto entity : entities->entities_with_components<OverHeating>())
    {
        entityx::ptr<OverHeating> overH = entity.component<OverHeating>();
        if(overH->overheating<overH->maxheating){
            entity.assign<OverHeating>(overH->overheating+1,overH->shootHeating,overH->maxheating);

        }
    }
}
