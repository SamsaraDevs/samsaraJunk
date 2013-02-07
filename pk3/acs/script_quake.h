script SAMSARA_MEGAHEALTH (int hpcount, int hpPerSec, int delayTics)
{
    int hpGiven = GetActorProperty(0, APROP_Health);
    int startHealth = hpGiven;
    SetActorProperty(0, APROP_Health, min(hpGiven + hpcount, 250));
    hpGiven = GetActorProperty(0, APROP_Health) - hpGiven;

    hpPerSec = 1.0 / (hpPerSec * 35);

    int takeCounter, hpToTake;

    while (1)
    {
        if (CheckInventory("QuakeRegenTimer") || CheckInventory("RuneProsperity"))
        {
            Delay(1);
            continue;
        }

        if (UnloadingNow)
        {
            hpToTake = GetActorProperty(0, APROP_Health);

            hpToTake = middle(hpToTake, getMaxHealth(), hpToTake - hpGiven);
            SetActorProperty(0, APROP_Health, hpToTake);
            break;
        }

        if (delayTics > 0) { delayTics--; }
        else
        {
            takeCounter += hpPerSec;
            hpToTake = min(ftoi(takeCounter), hpGiven);
            hpGiven -= hpToTake;

            SetActorProperty(0, APROP_Health, GetActorProperty(0, APROP_Health) - hpToTake);
            takeCounter -= itof(hpToTake);
        }

        if (hpGiven <= 0) { break; }
        if (GetActorProperty(0, APROP_Health) <= startHealth ||
            GetActorProperty(0, APROP_Health) <= getMaxHealth()) { break; }
        Delay(1);
    }
}

script SAMSARA_RECOIL (int degrees, int ticsup, int ticsdown) clientside
{
    if (GetCVar("samsara_cl_norecoil") == 1) { terminate; }

    degrees = itof(degrees);
    if (degrees < 0) { degrees /= -100; }

    int qCurve, oldPitch, newPitch, pitchDiff, i;
    int amplitude = degrees / 2;

    if (ticsup > 0)
    {
        qCurve = degrees / pow(ticsup, 2);  // this is a
        newPitch = qCurve * pow(ticsup, 2);

        for (i = 0; i < ticsup; i++)
        {
            oldPitch = newPitch;
            // y = a(x-h)**2 + k
            newPitch = qCurve * pow((i+1)-ticsup, 2);
            pitchDiff = (newPitch - oldPitch) / 360;

            SetActorPitch(0, GetActorPitch(0) + pitchDiff);
            Delay(1);
        }
    }
    else
    {
        SetActorPitch(0, GetActorPitch(0) - (degrees / 360));
    }

    if (ticsdown > 0)
    {
        if (GetCVar("samsara_cl_sinerecoil") == 0)
        {
            qCurve = -degrees / pow(ticsdown, 2); // this is also a
            newPitch = qCurve * pow(ticsdown, 2);

            for (i = 0; i < ticsdown; i++)
            {
                oldPitch = newPitch;
                // y = a(x-h)**2 + k
                newPitch = qCurve * pow((i+1)-ticsdown, 2);
                pitchDiff = (newPitch - oldPitch) / 360;

                SetActorPitch(0, GetActorPitch(0) + pitchDiff);
                Delay(1);
            }
        }
        else
        {
            // y = (a/2) * sin(x) + (a/2)
            newPitch = 0;

            for (i = 0; i <= ticsdown; i++)
            {
                oldPitch = newPitch;
                newPitch = -FixedMul(amplitude, cos(itof(i) / (ticsdown*2))) + amplitude;
                pitchDiff = (newPitch - oldPitch) / 360;

                SetActorPitch(0, GetActorPitch(0) + pitchDiff);
                Delay(1);
            }
        }
    }
    else
    {
        SetActorPitch(0, GetActorPitch(0) + (degrees / 360));
    }
}
