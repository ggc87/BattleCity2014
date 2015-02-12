#ifndef MOVEMENTMANAGER_H
#define MOVEMENTMANAGER_H
#include <entityx.h>
#include "Singleton.h"
#include "Messages.h"

class MovementManager : public Singleton<MovementManager>
{
public:
    MovementManager();
    void init();
    void update(double dt);

};

#endif // MOVEMENTMANAGER_H
