#include "puppet.h"

void move_puppet(Puppet *puppet, int16_t newX, int16_t newY)
{
    puppet->x=newX;
    puppet->y=newY;
}