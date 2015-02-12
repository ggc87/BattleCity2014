#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H
#include <OIS.h>
#include "Singleton.h"
#include <entityx.h>
#include "RenderManager.h"
#include "MessageManager.h"

class InputManager : public Singleton<InputManager>, public OIS::KeyListener, public OIS::MouseListener
{
public:
    InputManager();
    ~InputManager();
    void init();
    void update();
    bool keyPressed(const OIS::KeyEvent &arg);
    bool keyReleased(const OIS::KeyEvent &arg);
    bool mouseMoved(const OIS::MouseEvent &arg);
    bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
    bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
    void setWindowExtents(int width, int height);
    OIS::Mouse* getMouse();
    OIS::Keyboard* getKeyboard();

private:
    void capture();
    OIS::InputManager* mInputMgr;
    OIS::Mouse* mMouse;
    OIS::Keyboard* mKeyboard;
};

#endif // INPUTMANAGER_H
