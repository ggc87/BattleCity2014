#include "ScoreManager.h"
#include "PlayerManager.h"
#include "ScreenManager.h"
#include "Screen.h"
ScoreManager::ScoreManager()
{
}

void ScoreManager::init()
{
    score = 0;
    MessageManager::getPtr()->subscribe<ObjectDestroyed>(*this);

}

void ScoreManager::receive(const ObjectDestroyed &msg)
{
    entityx::Entity obj = msg.object;
    if(obj == PlayerManager::getPtr()->getPlayerEntity())
        ScreenManager::getPtr()->resetTo(GameOverScreen::getPtr());
    else if(obj.component<Name>()->name != "Block")
    {
        score += unitScore;
    }

}

int ScoreManager::getScore(){
    return score;
}

void ScoreManager::resetScore()
{
    score= 0;
}
