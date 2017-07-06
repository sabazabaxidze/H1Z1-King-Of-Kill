stock AC_MODE_SetPlayerPos(playerid,Float:x,Float:y,Float:z)
	return CallRemoteFunction("AC_SetPlayerPos","ifff",playerid,x,y,z);
#define SetPlayerPos AC_MODE_SetPlayerPos
stock AC_MODE_PutPlayerInVehicle(playerid,vehicleid,seatid)
	return CallRemoteFunction("AC_PutPlayerInVehicle","iii",playerid,vehicleid,seatid);
#define PutPlayerInVehicle AC_MODE_PutPlayerInVehicle
stock AC_MODE_SetPlayerHealth(playerid,Float:health)
	return CallRemoteFunction("AC_SetPlayerHealth","if",playerid,health);
#define SetPlayerHealth AC_MODE_SetPlayerHealth
stock AC_MODE_SetPlayerArmour(playerid,Float:armour)
	return CallRemoteFunction("AC_SetPlayerArmour","if",playerid,armour);
#define SetPlayerArmour AC_MODE_SetPlayerArmour
stock AC_MODE_AddVehicleComponent(vehicleid,componentid)
	return CallRemoteFunction("AC_AddVehicleComponent","ii",vehicleid,componentid);
#define AddVehicleComponent AC_MODE_AddVehicleComponent
stock AC_MODE_RemoveVehicleComponent(vehicleid,componentid)
	return CallRemoteFunction("AC_RemoveVehicleComponent","ii",vehicleid,componentid);
#define RemoveVehicleComponent AC_MODE_RemoveVehicleComponent
stock AC_MODE_SetVehicleHealth(vehicleid,Float:health)
	return CallRemoteFunction("AC_SetVehicleHealth","if",vehicleid,health);
#define SetVehicleHealth AC_MODE_SetVehicleHealth
stock AC_MODE_RepairVehicle(vehicleid)
	return CallRemoteFunction("AC_RepairVehicle","i",vehicleid);
#define RepairVehicle AC_MODE_RepairVehicle
stock AC_MODE_CreateVehicle(vehicletype, Float:x, Float:y, Float:z, Float:rotation, color1, color2, respawn_delay)
	return CallRemoteFunction("AC_CreateVehicle","iffffiii",vehicletype,x,y,z,rotation,color1,color2,respawn_delay);
#define CreateVehicle AC_MODE_CreateVehicle

stock AC_MODE_AddStaticVehicle(modelid, Float:spawn_x, Float:spawn_y, Float:spawn_z, Float:z_angle, color1, color2)
	return CallRemoteFunction("AC_AddStaticVehicle","iffffii",modelid,spawn_x,spawn_y,spawn_z,z_angle,color1,color2);
#define AddStaticVehicle AC_MODE_AddStaticVehicle
stock AC_MODE_AddStaticVehicleEx(modelid, Float:spawn_x, Float:spawn_y, Float:spawn_z, Float:z_angle, color1, color2,respawn_delay)
	return CallRemoteFunction("AC_AddStaticVehicleEx","iffffiii",modelid,spawn_x,spawn_y,spawn_z,z_angle,color1,color2,respawn_delay);
#define AddStaticVehicleEx AC_MODE_AddStaticVehicleEx
stock AC_MODE_SetVehiclePos(vehicleid,Float:x,Float:y,Float:z)
	return CallRemoteFunction("AC_SetVehiclePos","ifff",vehicleid,x,y,z);
#define SetVehiclePos AC_MODE_SetVehiclePos
stock AC_MODE_SetVehicleZAngle(vehicleid,Float:angle)
	return CallRemoteFunction("AC_SetVehicleZAngle","if",vehicleid,angle);
#define SetVehicleZAngle AC_MODE_SetVehicleZAngle
stock AC_MODE_PlayerSpectatePlayer(playerid,targetplayerid,mode=SPECTATE_MODE_NORMAL)
	return CallRemoteFunction("AC_PlayerSpectatePlayer","iii",playerid,targetplayerid,mode);
#define PlayerSpectatePlayer AC_MODE_PlayerSpectatePlayer
stock AC_MODE_PlayerSpectateVehicle(playerid,targetplayerid,mode=SPECTATE_MODE_NORMAL)
	return CallRemoteFunction("AC_PlayerSpectateVehicle","iii",playerid,targetplayerid,mode);
#define PlayerSpectateVehicle AC_MODE_PlayerSpectateVehicle
stock AC_MODE_RemovePlayerFromVehicle(playerid)
	return CallRemoteFunction("AC_RemovePlayerFromVehicle","i",playerid);
#define RemovePlayerFromVehicle AC_MODE_RemovePlayerFromVehicle
stock AC_MODE_SetPlayerPosFindZ(playerid,Float:x,Float:y,Float:z)
	return CallRemoteFunction("AC_SetPlayerPosFindZ","ifff",playerid,x,y,z);
#define SetPlayerPosFindZ AC_MODE_SetPlayerPosFindZ
forward AC_MODE_OnPlayerConnect(playerid);
#if defined OnPlayerConnect
#undef OnPlayerConnect
#endif
#define OnPlayerConnect AC_MODE_OnPlayerConnect
stock AC_M_TogglePlayerControllable(playerid,toggle)
	return CallRemoteFunction("AC_TogglePlayerControllable","ii",playerid,toggle);
#define TogglePlayerControllable AC_M_TogglePlayerControllable
forward OnPlayerCheat(playerid,code);
forward StartAntiCheat ( playerid );
stock AC_MODE_TogglePlayerSpectating(playerid, toggle)
	return CallRemoteFunction("AC_TogglePlayerSpectating","ii",playerid,toggle);
#define TogglePlayerSpectating AC_MODE_TogglePlayerSpectating
forward AC_BACK_OnPlayerDeath(playerid,killerid,reason);
#define OnPlayerDeath AC_BACK_OnPlayerDeath