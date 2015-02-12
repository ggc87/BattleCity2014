#ifndef SCREEN_H
#define SCREEN_H
#include <SdkTrays.h>
#include <entityx.h>
#include "Singleton.h"


class Screen : public OgreBites::SdkTrayListener
{
public:
    Screen() {}
    virtual void enter() = 0;
    virtual void pause() = 0;
    virtual void resume() = 0;
    virtual void leave() = 0;

    virtual void update(double dt) = 0;

    virtual void keyPressed(const OIS::KeyEvent &evt) = 0;
    virtual void keyReleased(const OIS::KeyEvent &evt) = 0;
    virtual void mouseMoved(const OIS::MouseEvent &evt);
    virtual void mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id);
    virtual void mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id);


    entityx::ptr<entityx::EntityManager> getEntities() { return mEntities; }
    OgreBites::SdkTrayManager *getTrayMgr() { return mTrayMgr; }


protected:
    entityx::ptr<entityx::EntityManager> mEntities;
    OgreBites::SdkTrayManager *mTrayMgr;

};



class MenuScreen : public Screen, public Singleton<MenuScreen>
{
public:
    MenuScreen(){}
    void enter();
    void pause();
    void resume();
    void leave();
    void update(double dt);
    void buttonHit(OgreBites::Button *button);
    void yesNoDialogClosed(const Ogre::DisplayString &question, bool yesHit);
    void keyPressed(const OIS::KeyEvent &evt);
    void keyReleased(const OIS::KeyEvent &evt);
private:
    bool change;
};

class PlayScreen : public Screen, public Singleton<PlayScreen>
{
public:
    PlayScreen() {}
    void enter();
    void pause();
    void resume();
    void leave();
    void update(double dt);
    void keyPressed(const OIS::KeyEvent &evt);

    void keyReleased(const OIS::KeyEvent &evt);
    void mouseMoved(const OIS::MouseEvent &evt);
    void mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id);
    void mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id);

private:
    OgreBites::ParamsPanel *mInfoPanel;
    OgreBites::ProgressBar *mHealthBar;
    OgreBites::ProgressBar *mHeatBar;
};

class PauseScreen : public Screen, public Singleton<PauseScreen>
{
public:
    PauseScreen() {}
    void enter();
    void pause();
    void resume();
    void leave();
    void update(double dt);
    void keyPressed(const OIS::KeyEvent &evt);

    void keyReleased(const OIS::KeyEvent &evt);

    void buttonHit(OgreBites::Button *button);
    void yesNoDialogClosed(const Ogre::DisplayString &question, bool yesHit);
};

class GameOverScreen : public Screen, public Singleton<GameOverScreen>
{
public:
    GameOverScreen() {}
    void enter();
    void pause();
    void resume();
    void leave();

    void update(double dt);

    void keyPressed(const OIS::KeyEvent &evt);
    void keyReleased(const OIS::KeyEvent &evt);

private:
    const double screenDuration = 5;
    double mTime;
};
#endif // SCREEN_H
