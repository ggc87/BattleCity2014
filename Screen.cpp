#include "Screen.h"
#include "RenderManager.h"
#include "InputManager.h"
#include "ScreenManager.h"
#include "Messages.h"
#include "MapManager.h"
#include "PlayerManager.h"
#include "Factory.h"
#include "ScreenManager.h"
#include "Components.h"
#include "ScoreManager.h"

using namespace OgreBites;
using namespace entityx;
/****** SCREEN BASE CLASS ******/
void Screen::mouseMoved(const OIS::MouseEvent &evt)
{
    if(mTrayMgr)
        mTrayMgr->injectMouseMove(evt);
}

void Screen::mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
    if(mTrayMgr)
        mTrayMgr->injectMouseDown(evt, id);
}

void Screen::mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
    if(mTrayMgr){
        mTrayMgr->injectMouseUp(evt, id);
    }
}


/****** MENU SCREEN******/
void MenuScreen::enter()
{
    mTrayMgr = new SdkTrayManager("MenuTray",RenderManager::getPtr()->getRenderWindow(),
                                  InputManager::getPtr()->getMouse(), this);
    mEntities = entityx::EntityManager::make(MessageManager::getPtr()->getEventMgr());

    //Ogre::Overlay *bclogo = Ogre::OverlayManager::getSingletonPtr()->getByName("BattlecityLogo");

    mTrayMgr->createDecorWidget(TL_TOP, "BCLOGO", "BattlecityLogo");

    mTrayMgr->createButton(TL_CENTER, "NewGameButton", "Nuova Partita", 200);
    mTrayMgr->createButton(TL_CENTER, "ExitButton", "Esci", 200);
    change = false;

    MessageManager::getPtr()->emit<GameStarted>();
}

void MenuScreen::pause()
{
    mTrayMgr->hideAll();
}

void MenuScreen::resume()
{
    mTrayMgr->showAll();
}

void MenuScreen::leave()
{
    mTrayMgr->destroyAllWidgets();
    delete mTrayMgr;
    mTrayMgr = 0;
}

void MenuScreen::buttonHit(Button *button)
{
    std::string buttonName = button->getName();

    if(buttonName == "ExitButton"){
        mTrayMgr->showYesNoDialog("Quit the Game?","Do you want to quit?");
    }

    if(buttonName == "NewGameButton"){
        change = true;
    }

}

void MenuScreen::update(double dt)
{
    if(change)
        ScreenManager::getPtr()->changeScreen(PlayScreen::getPtr());
}

void MenuScreen::yesNoDialogClosed(const Ogre::DisplayString &question, bool yesHit)
{
    if( question == "Do you want to quit?" && yesHit)
        MessageManager::getPtr()->emit<ShutDownEvent>();
}

void MenuScreen::keyPressed(const OIS::KeyEvent &evt)
{

}

void MenuScreen::keyReleased(const OIS::KeyEvent &evt)
{

}



/****** PlayScreen ******/
void PlayScreen::enter()
{
    mEntities = entityx::EntityManager::make(MessageManager::getPtr()->getEventMgr());

    mTrayMgr = new SdkTrayManager("PlayTray",RenderManager::getPtr()->getRenderWindow(),
                                  InputManager::getPtr()->getMouse(), this);

    // DRAW GUI
    Ogre::StringVector infoPanelStr;
    unsigned int screenWidth = RenderManager::getPtr()->getRenderWindow()->getWidth();
    infoPanelStr.push_back("Score");
    mInfoPanel = mTrayMgr->createParamsPanel(TL_TOP, "InfoPanel", screenWidth * .20, infoPanelStr);
    mHealthBar = mTrayMgr->createProgressBar(TL_BOTTOMLEFT, "HealthBar","Health", screenWidth * .25, 50);
    mHeatBar = mTrayMgr->createProgressBar(TL_BOTTOMRIGHT, "HeatBar", "Heat", screenWidth * .25, 50);
    mHealthBar->setComment(" ");
    mHeatBar->setComment(" ");

    mInfoPanel->setParamValue("Score",Ogre::StringConverter::toString(0));
    mHealthBar->setProgress(1);
    mHeatBar->setProgress(1);

    mTrayMgr->hideCursor();


    Ogre::Light* luce;

    luce = RenderManager::getPtr()->getSceneManager()->createLight();
    luce->setType(Ogre::Light::LT_POINT);
    luce->setPowerScale(2.0);
    luce->setPosition(-125, 10, -125);

    luce = RenderManager::getPtr()->getSceneManager()->createLight();
    luce->setType(Ogre::Light::LT_POINT);
    luce->setPowerScale(2.0);
    luce->setPosition(125, 10, -125);

    luce = RenderManager::getPtr()->getSceneManager()->createLight();
    luce->setType(Ogre::Light::LT_POINT);
    luce->setPowerScale(2.0);
    luce->setPosition(-125, 10, 125);

    luce = RenderManager::getPtr()->getSceneManager()->createLight();
    luce->setType(Ogre::Light::LT_POINT);
    luce->setPowerScale(2.0);
    luce->setPosition(125, 10, 125);

    RenderManager::getPtr()->getSceneManager()->setAmbientLight(Ogre::ColourValue::White);




    Entity tank1 = Factory::createTank(mEntities, "Player", 45, 65, Ogre::Vector3(100, 20, 100), 1000, false);

    tank1.assign<Position>(MapManager::getPtr()->getFreePos());
    tank1.assign<Destroyable>(1000);

    PlayerManager::getPtr()->setPlayerEntity(tank1);

    entityx::Entity tankcpu0;

    for (int i = 0; i < 5; ++i) {
        char prefix[15];
        sprintf(prefix,"CPU%d",i);
        tankcpu0 = Factory::createTank(mEntities, prefix,30,60,Ogre::Vector3(100,60,100),500, true);
        tankcpu0.assign<Position>(MapManager::getPtr()->getFreePos());
    }

    entityx::ptr<Children> children = tank1.component<Children>();
    entityx::ptr<Renderable> cameraParent = children->children["turret"].component<Renderable>();
    RenderManager::getPtr()->attachCameraTo(cameraParent->sceneNode, Ogre::Vector3(0,4.5,20));

    //RenderManager::getPtr()->getSceneManager()->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);

    Factory::createFloor(mEntities, 250, 250, 20, 20, "Floor");
    MapManager::getPtr()->print_maze(mEntities);

}

void PlayScreen::pause()
{
    mTrayMgr->hideAll();
}

void PlayScreen::resume()
{
    mTrayMgr->showAll();
    mTrayMgr->hideCursor();
}

void PlayScreen::leave()
{
    mEntities->destroy_all();
    RenderManager::getPtr()->getSceneManager()->getRootSceneNode()->removeAndDestroyAllChildren();
    RenderManager::getPtr()->getSceneManager()->destroyAllLights();
    mTrayMgr->destroyAllWidgets();
    delete mTrayMgr;
    mTrayMgr = 0;
}

void PlayScreen::keyPressed(const OIS::KeyEvent &evt)
{
    PlayerManager::getPtr()->handleKeyPressed(evt);
}

void PlayScreen::keyReleased(const OIS::KeyEvent &evt)
{
    if(evt.key == OIS::KC_ESCAPE)
        ScreenManager::getPtr()->pushScreen(PauseScreen::getPtr());
    else
        PlayerManager::getPtr()->handleKeyReleased(evt);
}

void PlayScreen::update(double dt)
{
    Entity player = PlayerManager::getPtr()->getPlayerEntity();
    if(player.valid()){
        ptr<Destroyable> dest = player.component<Destroyable>();
        ptr<OverHeating> heat = player.component<OverHeating>();
        double healthPercent = (double)dest->health / dest->maxHealth;
        double heatPercent = 1 - (heat->overheating / heat->maxheating);
        mHealthBar->setComment(Ogre::StringConverter::toString(dest->health));
        mHeatBar->setComment(Ogre::StringConverter::toString((int)(heatPercent * 100)));
        mHealthBar->setProgress(healthPercent);
        mHeatBar->setProgress(heatPercent);
    }
    mInfoPanel->setParamValue("Score",
                              Ogre::StringConverter::toString(ScoreManager::getPtr()->getScore()));
}

void PlayScreen::mouseMoved(const OIS::MouseEvent &evt)
{
    Screen::mouseMoved(evt);
    PlayerManager::getPtr()->handleMouseMoved(evt);
}

void PlayScreen::mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
    Screen::mousePressed(evt, id);
    PlayerManager::getPtr()->handleMousePressed(evt, id);
}

void PlayScreen::mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
    Screen::mouseReleased(evt, id);
    PlayerManager::getPtr()->handleMouseReleased(evt, id);
}


/**** PauseScreen *****/

void PauseScreen::enter()
{
    mEntities = entityx::EntityManager::make(MessageManager::getPtr()->getEventMgr());

    mTrayMgr = new SdkTrayManager("PauseTray",RenderManager::getPtr()->getRenderWindow(),
                                  InputManager::getPtr()->getMouse(), this);

    Ogre::Real buttonWidth = RenderManager::getPtr()->getRenderWindow()->getWidth() *.25;
    mTrayMgr->createButton(TL_CENTER, "ResumeGameButton","Resume Game", buttonWidth);
    mTrayMgr->createButton(TL_CENTER, "ReturnButton", "Return to Main Menu", buttonWidth);
    mTrayMgr->createButton(TL_CENTER, "QuitButton", "Quit Game", buttonWidth);
}

void PauseScreen::pause()
{
    mTrayMgr->hideAll();
}

void PauseScreen::resume()
{
    mTrayMgr->showAll();
}

void PauseScreen::leave()
{
    mTrayMgr->destroyAllWidgets();
    delete mTrayMgr;
    mTrayMgr = 0;
}

void PauseScreen::buttonHit(Button *button)
{
    std::string buttonName = button->getName();
    if(buttonName == "ResumeGameButton")
        ScreenManager::getPtr()->popScreen();
    if(buttonName == "ReturnButton")
        ScreenManager::getPtr()->resetTo(MenuScreen::getPtr());
    if(buttonName == "QuitButton")
        mTrayMgr->showYesNoDialog("Quit the Game?","Do you want to quit?");

}

void PauseScreen::yesNoDialogClosed(const Ogre::DisplayString &question, bool yesHit)
{
    if( question == "Do you want to quit?" && yesHit)
        MessageManager::getPtr()->emit<ShutDownEvent>();
}

void PauseScreen::keyPressed(const OIS::KeyEvent &evt)
{

}

void PauseScreen::keyReleased(const OIS::KeyEvent &evt)
{
    if(evt.key == OIS::KC_ESCAPE)
        ScreenManager::getPtr()->popScreen();
}

void PauseScreen::update(double dt)
{

}

/**** GameOverScreen *****/

void GameOverScreen::enter()
{
    mEntities = entityx::EntityManager::make(MessageManager::getPtr()->getEventMgr());

    mTime = screenDuration;

    Ogre::RenderWindow *window = RenderManager::getPtr()->getRenderWindow();
    float width = window->getWidth() * .25;

    mTrayMgr = new SdkTrayManager("GameOverTray",window, InputManager::getPtr()->getMouse(), this);

    mTrayMgr->createLabel(TL_CENTER,"GameOverNotice", "Game Over", width);
    mTrayMgr->createLabel(TL_CENTER,"GameOverScore", "Your Score: " +
                          Ogre::StringConverter::toString(ScoreManager::getPtr()->getScore()) ,
                          width);

}

void GameOverScreen::pause()
{
    mTrayMgr->hideAll();
}

void GameOverScreen::resume()
{
    mTrayMgr->showAll();
}

void GameOverScreen::leave()
{
    mTrayMgr->destroyAllWidgets();
    delete mTrayMgr;
    mTrayMgr = 0;

}

void GameOverScreen::update(double timeSinceLastFrame)
{
    mTime -= timeSinceLastFrame;
    std::cout << "Tempo rimasto: " << mTime << std::endl;
    if( mTime <= 0 ){
        mTime = screenDuration;
        ScoreManager::getPtr()->resetScore();
        ScreenManager::getPtr()->changeScreen(MenuScreen::getPtr());
    }

}

void GameOverScreen::keyPressed(const OIS::KeyEvent &evt)
{

}

void GameOverScreen::keyReleased(const OIS::KeyEvent &evt)
{

}


