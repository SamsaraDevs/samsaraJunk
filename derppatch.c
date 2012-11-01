script SAMSARA_PUKE (int values, int pln) net
{
    PrintBold(s:"Player \"", n:pln+1, s:"\c-\" puked script ", d:SAMSARA_PUKE, s:" (", d:values, s:", ", d:pln, s:")");
    array_wolfmove[pln]     = values & 1;
    array_vanillaAnim[pln]  = values & 2;
    array_ballgag[pln]      = values & 4;
    array_weaponBar[pln]    = values & 8;
    array_pickupswitch[pln] = values & 16;
}
