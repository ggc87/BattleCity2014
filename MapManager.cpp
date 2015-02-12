#include "MapManager.h"
#include "Factory.h"
#include "Components.h"
#include "MovementManager.h"
#include "PlayerManager.h"
#include "RenderManager.h"
#include <stdlib.h>
#include <time.h>

using namespace entityx;
using namespace std;

MapManager::MapManager()
{
    mRaySceneQuery = RenderManager::getPtr()->getSceneManager()->createRayQuery(Ogre::Ray());
}

void MapManager::init(){
    //void receive(const KeyPressedEvent &event);
    //events->subscribe<KeyReleasedEvent>(*this);


    vector < vector < bool > > maze;
    //init_maze(maze);
    srand(time(0));
    int toAdd=rand()%2+8;
    for(int i=0;i<62;i+=toAdd){
        for(int j=0;j<62;j+=toAdd)
            maze_generator(maze,7,i,j);
         toAdd=rand()%2+8;
    }
   //print_maze();
   /* init_maze(maze);
    maze_generator(indeks, maze, backtrack_x, backtrack_y, 0, 0, 10, 0);
    print_maze(maze,3,20);
    init_maze(maze);
    maze_generator(indeks, maze, backtrack_x, backtrack_y, 0, 0, 10, 0);
    print_maze(maze,4,40);
    init_maze(maze);
    maze_generator(indeks, maze, backtrack_x, backtrack_y, 0, 0, 10, 0);
    print_maze(maze,3,-40);*/


}

void MapManager::init_maze(int maze[MAX][MAX]){
    for(int a = 0; a < MAX; a++)
        {
            for(int b = 0; b < MAX; b++)
            {
                if(a % 2 == 0 || b % 2 == 0)
                    maze[a][b] = 1;
                else
                    maze[a][b] = PATH;
            }
        }
}

void MapManager::maze_generator(vector < vector < bool > > maze,int size,int shiftx,int shiftz){


     const int maze_size_x=size;
     const int maze_size_y=size;
     list < pair < int, int> > drillers;

     maze.resize(maze_size_y);
     for (size_t y=0;y<maze_size_y;y++)
             maze[y].resize(maze_size_x);

     for (size_t x=0;x<maze_size_x;x++)
             for (size_t y=0;y<maze_size_y;y++)
                     maze[y][x]=false;

     drillers.push_back(make_pair(maze_size_x/2,maze_size_y/2));
     while(drillers.size()>0)
     {
             list < pair < int, int> >::iterator m,_m,temp;
             m=drillers.begin();
             _m=drillers.end();
             while (m!=_m)
             {
                     bool remove_driller=false;
                     switch(rand()%4)
                     {
                     case 0:
                             (*m).second-=2;
                             if ((*m).second<0 || maze[(*m).second][(*m).first])
                             {
                                     remove_driller=true;
                                     break;
                             }
                             maze[(*m).second+1][(*m).first]=true;
                             break;
                     case 1:
                             (*m).second+=2;
                             if ((*m).second>=maze_size_y || maze[(*m).second][(*m).first])
                             {
                                     remove_driller=true;
                                     break;
                             }
                             maze[(*m).second-1][(*m).first]=true;
                             break;
                     case 2:
                             (*m).first-=2;
                             if ((*m).first<0 || maze[(*m).second][(*m).first])
                             {
                                     remove_driller=true;
                                     break;
                             }
                             maze[(*m).second][(*m).first+1]=true;
                             break;
                     case 3:
                             (*m).first+=2;
                             if ((*m).first>=maze_size_x || maze[(*m).second][(*m).first])
                             {
                                     remove_driller=true;
                                     break;
                             }
                             maze[(*m).second][(*m).first-1]=true;
                             break;
                     }
                     if (remove_driller)
                             m = drillers.erase(m);
                     else
                     {
                             drillers.push_back(make_pair((*m).first,(*m).second));
                             // uncomment the line below to make the maze easier
                             // if (rand()%2)
                             drillers.push_back(make_pair((*m).first,(*m).second));

                             maze[(*m).second][(*m).first]=true;
                             ++m;
                     }
             }
     }
     for (int y=0;y<maze_size_y;y++)
                for (int x=0;x<maze_size_x;x++)
                {
                        if (maze[y][x]==true){
                            //Factory::getPtr()->createBlock((x+shiftx-(125/4))*4, 0, (-125/4+y+shiftz)*4, "Wall");
                            finalMaze[y+shiftz][x+shiftx]=1;
                           // std::cout<<(x+shiftx-(125/4))*4<<" "<<(-125/4+y+shiftz)*4<<std::endl;
                        }




                }
}

int MapManager::is_closed(int maze[MAX][MAX], int x, int y){
    if(maze[x - 1][y]  == WALL
          && maze[x][y - 1] == WALL
          && maze[x][y + 1] == WALL
          && maze[x + 1][y] == WALL
       )
           return 1;

       return 0;
}

void MapManager::print_maze(entityx::ptr<entityx::EntityManager> entities){
    int div=8;
    int _shiftx=0;
    int _shifty=0;
    int pMetalTreshold = 15; // soglia scelta metallo invece di mattoni
    int p = 0;
    std::string material;
  for(int i=0;i<62;i++){
      finalMaze[0][i]=1;
      finalMaze[61][i]=1;
      finalMaze[i][0]=1;
      finalMaze[i][61]=1;
  }
  finalMaze[0][0]=2;
  for(int i=0;i<MAX;i++){
      for(int j=0;j<MAX;j++){
          if(finalMaze[i][j]==1){
              p = rand()%100;
              if(i == 0 || i == MAX -1 || j == 0 || j == MAX -1 || p < pMetalTreshold)
                  material = "Metal";
              else
                  material = "Wall";


              Factory::createBlock(entities, (j-125/4)*4, 0, (i-125/4)*4, material);

              //std::cout<<finalMaze[i][j]<<std::endl;
          }
      }
  }

}



int MapManager::collide(Position pos,Ogre::Vector3 delta,Orientation ori){
    delta=delta*4;
  //  int newx=x+delta.x;
  //  int newz=y+delta.z;
    Position newPos = pos.position+delta-ori.orientation.zAxis();
   // std::cout<<newx<<" "<<newz<<" "<<(newx+125)/4<<" "<<(newz+125)/4<<std::endl;
    if((newPos.position.x+125)/4<0 || (newPos.position.x+125)/4>MAX)
        return 0;
    if((newPos.position.z+125)/4<0 || (newPos.position.z+125)/4>MAX)
        return 0;
    if(finalMaze[int((newPos.position.z+125)/4)][int(newPos.position.x+125)/4]==1)
        return 1;
    newPos = pos.position+delta-2*ori.orientation.xAxis();
    if(finalMaze[int((newPos.position.z+125)/4)][int(newPos.position.x+125)/4]==1)
        return 1;
    newPos = pos.position+delta+2*ori.orientation.xAxis();
    if(finalMaze[int((newPos.position.z+125)/4)][int(newPos.position.x+125)/4]==1)
        return 1;


    return 0;
}


/*bool MapManager::fireCollision(entityx::ptr<Position> start, entityx::ptr<Orientation> direction, Ogre::String name){
  /*  Ogre::Ray ray(start->position,-direction->orientation.zAxis());
    mRaySceneQuery->setRay(ray);
    Ogre::RaySceneQueryResult &result = mRaySceneQuery->execute();
    Ogre::RaySceneQueryResult::iterator itr;
    //itr = result.begin();
    //itr++;
    Ogre::Real min = 100000;
    Ogre::String minName="";
    for (itr = result.begin(); itr != result.end(); itr++){
        if(min>itr->distance && itr->distance>0){
            min=itr->distance;
            minName=itr->movable->getParentSceneNode()->getName();
        }
      //  std::cout<<itr->movable->getParentSceneNode()->getName()<<" "<<itr->distance<<" ";
    }
    std::cout<<minName<<std::endl;
    //if(name.compare(minName)==0)
      //      return true;
    std::cout<<std::endl;
    return false;
}
*/

bool MapManager::isFree(Ogre::Vector3 pos)
{

    if(finalMaze[int((pos.z+125)/4)][int(pos.x+125)/4]==1)
        return false;
    return true;
}

void MapManager::deletePosition(Ogre::Vector3 pos)
{
    finalMaze[int((pos.z+125)/4)][int(pos.x+125)/4] = 0;
}

Ogre::Vector3 MapManager::getFreePos(){
    int x,z;
    do{

        x=(rand()%220)-110;
        z=(rand()%220)-110;
        std::cout<<x<<"-"<<z<<std::endl;
    }while(!isFree(Ogre::Vector3(x,0,z)));

    return Ogre::Vector3(x,1,z);
}
