#include "IPuppetFactory.h"
#include "puppetFactory.h"
#include "puppet.h"
#include "../shared/rawPuppets.h"

Puppet *create_puppet(uint8_t puppetIndex)
{
    Puppet *newPuppet = (Puppet*)malloc(sizeof(Puppet));
    
    return newPuppet;
}

static IPuppetFactory puppet = {
    .create_puppet = create_puppet,
};

const IPuppetFactory *get_puppetFactory(void)
{
    return &puppet;
}