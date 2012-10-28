#include "zcommon.acs"
#library "samsara_tichy"

#include "commonFuncs.h"

#include "samsaraDefs.h"
#include "samsaraFuncs.h"


script 717 (void)
{
    GiveClassWeapon(samsaraClassNum(), itemToSlot(7), 2);
}

script SAMSARA_ENTER enter { ACS_ExecuteAlways(SAMSARA_SPAWN, 0, 0,0,0); }
script SAMSARA_RESPAWN respawn { ACS_ExecuteAlways(SAMSARA_SPAWN, 0, 1,0,0); }

script SAMSARA_SPAWN (int respawning)
{
    if (isLMS()) { ApplyLMS(); }

    SetInventory("CoopModeOn", isCoop());

    switch (samsaraClassNum())
    {
      case CLASS_DUKE:
        GiveInventory("DukeReady", 1);
        break;

      case CLASS_MARATHON:
        if (isInvasion()) { GiveInventory("InvasionDualShottyCheck", 1); }
        break;
    }

    while (1)
    {
        if (CheckInventory("MarathonClass"))
        {
            if (GetCVar("sv_sogravity"))
            {
                SetActorProperty(0, APROP_Gravity, 1.0);
                SetActorProperty(0, APROP_JumpZ,   9.0);
            }
            else
            {
                SetActorProperty(0, APROP_Gravity, 0.15);
                SetActorProperty(0, APROP_JumpZ,   4.0);
            }
        }

        Delay(1);

        if (isDead(0)) { break; }
    }
}
