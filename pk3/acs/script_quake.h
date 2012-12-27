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
        if (DEBUG) { Print(s:"hpGiven = ", d:hpGiven, s:", startHealth = ", d:startHealth); }

        if (CheckInventory("RuneProsperity"))
        {
            break;
        }

        if (CheckInventory("QuakeRegenTimer"))
        {
            Delay(1);
            continue;
        }

        if (UnloadingNow)
        {
            hpToTake = GetActorProperty(0, APROP_Health);

            if (DEBUG) { Print(d:hpToTake, s:", ", d:getMaxHealth(), s:", ", d:hpToTake - hpGiven); }

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
    if (defaultCVar("samsara_cl_norecoil", 0) == 1) { terminate; }

    degrees = itof(degrees);
    if (degrees < 0) { degrees /= -100; }

    int qCurve, oldPitch, newPitch, pitchDiff, i;

    if (ticsup > 0)
    {
        qCurve = degrees / pow(-ticsup, 2);
        newPitch = qCurve * pow(-ticsup, 2);

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
        qCurve = -degrees / pow(ticsDown, 2);
        newPitch = qCurve * pow(-ticsdown, 2);

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
        SetActorPitch(0, GetActorPitch(0) + (degrees / 360));
    }
}
