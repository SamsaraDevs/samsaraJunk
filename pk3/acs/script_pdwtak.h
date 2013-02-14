script SAMSARA_PDWTAK death
{
    int bombtimer, yourTID;
    int myTID = defaultTID(-1);
    int classNum = samsaraClassNum();
    int x,y,z;
    int rVel, rAng, rPit, rTID;
    int i,j,k;

    if (!GetCVar("samsara_peoplediewhentheyarekilled")) { terminate; }

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
            if (yourTID == 0)
            {
                yourTID = unusedTID(15000, 16999);
                Thing_ChangeTID(0, yourTID);
            }

            x = ftoi(GetActorX(yourTID) - GetActorX(myTID));
            y = ftoi(GetActorY(yourTID) - GetActorY(myTID));
            z = ftoi(GetActorZ(yourTID) - GetActorZ(myTID));

            bombtimer = itof(magnitudeThree(x, y, z));

            if (bombtimer < 128.0)
            {
                bombtimer = 98;
            }
            else if (bombtimer > 1024.0)
            {
                bombtimer = 0;
            }
            else
            {
                bombtimer = ftoi(98 * FixedDiv(1.0, bombtimer/128));
            }
        }
    }

    SetActivator(myTID);
    x = GetActorX(0);
    y = GetActorY(0);
    z = GetActorZ(0) + 32.0;

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
        else
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

    SetActorPosition(yourTID, x,y,z, 0);
    GiveActorInventory(yourTID, "InvPeopleDie", 1);
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
