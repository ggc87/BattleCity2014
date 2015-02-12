#include "InputManager.h"
#include "Messages.h"
#include "MessageManager.h"
InputManager::InputManager(){}

InputManager::~InputManager()
{
    if(mInputMgr)
    {
        if(mMouse)
        {
            mInputMgr->destroyInputObject(mMouse);
            mMouse = 0;
        }

        if(mKeyboard)
        {
            mInputMgr->destroyInputObject(mKeyboard);
            mKeyboard = 0;
        }

        mInputMgr->destroyInputSystem(mInputMgr);
        mInputMgr = 0;
    }


}

void InputManager::init()
{
    OIS::ParamList pl;
    Ogre::RenderWindow* window = RenderManager::getPtr()->getRenderWindow();

    std::ostringstream windowHndStr;
    size_t windowHnd = 0;

    window->getCustomAttribute("WINDOW", &windowHnd);

    windowHndStr << windowHnd;

    pl.insert(std::make_pair(std::string("WINDOW"),windowHndStr.str()));
    pl.insert(std::make_pair(std::string("x11_mouse_grab"), std::string("false")));
    //pl.insert(std::make_pair(std::string("x11_keyboard_grab"), std::string("false")));

    mInputMgr = OIS::InputManager::createInputSystem(pl);

    mKeyboard = static_cast<OIS::Keyboard*>(mInputMgr->createInputObject(OIS::OISKeyboard, true));
    mMouse = static_cast<OIS::Mouse*>(mInputMgr->createInputObject(OIS::OISMouse, true));

    mKeyboard->setEventCallback(this);
    mMouse->setEventCallback(this);


    unsigned int width, height, depth;
    int left, top;

    window->getMetrics(width, height, depth, left, top);

    this->setWindowExtents(width, height);

}

void InputManager::setWindowExtents(int width, int height)
{
    const OIS::MouseState &mouseState = mMouse->getMouseState();

    mouseState.width = width;
    mouseState.height = height;
}

void InputManager::update()
{
    capture();
}

bool InputManager::keyPressed(const OIS::KeyEvent &arg)
{
    MessageManager::getPtr()->emit<KeyPressedEvent>(arg);
}

bool InputManager::keyReleased(const OIS::KeyEvent &arg)
{
    MessageManager::getPtr()->emit<KeyReleasedEvent>(arg);
}

bool InputManager::mouseMoved(const OIS::MouseEvent &arg)
{
    MessageManager::getPtr()->emit<MouseMovedEvent>(arg);
}

bool InputManager::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
    MessageManager::getPtr()->emit<MousePressedEvent>(arg,id);
}

bool InputManager::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
    MessageManager::getPtr()->emit<MouseReleasedEvent>(arg, id);
}



OIS::Mouse* InputManager::getMouse()
{
    return mMouse;
}

OIS::Keyboard* InputManager::getKeyboard()
{
    return mKeyboard;
}

void InputManager::capture()
{
    if(mMouse)
        mMouse->capture();

    if(mKeyboard)
        mKeyboard->capture();

}
