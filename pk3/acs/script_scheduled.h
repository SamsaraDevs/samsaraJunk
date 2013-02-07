script SAMSARA_SCHEDULED (int respawning, int slot)
{
    int pln = PlayerNumber();
    int class = samsaraClassNum();
    int startTic = Timer();
    int ticNum;

    if (slot < 0 || slot > 1 || class == -1) { terminate; }
    ScheduledTimes[pln][slot] = startTic;

    while (ScheduledTimes[pln][slot] == startTic)
    {
        switch (ticNum)
        {
          case 0:
            if (slot != 0 || class != CLASS_CHEX || GameType() != GAME_NET_DEATHMATCH) { break; }
            GiveInventory("ChexUnsolid", 1);
            break;
          
          case 53:  // 1.5 seconds
            if (slot != 0 || class != CLASS_CHEX || GameType() != GAME_NET_DEATHMATCH) { break; }
            GiveInventory("ChexSolid", 1);
            break;
        }

        ticNum++;
        Delay(1);
    }
}
