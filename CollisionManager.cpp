#include "CollisionManager.h"
#include "MapManager.h"

CollisionManager::CollisionManager()
{
}



bool CollisionManager::collide(Position pos, Ogre::Vector3 delta,Orientation ori, Ogre::String name)
{
    MapManager *map = MapManager::getPtr();
    if(map->collide(pos,delta,ori)!=0)
        return true;

    entityx::ptr<entityx::EntityManager> entities = ScreenManager::getPtr()->getCurrentEntities();

    for( auto entity : entities->entities_with_components<Position, Orientation, Velocity, AngularVelocity,Name>()){
        entityx::ptr<Position> pos0 = entity.component<Position>();
        Ogre::String name0 = entity.component<Name>()->name;
        if(name0.compare(name)!=0){
            Ogre::Real dist=pos0->position.distance(pos.position+delta);
            if( dist < 1 && dist > 0)
                return true;
        }
    }

    return false;
}
