#ifndef MAPMANAGER_H
#define MAPMANAGER_H

#define MAX 62  // 30 * 2 + 1
#define CELL 65000  // 30 * 30
#define WALL 1
#define PATH 0

#include<time.h>
#include<entityx.h>
#include "Singleton.h"
#include "Components.h"
#include <time.h>
#include <vector>
#include <list>
#include <Ogre.h>
#include "ScreenManager.h"

using namespace std;

class MapManager : public Singleton<MapManager>, public entityx::Receiver<MapManager>
{
    friend class PlayScreen;
public:
    MapManager();
    void init();
    void update(entityx::ptr<entityx::EntityManager>);
    int collide(Position pos,Ogre::Vector3 delta,Orientation ori);
  //  bool fireCollision(entityx::ptr<Position> start,entityx::ptr<Orientation>direction,Ogre::String name);
    bool isFree(Ogre::Vector3 pos);
    void deletePosition(Ogre::Vector3 pos);
    Ogre::Vector3 getFreePos();
protected:
    Ogre::RaySceneQuery *mRaySceneQuery;


    void init_maze(int maze[MAX][MAX]);
    void maze_generator(vector < vector < bool > > maze,int size,int shiftx,int shiftz);
    int is_closed(int maze[MAX][MAX], int x, int y);
    void print_maze(entityx::ptr<entityx::EntityManager>);

    int finalMaze[MAX][MAX];

};

#endif // MAPMANAGER_H
