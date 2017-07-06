/*==============================================================================
    ArthourP's And sabazabaxidze's H1Z1 Gamemode
        Copyright (C) 2017 Giorgi "Crayder" Medzvelia
        This program is free software: you can redistribute it and/or modify it
        under the terms of the GNU General Public License as published by the
        Free Software Foundation, either version 3 of the License, or (at your
        option) any later version.
        This program is distributed in the hope that it will be useful, but
        WITHOUT ANY WARRANTY; without even the implied warranty of
        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
        See the GNU General Public License for more details.
        You should have received a copy of the GNU General Public License along
        with this program.  If not, see <http://www.gnu.org/licenses/>.
==============================================================================*/

#include <YSI\y_hooks>


enum
{
	ATTACHSLOT_BLOOD
}


static
Float:	bld_BleedRate[MAX_PLAYERS], Float: PlayerHP[MAX_PLAYERS];




hook OnPlayerScriptUpdate(playerid)
{
/*	if(!IsPlayerSpawned(playerid))
	{
		RemovePlayerAttachedObject(playerid, ATTACHSLOT_BLOOD);
		return;
	}*/
	new Float:UpdateHP[MAX_PLAYERS];
	GetPlayerHealth(playerid, UpdateHP[playerid]);
	PlayerHP[playerid] = UpdateHP[playerid]; 
	if(IsNaN(bld_BleedRate[playerid]) || bld_BleedRate[playerid] < 0.0)
		bld_BleedRate[playerid] = 0.0;

	if(bld_BleedRate[playerid] > 0.0)
	{
		new
			Float:hp = GetPlayerHP(playerid),
			Float:slowrate = (((((100.0 - hp) / 360.0) * bld_BleedRate[playerid])) / 100.0);

		if(frandom(1.0) < 0.7)
		{
			SetPlayerHealth(playerid, hp - bld_BleedRate[playerid]);

			if(GetPlayerHP(playerid) < 0.1)
				SetPlayerHealth(playerid, 0.0);
		}

		if(random(100) < 50)
			bld_BleedRate[playerid] -= slowrate;

		if(!IsPlayerInAnyVehicle(playerid))
		{
			if(IsPlayerAttachedObjectSlotUsed(playerid, ATTACHSLOT_BLOOD))
			{
				if(frandom(0.1) < 0.1 - bld_BleedRate[playerid])
					RemovePlayerAttachedObject(playerid, ATTACHSLOT_BLOOD);
			}
			else
			{
				if(frandom(0.1) < bld_BleedRate[playerid])
					SetPlayerAttachedObject(playerid, ATTACHSLOT_BLOOD, 18706, 1,  0.088999, 0.020000, 0.044999,  0.088999, 0.020000, 0.044999,  1.179000, 1.510999, 0.005000);
			}
		}
		else
		{
			RemovePlayerAttachedObject(playerid, ATTACHSLOT_BLOOD);
		}
	}
	else
	{
		if(IsPlayerAttachedObjectSlotUsed(playerid, ATTACHSLOT_BLOOD))
			RemovePlayerAttachedObject(playerid, ATTACHSLOT_BLOOD);
		if(bld_BleedRate[playerid] < 0.0)
			bld_BleedRate[playerid] = 0.0;
	}

	return;
}

stock SetPlayerBleedRate(playerid, Float:rate)
{
	if(!IsPlayerConnected(playerid))
		return 0;

	bld_BleedRate[playerid] = rate;

	return 1;
}

forward Float:GetPlayerBleedRate(playerid);
stock Float:GetPlayerBleedRate(playerid)
{
	return bld_BleedRate[playerid];
}


stock GivePlayerHP(playerid, Float:gavehp)
{
	new Float:playercurhp[MAX_PLAYERS];
	GetPlayerHealth(playerid, playercurhp[playerid]);
	SetPlayerHealth(playerid, playercurhp[playerid] + gavehp);
	return 1;
}

forward Float:GetPlayerHP(playerid);
stock Float:GetPlayerHP(playerid)
{
	return PlayerHP[playerid];
}
