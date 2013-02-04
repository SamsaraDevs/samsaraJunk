http://zandronum.com/forum/showthread.php?tid=200
http://forum.zdoom.org/viewtopic.php?f=19&t=33219

Compiling samsara.c:

You'll need a special zspecial.acs, and the latest version of ACC. Since this is
primarily a Zandronum project and thus can't use ZDoom-specific features (and
for the ZDoom version, it sticks to 2.3.1 ACS features only), you need a
zspecial.acs with definitions for ConsolePlayerNumber, PlayerIsSpectator, and
maybe a couple other things I forgot. You also need to comment out the Sqrt and
FixedSqrt definitions in zspecial.acs, if they're there. You can either edit the
file directly or use the zspecial.acs at (http://ijontichy.lostsig.com/pub/zspecial.acs).

If you do edit your zspecial.acs, here are the Zandronum-specific definitions:
	
        -100:ResetMap(0),
        -101:PlayerIsSpectator(1),
        -102:ConsolePlayerNumber(0),
        -103:GetTeamProperty(2),

And comment these lines out, if present.

	-48:Sqrt(1),
	-49:FixedSqrt(1),

samsara.c should compile after that. 
