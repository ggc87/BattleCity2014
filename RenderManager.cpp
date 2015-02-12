#include "RenderManager.h"
#include "Components.h"
#include "ScreenManager.h"
using namespace entityx;
RenderManager::RenderManager(){}
RenderManager::~RenderManager()
{
    /*
    mWindow->destroy();
    mRoot->shutdown();
    delete mRoot;
    */
}

void RenderManager::loadResources()
{
    Ogre::ConfigFile cf;
    cf.load("resources.cfg");
    Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();

    Ogre::String secName, typeName, archName;
    while( seci.hasMoreElements() )
    {
        secName = seci.peekNextKey();
        Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
        Ogre::ConfigFile::SettingsMultiMap::iterator it;

        for( it = settings->begin(); it != settings->end(); ++it)
        {
            typeName = it->first;
            archName = it->second;
            std::cout << archName << " " << typeName << " " << secName << std::endl;
            Ogre::ResourceGroupManager::getSingleton().addResourceLocation(archName, typeName, secName);

        }
    }

}

void RenderManager::init(std::string windowTitle)
{
    // eventuale registrazione a events

    mRoot = new Ogre::Root();

    loadResources();

    if(!mRoot->restoreConfig())
        if(!mRoot->showConfigDialog())
            throw Ogre::Exception(0, "Errore nel caricare la configurazione", "RenderManager::init");

    mWindow = mRoot->initialise(true, windowTitle);

    mSceneMgr = mRoot->createSceneManager(Ogre::ST_GENERIC);

    mCamera = mSceneMgr->createCamera("MainCamera");

    mViewport = mWindow->addViewport(mCamera);

    mCamera->setAspectRatio(mViewport->getHeight()/mViewport->getWidth());

    mCamera->setNearClipDistance(5);

    mCamera->setFarClipDistance(300);

    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}

void RenderManager::setFrameListener(Ogre::FrameListener *listener)
{
    mRoot->addFrameListener(listener);
}
Ogre::Camera* RenderManager::getCamera(){ return mCamera; }
Ogre::Viewport* RenderManager::getViewport(){ return mViewport; }
Ogre::SceneManager* RenderManager::getSceneManager() { return mSceneMgr; }

Ogre::Root* RenderManager::getRoot(){ return mRoot; }
Ogre::RenderWindow* RenderManager::getRenderWindow(){ return mWindow; }


void RenderManager::update(double dt)
{
    ptr<EntityManager> entities = ScreenManager::getPtr()->getCurrentEntities();
    for(auto entity: entities->entities_with_components<Position, Orientation, Renderable>()){
        ptr<Renderable> node = entity.component<Renderable>();
        ptr<Position> pos = entity.component<Position>();
        ptr<Orientation> ori = entity.component<Orientation>();
        node->sceneNode->setPosition(pos->position);
        node->sceneNode->setOrientation(ori->orientation);
    }

}

void RenderManager::attachCameraTo(Ogre::SceneNode *node, Ogre::Vector3 offset)
{
    Ogre::SceneNode* child = node->createChildSceneNode("CameraNode", offset);
    mCamera->setPosition(child->getPosition() + offset);
    child->attachObject(mCamera);
    mCamera->lookAt(-offset);
}

