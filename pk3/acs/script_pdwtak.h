script SAMSARA_PDWTAK death
{
    int bombtimer, yourTID;
    int pdwtak = GetCVar("samsara_peoplediewhentheyarekilled");
    int myTID = defaultTID(-1);
    int classNum = samsaraClassNum();
    int x,y,z,z2;
    int rVel, rAng, rPit, rTID;
    int i,j,k,l;

    if (!pdwtak) { terminate; }
    pdwtak = middle(1, pdwtak, PDWTAKCOUNT)-1;

    x  = GetActorX(myTID);
    y  = GetActorY(myTID);
    z  = GetActorZ(myTID) + 32.0;
    z2 = GetActorZ(myTID);

    SetActivatorToTarget(0);

    if (PlayerNumber() == -1)
    {
        bombtimer = 35;
    }
    else
    {
        yourTID = ActivatorTID();

        if (yourTID == myTID)
        {
            bombtimer = 0;
        }
        else
        {
            bombtimer = 24;
        }

        if (yourTID == 0)
        {
            yourTID = unusedTID(15000, 16999);
            Thing_ChangeTID(0, yourTID);
        }
    }

    for (bombtimer += 0; bombtimer > 0; bombtimer--)
    {
        rTID = unusedTID(12000, 14999);

        rVel = random(0, 12.0);
        rAng = random(0, 1.0);
        rPit = random(-0.25, 0.25);

        if (bombtimer % 18 == 0)
        {
            if (classNum != -1)
            {
                j = PeopleDiePulses[classNum];
            }
            else
            {
                j = "DoomguysDiePulse";
            }

            Spawn(j, x,y,z, rTID);
        }
        else if (Timer() % 2)
        {
            if (classNum != -1)
            {
                j = PeopleDieParticles[classNum];
            }
            else
            {
                j = "DoomguysDieParticle";
            }

            Spawn(j, 
                x + FixedMul(FixedMul(rVel, cos(rAng)), cos(rPit)),
                y + FixedMul(FixedMul(rVel, sin(rAng)), cos(rPit)),
                z + FixedMul(rVel, sin(rPit)),
                  rTID);

            rVel = random(0, 1.0);
            rVel = FixedMul(rVel, rVel);
            rAng = random(0, 1.0);
            rPit = random(-0.25, 0.25);

            SetActorVelocity(rTID,
                            FixedMul(FixedMul(rVel, cos(rAng)), cos(rPit)) / 2,
                            FixedMul(FixedMul(rVel, sin(rAng)), cos(rPit)) / 2,
                            FixedMul(rVel, sin(rPit))*2,
                            0, 0);
        }

        Delay(1);
    }

    i = GetActorX(yourTID);
    j = GetActorY(yourTID);
    k = GetActorZ(yourTID);

    SetActorPosition(yourTID, x,y,z2, 0);
    GiveActorInventory(yourTID, PDWTAKItems[pdwtak][classNum == CLASS_CHEX], 1);
    SetActorPosition(yourTID, i,j,k, 0);


    if (classNum != -1)
    {
        Spawn(PeopleDieBlasts[classNum], x,y,z);
    }
    else
    {
        Spawn("DoomguyDiesWhenHeIsKilled", x,y,z);
    }
}
