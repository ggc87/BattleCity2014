#ifndef RENDERMANAGER_H
#define RENDERMANAGER_H
#include "Singleton.h"
#include <entityx.h>
#include <map>
#include<Ogre.h>

class RenderManager : public Singleton<RenderManager>
{
public:
    RenderManager();
    ~RenderManager();
    void init(std::string windowTitle);
    void update(double dt);
    void setFrameListener(Ogre::FrameListener *listener);
    void attachCameraTo(Ogre::SceneNode* node, Ogre::Vector3 offset);
    Ogre::Root* getRoot();
    Ogre::RenderWindow* getRenderWindow();
    Ogre::Camera* getCamera();
    Ogre::Viewport* getViewport();
    Ogre::SceneManager* getSceneManager();


protected:
    void loadResources();

private:
    Ogre::Root* mRoot;
    Ogre::RenderWindow* mWindow;
    Ogre::Camera* mCamera;
    Ogre::Viewport* mViewport;
    Ogre::SceneManager* mSceneMgr;

};

#endif // RENDERMANAGER_H
