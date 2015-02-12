#ifndef HEATINGMANAGER_H
#define HEATINGMANAGER_H

#include "Singleton.h"
#include "Messages.h"
#include <entityx.h>


class HeatingManager : public Singleton<HeatingManager>
{
public:
    HeatingManager();
    void init();
    void update();
};

#endif // HEATINGMANAGER_H
