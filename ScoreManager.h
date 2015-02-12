#ifndef SCOREMANAGER_H
#define SCOREMANAGER_H
#include "MessageManager.h"
#include "Messages.h"
#include <entityx.h>
#include "Singleton.h"
class ScoreManager : public entityx::Receiver<ScoreManager>, public Singleton<ScoreManager>
{
public:
    ScoreManager();
    void init();
    void receive(const ObjectDestroyed &);
    int getScore();
    void resetScore();
private:
    const int unitScore = 10;
    int score;
    std::map<std::string, int> ranking;


};

#endif // SCOREMANAGER_H
