#define ACCURACYCOUNT   10
#define SIGILCOUNT      5

int SigilSplinters[SIGILCOUNT] = 
{
    "SigilSplinter1",
    "SigilSplinter2",
    "SigilSplinter3",
    "SigilSplinter4",
    "SigilSplinter5",
};

int SpectralTogglers[SIGILCOUNT] = 
{
    "SpectralToggler1",
    "SpectralToggler2",
    "SpectralToggler3",
    "SpectralToggler4",
    "SpectralToggler5",
};

int BeaconMonsters[CLASSCOUNT] = 
{
    "FriendlyDoomImp",
    "FriendlyFlemoid",
    "FriendlyNitrogolem",
    "FriendlyNazi",
    "FriendlyAfrit",
    "FriendlyAlienTrooper",
    "FriendlyPfhorTrooper",
    "FriendlyGrunt",
};

int AccuracyUniques[CLASSCOUNT][ACCURACYCOUNT] = 
{
    {"DoomZerkCounter",  "", "", "", "", "", "", "", "", ""},
    {"ChexSoulsphere",   "", "", "", "", "", "", "", "", ""},
    {"ArtiEgg2",         "", "", "", "", "", "", "", "", ""},
    {"WolfExtraLife",    "", "", "", "", "", "", "", "", ""},
    {"PortMysticAmbit",  "", "", "", "", "", "", "", "", ""},
    {"DukeguyStrifeJet", "", "", "", "", "", "", "", "", ""},
    {"RandAlienWeapon",  "", "", "", "", "", "", "", "", ""},
    {"Thunderbolt",      "RangerMaxCells", "", "", "", "", "", "", "", ""},
};

int DoomAccuracyZerks[ACCURACYCOUNT] = 
{
    "DoomguyStrifeZerk1",
    "DoomguyStrifeZerk2",
    "DoomguyStrifeZerk3",
    "DoomguyStrifeZerk4",
    "DoomguyStrifeZerk5",
    "DoomguyStrifeZerk6",
    "DoomguyStrifeZerk7",
    "DoomguyStrifeZerk8",
    "DoomguyStrifeZerk9",
    "DoomguyStrifeZerk0",
};

int AccuracyMessages[CLASSCOUNT][MSGCOUNT] = 
{
    {
        "You grow more berserk.",
        "You're not actually getting angrier. You're just punching more accurately.",
        "Fuck your accuracy training shit!",
        "Yes... channel your anger...",
        "How does that even work?",
        "Well, if you insist on using your fists...",
        "",
        "",
        "",
        "",
    },
    {
        "Supercharge breakfast!",
        "OM NOM NOM NOM",
        "Free breakfast with every training!",
        "Crunchy.",
        "Chex marks the spot!",
        "How generous of him to provide you breakfast, too.",
        "",
        "",
        "",
        "",
    },
    {
        "MORPH OVUM",
        "Parthoris Fried Chicken.",
        "Because nothing's better than turning an Inquisitor harmless.",
        "I'm calling fowl on this.",
        "Don't eat it.",
        "I'm sure there's hundreds of egg puns you can use.",
        "",
        "",
        "",
        "",
    },
    {
        "You got an extra life!",
        "Maybe now you can go to college.",
        "Maybe one day this won't suck in Zandronum.",
        "One day, the villains will learn how to respawn, too.",
        "Now you can hug a demon with no repercussions.",
        "Go ahead, taunt those Templars.",
        "",
        "",
        "",
        "",
    },
    {
        "MYSTIC AMBIT INCANT",
        "Heal plz.",
        "Warning: Not for use in deathmatch.",
        "A crowd on demand.",
        "Well, you're a cleric. Gotta have a heal.",
        "This would probably be more useful if Strife could do coop.",
        "The rebels already have a doctor...",
        "",
        "",
        "",
    },
    {
        "JETPACK!",
        "I believe I can touch the sky.",
        "Guaranteed to make friends with mappers.",
        "And this is why Virus will never be in.",
        "Because you have that on your sprite for a reason.",
        "Watch your ass, the jet burns.",
        "And fuck you too, fall damage.",
        "",
        "",
        "",
    },
    {
        "You got th@nS0-.'\casystem error \cd0xfded",
        "The difference between a room of aliens and a carpet of them.",
        "What fun to watch you work.",
        "~text interface term\cinal malfunction error \cd~2992dud",
        "your former self destroyed the dreaming way is eased",
        "down to the crushing center and spared the dance of forever",
        "This will allow you to kill more aliens. Does that make you happy?",
        "shedding all the weight of imagined moments and fear",
        "because fear is not the blood",
        "",
    },
    {
        "You got the Thunderbolt!",
        "Let's make an electromagnet, bring the enemies closer.",
        "Who wants a body massage?",
        "Stick a socket in it.",
        "There's nothin' like a fistfulla' lightnin', now is there?",
        "Cower, mortal, for now I control thy destiny.",
        "Atop the tallest of earth's peaks dwell the gods of earth, and suffer not man to tell that he hath looked upon them.",
        "Ia ia fhtagn, you flying squash.",
        "Ia ia fhtagn, you floating flower pot.",
        "Control the fucking weather.",
    },
};
