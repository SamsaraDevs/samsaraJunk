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
        if (CheckInventory("QuakeRegenTimer") || CheckInventory("RuneProsperity")
            || GetCVar("sv_degeneration") > 0)
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
    if (degrees == 0) { terminate; }
    if (defaultCVar("samsara_cl_norecoil", 0) == 1 && ticsup >= 0) { terminate; }
    if (GetCVar("sv_nofreelook")) { terminate; }

    degrees = itof(degrees);
    if (degrees < 0) { degrees /= -100; }

    int qCurve, oldPitch, newPitch, pitchDiff, i;
    int amplitude = degrees / 2;
    int doDownRecoil = 1;
    ticsup = abs(ticsup);

    if (ticsdown < 0)
    {
        if (GetCVar("freelook")) { doDownRecoil = 0; }
        else { ticsdown = -ticsdown; }
    }

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

    if (doDownRecoil)
    {
        if (ticsdown > 0)
        {
            if (defaultCVar("samsara_cl_sinerecoil", 0) == 0)
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
}

script SAMSARA_QPOWERS (int startTime)
{
    int pln = PlayerNumber();
    int quadTimer,  oQuadTimer;
    int regenTimer, oRegenTimer;
    int invisTimer, oInvisTimer;
    int pentTimer,  oPentTimer;
    int health, oHealth, regenPulse, oPulse;
    int regenX, regenY;
    int healthGiven, healthMax;
    int opowercount, powercount, offset;
    
    if (samsaraClassNum() != CLASS_QUAKE) { terminate; }

    quadTimer   = CheckQuad();
    regenTimer  = CheckInventory("QuakeRegenTimer");
    invisTimer  = CheckInventory("QuakeInvisTimer");
    pentTimer   = CheckInventory("QuakePentTimer");

    health = GetActorProperty(0, APROP_Health);

    while (ServerEnterTimes[pln] == startTime)
    {
        oHealth = health;
        health = GetActorProperty(0, APROP_Health);

        if (health < 0)
        {
            quadTimer = CheckQuad(); 
            TakeInventory("QuakeQuadTimer", quadTimer);

            TakeInventory("QuakeRegenTimer", 0x7FFFFFFF);
            TakeInventory("QuakeInvisTimer", 0x7FFFFFFF);
            TakeInventory("QuakePentTimer", 0x7FFFFFFF);

            Delay(1);
            continue;
        }

        oQuadTimer = quadTimer;
        quadTimer  = CheckQuad();

        oRegenTimer = regenTimer;
        regenTimer  = CheckInventory("QuakeRegenTimer");

        oInvisTimer = invisTimer;
        invisTimer  = CheckInventory("QuakeInvisTimer");

        oPentTimer = pentTimer;
        pentTimer  = CheckInventory("QuakePentTimer");

        offset = 0;
        opowercount = powercount;
        powercount = 0;
        powercount += (quadTimer > 0);
        powercount += (regenTimer > 0);
        powercount += (invisTimer > 0);
        powercount += (pentTimer > 0);

        if (quadTimer - 35 > oQuadTimer)
        {
            AmbientSound("quakeweps/quadon", 127);
        }

        if (quadTimer > 0)
        {
            if (quadTimer % 35 == 0 || oQuadTimer < quadTimer || (opowercount != powercount))
            {
                SetHudSize(640, 480, 1);
                SetFont("QUADICO2");
                HudMessage(s:"A"; HUDMSG_FADEOUT, 58101, CR_UNTRANSLATED, 610.4, PY640 - (POFF640 * offset), 1.5, 1.0);
                SetHudSize(320, 240, 1);
                SetFont("QUA3HUDF");
                HudMessage(d:(quadTimer+34) / 35;  HUDMSG_FADEOUT | HUDMSG_COLORSTRING, 58102, "QuakeBrick", 295.2, PY320 - (POFF320 * offset), 1.5, 1.0);
            }

            offset++;

            if (oQuadTimer <= 0 || oHealth <= 0) { GiveInventory("QuadDamagePower", 1); }
        }
        else
        {
            if (CheckInventory("DoNotQuad")) { TakeInventory("DoNotQuad", 1); }

            if (quadTimer == 0)
            {
                HudMessage(s:""; HUDMSG_PLAIN, 58101, CR_UNTRANSLATED, 0, 0, 1);
                HudMessage(s:""; HUDMSG_PLAIN, 58102, CR_UNTRANSLATED, 0, 0, 1);

                if (CheckInventory("SpectralFiring"))
                {
                    TakeInventory("SpectralFiring", 1);   // So ranger can't break the spectral weapons
                    LocalAmbientSound("weapons/sigil", 127);
                }
            }
            TakeInventory("QuadDamagePower", 1);
        }

        if (quadTimer == 105)
        {
            LocalAmbientSound("quakeweps/quadoff", 127);
        }

        if ((quadTimer % 35 == 0) && (quadTimer / 35 <= 3) && (quadTimer > 0))
        {
            FadeRange(0, 64, 255, 0.25, 0, 64, 255, 0, 0.33);
        }

        if (quadTimer == -QUAD_THRESHOLD)
        {
            if (CheckInventory("CantQuad") && CheckInventory("QuadDamageItem") && !UnloadingNow)
            {
                LocalAmbientSound("quakeweps/quadready", 96);
                SetHudSize(240, 180, 1);
                SetFont("QUADICO2");
                HudMessage(s:"A"; HUDMSG_FADEOUT, 58103, CR_UNTRANSLATED, 215.4, 142.0, 0.0, 1.0);
            }

            TakeInventory("CantQuad", 0x7FFFFFFF);
        }
        else
        {
            GiveInventory("CantQuad", 1);
        }

        /*
         * Pentagram shit
         */

        if (pentTimer - 35 > oPentTimer)
        {
            AmbientSound("quakeweps/pentagramon", 127);
        }

        if (pentTimer > 0)
        {
            if (pentTimer % 35 == 0 || oPentTimer < pentTimer || powercount != opowercount)
            {
                SetHudSize(640, 480, 1);
                SetFont("PENTICON");
                HudMessage(s:"A"; HUDMSG_FADEOUT, 58107, CR_UNTRANSLATED, 610.4, PY640 - (POFF640*offset), 1.5, 1.0);
                SetHudSize(320, 240, 1);
                SetFont("QUA3HUDF");
                HudMessage(d:(pentTimer+34) / 35;  HUDMSG_FADEOUT | HUDMSG_COLORSTRING, 58108, "QuakeBrick", 295.2, PY320 - (POFF320*offset), 1.5, 1.0);
            }

            offset++;

            if (pentTimer % 35 == 0 && pentTimer / 35 < 5)
            {
                LocalAmbientSound("quakeweps/pentagramout", PowerOutVols[regenTimer / 35]);
            }
            else if (Timer() % 140 == 0)
            {
                ActivatorSound("quakeweps/pentagram", 96);
            }

            if (oPentTimer <= 0 || oHealth <= 0) { GiveInventory("QuakePentagram", 1); }
        }
        else
        {
            if (oPentTimer != 0)
            {
                HudMessage(s:""; HUDMSG_PLAIN, 58107, CR_UNTRANSLATED, 0, 0, 1);
                HudMessage(s:""; HUDMSG_PLAIN, 58108, CR_UNTRANSLATED, 0, 0, 1);
                TakeInventory("QuakePentagram", 0x7FFFFFFF);
            }

        }

        /*
         * Regen shit
         */

        if (regenTimer != 0)
        {
            if (regenTimer - 35 > oRegenTimer) { AmbientSound("quakeweps/regenannounce", 127); }

            regenX = 640 - (regenPulse * 18);
            regenY = 480 - (regenPulse * 18);

            if (regenTimer % 35 == 0 || regenPulse != 0 || oPulse != 0 || oRegenTimer < regenTimer || (opowercount != powercount))
            {
                SetHudSize(regenX, regenY, 1);
                regenX = ftoi(regenX * REGEN_CENTER_X);
                regenY = ftoi(regenY * REGEN_CENTER_Y) - ftoi(regenY * (REGEN_OFFSET * offset));

                SetFont("REGENICO");
                HudMessage(s:"A"; HUDMSG_FADEOUT, 58103, CR_UNTRANSLATED, itof(regenX) + 0.4, itof(regenY), 1.25, 0.25);
                SetHudSize(320, 240, 1);
                SetFont("QUA3HUDF");
                HudMessage(d:(regenTimer+34) / 35;  HUDMSG_FADEOUT | HUDMSG_COLORSTRING, 58104, "QuakeBrick", 295.2, PY320 - (POFF320 * offset), 1.25, 0.25);
                // the +34 was added so that the regen pulse didn't prematurely lower the seconds display by one
            }

            offset++;

            oPulse = regenPulse;
            regenPulse = max(0, regenPulse - 1);

            if (regenTimer % 35 == 18 && !isDead(0))
            {
                healthMax = cond(GetCVar("samsara_nohealthcap"), 0x7FFFFFFF, 150+getMaxHealth());

                if (health >= getMaxHealth()) { giveHealthMax(5, healthMax); }
                else if (health + 10 >= getMaxHealth())
                {
                    SetActorProperty(0, APROP_Health, getMaxHealth());
                    giveHealthMax(5, healthMax);
                }
                else { giveHealthMax(15, healthMax); }

                if (GetActorProperty(0, APROP_Health) > health)
                {
                    FadeRange(255, 128, 128, 0.1, 255, 0, 0, 0.0, 0.3333);
                    ActivatorSound("quakeweps/regen", 127);
                    regenPulse = 12;
                }

                if (!GetCVar("samsara_nohealthcap")) { healthGiven += max(GetActorProperty(0, APROP_Health) - health, 0); }
                health = GetActorProperty(0, APROP_Health);
            }

            if (regenTimer % 35 == 0 && regenTimer / 35 < 5)
            {
                LocalAmbientSound("quakeweps/regenout", PowerOutVols[regenTimer / 35]);
            }
        }
        else if (CheckInventory("RuneProsperity") || GetCVar("sv_degeneration") > 0)
        {
            regenTimer = 0;
        }
        else
        {
            if (oRegenTimer != 0)
            {
                HudMessage(s:""; HUDMSG_PLAIN, 58103, CR_UNTRANSLATED, 0, 0, 1);
                HudMessage(s:""; HUDMSG_PLAIN, 58104, CR_UNTRANSLATED, 0, 0, 1);
            }

            if (health < getMaxHealth()) { healthGiven = 0; }

            if (Timer() % 35 == 0 && healthGiven > 0 && (health - 1 >= getMaxHealth()))
            {
                SetActorProperty(0, APROP_Health, health - 1);
            }
        }

        /*
         * Invis shit
         */

        if (invisTimer - 35 > oInvisTimer)
        {
            ActivatorSound("quakeweps/invison", 127);
        }

        if (invisTimer > 0)
        {
            if (invisTimer % 35 == 0 || oInvisTimer < invisTimer || powercount != opowercount)
            {
                SetHudSize(640, 480, 1);
                SetFont("INVISICO");
                HudMessage(s:"A"; HUDMSG_FADEOUT, 58105, CR_UNTRANSLATED, 610.4, PY640 - (POFF640 * offset), 1.5, 1.0);
                SetHudSize(320, 240, 1);
                SetFont("QUA3HUDF");
                HudMessage(d:(invisTimer+34) / 35;  HUDMSG_FADEOUT | HUDMSG_COLORSTRING, 58106, "QuakeBrick", 295.2, PY320 - (POFF320 * offset), 1.5, 1.0);
            }

            offset++;

            if (invisTimer % 105 == 0)
            {
                ActivatorSound("quakeweps/inviswhisper", 127);
            }

            if (invisTimer == 105)
            {
                LocalAmbientSound("quakeweps/invisout", 127);
            }

            if (oInvisTimer <= 0 || oHealth <= 0) { GiveInventory("QuakeInvisibility", 1); }
        }
        else
        {
            if (oInvisTimer != 0)
            {
                HudMessage(s:""; HUDMSG_PLAIN, 58105, CR_UNTRANSLATED, 0, 0, 1);
                HudMessage(s:""; HUDMSG_PLAIN, 58106, CR_UNTRANSLATED, 0, 0, 1);
                GiveInventory("QuakeInvisibilityOff", 1);
            }

        }

        TakeInventory("QuakeQuadTimer", 1);
        TakeInventory("QuakeRegenTimer", 1);
        TakeInventory("QuakeInvisTimer", 1);
        TakeInventory("QuakePentTimer", 1);

        if (UnloadingNow)
        {
            SetActorProperty(0, APROP_Health, middle(health, getMaxHealth(), health - healthGiven));
            health = GetActorProperty(0, APROP_Health);
            SetActorProperty(0, APROP_Health, max(health, getMaxHealth() / 2));
            break;
        }

        Delay(1);
    }
}

script SAMSARA_QUAKE (int class, int slot, int dropped)
{
    int givingQuad, givingLG;

    int stay = GetCVar("sv_weaponstay") && !dropped;
    int ultStay = GetCVar("samsara_permault") && !dropped;

    int doquad = 1;

    if (GetCVar("samsara_permault") && CheckInventory("QuakeQuadTimer") > 0)//CheckQuad() > 0)
    {
        doquad = 0;
    }

    switch (slot)
    {
      case SLOT_BFG9000:
        if (doquad)
        {
            if (isLMS())
            {
                if (GetCvar("samsara_permaquad") == 1)
                { GiveInventory("QuadDamageItem", 1); }
            }
            else
            {
                GiveQuad(1050);
    
                if (GetCvar("samsara_permaquad") == 1)//if (isCoop() || isSinglePlayer())
                { GiveInventory("QuadDamageItem", 1); }
			
            }
            givingQuad = 1;
        }

        if (!CheckInventory("Thunderbolt")
         || (!ultStay && (GetAmmoCapacity("Cell") > CheckInventory("Cell"))))
        {
            givingLG = 1;
        }

        if (givingLG)
        {
            GiveInventory("Thunderbolt", 1);
        }

        SetResultValue(givingQuad || givingLG);
        break;
    }
}
