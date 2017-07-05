/*  surface.inc
 *
 *  (c) Copyright 2015, Emilijo "Correlli" Lovrich
 *
 *  Credits: - Incognito for streamer plugin,
 *			 - Y_Less for foreach/iterator.
*/

#define MAX_SURFACES															(10)
#define MAX_SURFACE_OBJECTS														(10000)

#define S_X_OFFSET																(0)
#define S_Y_OFFSET																(1)
#define S_HEIGHT																(2)
#define S_X_START																(3)
#define S_Y_START																(4)
#define S_X_END																	(5)
#define S_Y_END																	(6)
#define S_X_ROT																	(7)
#define S_Y_ROT																	(8)
#define S_Z_ROT																	(9)
#define S_STREAM																(10)
#define S_DRAW																	(11)

#define OBJ_DBG_MSG

enum DataSurface
{
	Model,
	World,
	Interior,
	Player,
	Float:Property_Float[12],
	Object[MAX_SURFACE_OBJECTS],
	ObjectCount,
	HorizontalCount,
	VerticalCount
}

new
		SurfaceData[MAX_SURFACES][DataSurface], g_Count, Iterator:Iter_Surface<MAX_SURFACES>;

stock CreateSurface(object_model, Float:x_off, Float:y_off, Float:height, Float:x_start, Float:y_start, Float:x_end = 0.0, Float:y_end = 0.0, Float:x_rot = 0.0, Float:y_rot = 0.0, Float:z_rot = 0.0, world = -1, interior = -1, player = -1, Float:d_stream = 200.0, Float:d_draw = 0.0)
{
	new
			s_id = Iter_Free(Iter_Surface);
	if(s_id == -1)
		return -1;

	/*

	* Example (this covers the whole San Andreas map with breakable glass objects (model: 1649)):

	* -3000.0 -> 3000.0 (3000.0 + 3000.0 = 6000.0)

	* x_off = 4.4
	* 6000.0 / 4.4 = 1363.636 ~= 1364

	* y_off = 3.31
	* 6000.0 / 3.31 = 1812.6888 ~= 1813

	* 1364 x 1813 = 2472932 objects in total

	*/

	Surface_FixPos(x_start, y_start, x_end, y_end);

	if(x_start == x_end || y_start == y_end)
	{
		printf("Error at \"CreateSurface\" function for model %i - one of the starting and ending positions (X: %0.2f, Y: %0.2f) is the same.",
			object_model,
			x_start,
			y_start
		);
		return -2;
	}

	Iter_Add(Iter_Surface, s_id);

	SurfaceData[s_id][Model]						=		 object_model;
	SurfaceData[s_id][World]						=				world;
	SurfaceData[s_id][Interior]						=			 interior;
	SurfaceData[s_id][Player]						=			   player;
	SurfaceData[s_id][Property_Float][S_X_OFFSET]	=				x_off;
	SurfaceData[s_id][Property_Float][S_Y_OFFSET]	=				y_off;
	SurfaceData[s_id][Property_Float][S_HEIGHT]		=			   height;
	SurfaceData[s_id][Property_Float][S_X_START]	=			  x_start;
	SurfaceData[s_id][Property_Float][S_Y_START]	=			  y_start;
	SurfaceData[s_id][Property_Float][S_X_END]		=				x_end;
	SurfaceData[s_id][Property_Float][S_Y_END]		=				y_end;
	SurfaceData[s_id][Property_Float][S_X_ROT]		=				x_rot;
	SurfaceData[s_id][Property_Float][S_Y_ROT]		=				y_rot;
	SurfaceData[s_id][Property_Float][S_Z_ROT]		=				z_rot;
	SurfaceData[s_id][Property_Float][S_STREAM]		=			 d_stream;
	SurfaceData[s_id][Property_Float][S_DRAW]		=			   d_draw;

	for(new a = 0; a < MAX_SURFACE_OBJECTS; a++)
		SurfaceData[s_id][Object][a]				=	INVALID_OBJECT_ID;

	UpdateSurface(s_id);
	return s_id;
}

stock DestroySurface(s_id)
{
	if(!Iter_Contains(Iter_Surface, s_id))
		return false;

	new
			a;

	Iter_Remove(Iter_Surface, s_id);

	SurfaceData[s_id][Model]						=					0;
	SurfaceData[s_id][World]						=					0;
	SurfaceData[s_id][Interior]						=					0;
	SurfaceData[s_id][Player]						=	INVALID_PLAYER_ID;
	for(a = 0; a < 12; a++)
		SurfaceData[s_id][Property_Float][a]		=				  0.0;
	for(a = 0; a < SurfaceData[s_id][ObjectCount]; a++)
	{
		if(IsValidDynamicObject(SurfaceData[s_id][Object][a]))
		{
			DestroyDynamicObject(SurfaceData[s_id][Object][a]);
			SurfaceData[s_id][Object][a]			=	INVALID_OBJECT_ID;
		}
	}
	SurfaceData[s_id][ObjectCount]					=					0;
	SurfaceData[s_id][HorizontalCount]				=					0;
	SurfaceData[s_id][VerticalCount]				=					0;
	return true;
}

stock SetSurfaceTexture(s_id, m_index, txd_model, txd_name[], texture_name[], m_color = 0)
{
	if(!Iter_Contains(Iter_Surface, s_id))
		return false;

	for(new a = 0; a < SurfaceData[s_id][ObjectCount]; a++)
	{
		if(IsValidDynamicObject(SurfaceData[s_id][Object][a]))
			SetDynamicObjectMaterial(SurfaceData[s_id][Object][a], m_index, txd_model, txd_name, texture_name, m_color);
	}
	return true;
}

stock SetSurfaceObjectTexture(s_id, slot, m_index, txd_model, txd_name[], texture_name[], m_color = 0)
{
	if(!Iter_Contains(Iter_Surface, s_id))
		return false;
	if(slot < 0 || slot >= MAX_SURFACE_OBJECTS)
		return false;
	if(!IsValidDynamicObject(SurfaceData[s_id][Object][slot]))
		return false;
	SetDynamicObjectMaterial(SurfaceData[s_id][Object][slot], m_index, txd_model, txd_name, texture_name, m_color);
	return true;
}

stock SetSurfaceModel(s_id, object_model, Float:x_off, Float:y_off)
{
	if(!Iter_Contains(Iter_Surface, s_id))
		return false;

	for(new a = 0; a < SurfaceData[s_id][ObjectCount]; a++)
	{
		if(IsValidDynamicObject(SurfaceData[s_id][Object][a]))
			Streamer_SetIntData(STREAMER_TYPE_OBJECT, SurfaceData[s_id][Object][a], E_STREAMER_MODEL_ID, object_model);
	}

	SurfaceData[s_id][Model]						=	object_model;
	SurfaceData[s_id][Property_Float][S_X_OFFSET]	=		   x_off;
	SurfaceData[s_id][Property_Float][S_Y_OFFSET]	=		   y_off;

	UpdateSurface(s_id);
	return true;
}

stock GetSurfaceModel(s_id)
{
	if(!Iter_Contains(Iter_Surface, s_id))
		return false;
	return SurfaceData[s_id][Model];
}

stock SetSurfacePos(s_id, Float:x_start, Float:y_start, Float:x_end = 0.0, Float:y_end = 0.0)
{
	if(!Iter_Contains(Iter_Surface, s_id))
		return false;

	Surface_FixPos(x_start, y_start, x_end, y_end);

	if(x_start == x_end || y_start == y_end)
	{
		printf("Error at \"SetSurfacePos\" function for surface ID %i - one of the starting and ending positions (X: %0.2f, Y: %0.2f) is the same.",
			s_id,
			x_start,
			y_start
		);
		return false;
	}

	SurfaceData[s_id][Property_Float][S_X_START]	=		 x_start;
	SurfaceData[s_id][Property_Float][S_Y_START]	=		 y_start;
	SurfaceData[s_id][Property_Float][S_X_END]		=		   x_end;
	SurfaceData[s_id][Property_Float][S_Y_END]		=		   y_end;

	UpdateSurface(s_id);
	return true;
}

stock GetSurfacePos(s_id, &Float:x_start, &Float:y_start, &Float:x_end, &Float:y_end)
{
	if(!Iter_Contains(Iter_Surface, s_id))
		return false;

	x_start		=	SurfaceData[s_id][Property_Float][S_X_START];
	y_start		=	SurfaceData[s_id][Property_Float][S_Y_START];
	x_end		=	SurfaceData[s_id][Property_Float][S_X_END]	;
	y_end		=	SurfaceData[s_id][Property_Float][S_Y_END]	;
	return true;
}

stock SetSurfaceHeight(s_id, Float:height)
{
	if(!Iter_Contains(Iter_Surface, s_id))
		return false;

	new
			Float:pos[3];

	for(new a = 0; a < SurfaceData[s_id][ObjectCount]; a++)
	{
		if(IsValidDynamicObject(SurfaceData[s_id][Object][a]))
		{
			GetDynamicObjectPos(SurfaceData[s_id][Object][a], pos[0], pos[1], pos[2]);
			SetDynamicObjectPos(SurfaceData[s_id][Object][a], pos[0], pos[1], height);
		}
	}

	SurfaceData[s_id][Property_Float][S_HEIGHT] = height;
	return true;
}

stock Float:GetSurfaceHeight(s_id)
{
	if(!Iter_Contains(Iter_Surface, s_id))
		return -100.0;
	return SurfaceData[s_id][Property_Float][S_HEIGHT];
}

stock SetSurfaceRot(s_id, Float:x_rot, Float:y_rot, Float:z_rot)
{
	if(!Iter_Contains(Iter_Surface, s_id))
		return false;

	for(new a = 0; a < SurfaceData[s_id][ObjectCount]; a++)
	{
		if(IsValidDynamicObject(SurfaceData[s_id][Object][a]))
			SetDynamicObjectRot(SurfaceData[s_id][Object][a], x_rot, y_rot, z_rot);
	}

	SurfaceData[s_id][Property_Float][S_X_ROT] = x_rot;
	SurfaceData[s_id][Property_Float][S_Y_ROT] = y_rot;
	SurfaceData[s_id][Property_Float][S_Z_ROT] = z_rot;
	return true;
}

stock GetSurfaceRot(s_id, &Float:x_rot, &Float:y_rot, &Float:z_rot)
{
	if(!Iter_Contains(Iter_Surface, s_id))
		return false;

	x_rot = SurfaceData[s_id][Property_Float][S_X_ROT];
	y_rot = SurfaceData[s_id][Property_Float][S_Y_ROT];
	z_rot = SurfaceData[s_id][Property_Float][S_Z_ROT];
	return true;
}

stock SetSurfaceStreamDistance(s_id, Float:d_stream)
{
	if(!Iter_Contains(Iter_Surface, s_id))
		return false;

	for(new a = 0; a < SurfaceData[s_id][ObjectCount]; a++)
	{
		if(IsValidDynamicObject(SurfaceData[s_id][Object][a]))
			Streamer_SetFloatData(STREAMER_TYPE_OBJECT, SurfaceData[s_id][Object][a], E_STREAMER_STREAM_DISTANCE, d_stream);
	}

	SurfaceData[s_id][Property_Float][S_STREAM] = d_stream;
	return true;
}

stock Float:GetSurfaceStreamDistance(s_id)
{
	if(!Iter_Contains(Iter_Surface, s_id))
		return 0.0;
	return SurfaceData[s_id][Property_Float][S_STREAM];
}

stock SetSurfaceDrawDistance(s_id, Float:d_draw)
{
	if(!Iter_Contains(Iter_Surface, s_id))
		return false;

	for(new a = 0; a < SurfaceData[s_id][ObjectCount]; a++)
	{
		if(IsValidDynamicObject(SurfaceData[s_id][Object][a]))
			Streamer_SetFloatData(STREAMER_TYPE_OBJECT, SurfaceData[s_id][Object][a], E_STREAMER_DRAW_DISTANCE, d_draw);
	}

	SurfaceData[s_id][Property_Float][S_DRAW] = d_draw;
	return true;
}

stock Float:GetSurfaceDrawDistance(s_id)
{
	if(!Iter_Contains(Iter_Surface, s_id))
		return -1.0;
	return SurfaceData[s_id][Property_Float][S_DRAW];
}

stock SetSurfaceWorld(s_id, world)
{
	if(!Iter_Contains(Iter_Surface, s_id))
		return false;

	for(new a = 0; a < SurfaceData[s_id][ObjectCount]; a++)
	{
		if(IsValidDynamicObject(SurfaceData[s_id][Object][a]))
			Streamer_SetIntData(STREAMER_TYPE_OBJECT, SurfaceData[s_id][Object][a], E_STREAMER_WORLD_ID, world);
	}

	SurfaceData[s_id][World] = world;
	return true;
}

stock GetSurfaceWorld(s_id)
{
	if(!Iter_Contains(Iter_Surface, s_id))
		return -2;
	return SurfaceData[s_id][World];
}

stock SetSurfaceInterior(s_id, interior)
{
	if(!Iter_Contains(Iter_Surface, s_id))
		return false;

	for(new a = 0; a < SurfaceData[s_id][ObjectCount]; a++)
	{
		if(IsValidDynamicObject(SurfaceData[s_id][Object][a]))
			Streamer_SetIntData(STREAMER_TYPE_OBJECT, SurfaceData[s_id][Object][a], E_STREAMER_INTERIOR_ID, interior);
	}

	SurfaceData[s_id][Interior] = interior;
	return true;
}

stock GetSurfaceInterior(s_id)
{
	if(!Iter_Contains(Iter_Surface, s_id))
		return -2;
	return SurfaceData[s_id][Interior];
}

stock SetSurfacePlayer(s_id, player)
{
	if(!Iter_Contains(Iter_Surface, s_id))
		return false;
	if(!IsPlayerConnected(player) && player != -1)
		return false;

	for(new a = 0; a < SurfaceData[s_id][ObjectCount]; a++)
	{
		if(IsValidDynamicObject(SurfaceData[s_id][Object][a]))
			Streamer_SetIntData(STREAMER_TYPE_OBJECT, SurfaceData[s_id][Object][a], E_STREAMER_PLAYER_ID, player);
	}

	SurfaceData[s_id][Player] = player;
	return true;
}

stock GetSurfacePlayer(s_id)
{
	if(!Iter_Contains(Iter_Surface, s_id))
		return -2;
	return SurfaceData[s_id][Player];
}

stock GetSurfaceObjectCount(s_id)
{
	if(!Iter_Contains(Iter_Surface, s_id))
		return false;
	return SurfaceData[s_id][ObjectCount];
}

stock GetSurfaceHorizontalObjects(s_id)
{
	if(!Iter_Contains(Iter_Surface, s_id))
		return false;
	return SurfaceData[s_id][HorizontalCount];
}

stock GetSurfaceVerticalObjects(s_id)
{
	if(!Iter_Contains(Iter_Surface, s_id))
		return false;
	return SurfaceData[s_id][VerticalCount];
}

stock GetSurfaceSurplusObjects(s_id)
{
	if(!Iter_Contains(Iter_Surface, s_id))
		return -1;
	return (SurfaceData[s_id][ObjectCount] - (SurfaceData[s_id][HorizontalCount] * SurfaceData[s_id][VerticalCount]));
}

stock UpdateSurface(s_id)
{
	if(!Iter_Contains(Iter_Surface, s_id))
		return false;

	new
			Float:coord[2], bool:calculate[2], a, bool:loop_break = false;

	if(SurfaceData[s_id][Property_Float][S_X_START] < 0.0)
	{
		// -3000.0, -4000.0 -> 1000.0
		// -4000.0, -3000.0 -> 1000.0
		if(SurfaceData[s_id][Property_Float][S_X_END] < 0.0)
		{
			if(SurfaceData[s_id][Property_Float][S_X_START] > SurfaceData[s_id][Property_Float][S_X_END])
				calculate[0] = true;
			coord[0] = floatabs(SurfaceData[s_id][Property_Float][S_X_START] + floatabs(SurfaceData[s_id][Property_Float][S_X_END]));
		}
		// -3000.0, 4000.0 -> 7000.0
		else
			coord[0] = floatabs(SurfaceData[s_id][Property_Float][S_X_START]) + SurfaceData[s_id][Property_Float][S_X_END];
	}
	else
	{
		// 3000.0, -4000.0 -> 7000.0
		if(SurfaceData[s_id][Property_Float][S_X_END] < 0.0)
		{
			calculate[0] = true;
			coord[0] = SurfaceData[s_id][Property_Float][S_X_START] + floatabs(SurfaceData[s_id][Property_Float][S_X_END]);
		}
		// 3000.0, 4000.0 -> 1000.0
		// 4000.0, 3000.0 -> 1000.0
		else
		{
			if(SurfaceData[s_id][Property_Float][S_X_START] > SurfaceData[s_id][Property_Float][S_X_END])
				calculate[0] = true;
			coord[0] = floatabs(SurfaceData[s_id][Property_Float][S_X_START] - SurfaceData[s_id][Property_Float][S_X_END]);
		}
	}

	if(SurfaceData[s_id][Property_Float][S_Y_START] < 0.0)
	{
		// -3000.0, -4000.0 -> 1000.0
		// -4000.0, -3000.0 -> 1000.0
		if(SurfaceData[s_id][Property_Float][S_Y_END] < 0.0)
		{
			if(SurfaceData[s_id][Property_Float][S_Y_START] > SurfaceData[s_id][Property_Float][S_Y_END])
				calculate[1] = true;
			coord[1] = floatabs(SurfaceData[s_id][Property_Float][S_Y_START] + floatabs(SurfaceData[s_id][Property_Float][S_Y_END]));
		}
		// -3000.0, 4000.0 -> 7000.0
		else
			coord[1] = floatabs(SurfaceData[s_id][Property_Float][S_Y_START]) + SurfaceData[s_id][Property_Float][S_Y_END];
	}
	else
	{
		// 3000.0, -4000.0 -> 7000.0
		if(SurfaceData[s_id][Property_Float][S_Y_END] < 0.0)
		{
			calculate[1] = true;
			coord[1] = SurfaceData[s_id][Property_Float][S_Y_START] + floatabs(SurfaceData[s_id][Property_Float][S_Y_END]);
		}
		// 3000.0, 4000.0 -> 1000.0
		// 4000.0, 3000.0 -> 1000.0
		else
		{
			if(SurfaceData[s_id][Property_Float][S_Y_START] > SurfaceData[s_id][Property_Float][S_Y_END])
				calculate[1] = true;
			coord[1] = floatabs(SurfaceData[s_id][Property_Float][S_Y_START] - SurfaceData[s_id][Property_Float][S_Y_END]);
		}
	}

	SurfaceData[s_id][HorizontalCount]	=	floatround(coord[0] / SurfaceData[s_id][Property_Float][S_X_OFFSET], floatround_ceil);
	SurfaceData[s_id][VerticalCount]	=	floatround(coord[1] / SurfaceData[s_id][Property_Float][S_Y_OFFSET], floatround_ceil);

	g_Count = 0;

	for(a = 0; a < SurfaceData[s_id][HorizontalCount]; a++)
	{
		for(new b = 0; b < SurfaceData[s_id][VerticalCount]; b++)
		{
			if(g_Count == MAX_SURFACE_OBJECTS)
			{
				printf("Error at \"UpdateSurface\" function for model %i (surface ID: %i) - limit for objects per surface is reached (%i/%i). Change the \"MAX_SURFACE_OBJECTS\" definition in your \"surface.inc\" include file if you want to have more objects per surface.",
					SurfaceData[s_id][Model],
					s_id,
					g_Count,
					(SurfaceData[s_id][HorizontalCount] * SurfaceData[s_id][VerticalCount])
				);

				SurfaceData[s_id][HorizontalCount]	=	(a + 1);
				SurfaceData[s_id][VerticalCount]	=	(b + 1);

				loop_break = true;
				break;
			}

			if(calculate[0])	coord[0] = SurfaceData[s_id][Property_Float][S_X_START] - (SurfaceData[s_id][Property_Float][S_X_OFFSET] * a);
			else				coord[0] = SurfaceData[s_id][Property_Float][S_X_START] + (SurfaceData[s_id][Property_Float][S_X_OFFSET] * a);

			if(calculate[1])	coord[1] = SurfaceData[s_id][Property_Float][S_Y_START] - (SurfaceData[s_id][Property_Float][S_Y_OFFSET] * b);
			else				coord[1] = SurfaceData[s_id][Property_Float][S_Y_START] + (SurfaceData[s_id][Property_Float][S_Y_OFFSET] * b);

			if(!IsValidDynamicObject(SurfaceData[s_id][Object][g_Count]))
			{
				SurfaceData[s_id][Object][g_Count] = CreateDynamicObject(
					SurfaceData[s_id][Model],
					coord[0],
					coord[1],
					SurfaceData[s_id][Property_Float][S_HEIGHT],
					SurfaceData[s_id][Property_Float][S_X_ROT],
					SurfaceData[s_id][Property_Float][S_Y_ROT],
					SurfaceData[s_id][Property_Float][S_Z_ROT],
					SurfaceData[s_id][World],
					SurfaceData[s_id][Interior],
					SurfaceData[s_id][Player],
					SurfaceData[s_id][Property_Float][S_STREAM],
					SurfaceData[s_id][Property_Float][S_DRAW]
				);
			}
			else
			{
				SetDynamicObjectPos(
					SurfaceData[s_id][Object][g_Count],
					coord[0],
					coord[1],
					SurfaceData[s_id][Property_Float][S_HEIGHT]
				);

				SetDynamicObjectRot(
					SurfaceData[s_id][Object][g_Count],
					SurfaceData[s_id][Property_Float][S_X_ROT],
					SurfaceData[s_id][Property_Float][S_Y_ROT],
					SurfaceData[s_id][Property_Float][S_Z_ROT]
				);
			}

			g_Count++;
		}

		if(loop_break)
			break;
	}

	if(g_Count < SurfaceData[s_id][ObjectCount])
	{
		for(a = g_Count; a < MAX_SURFACE_OBJECTS; a++)
		{
			if(IsValidDynamicObject(SurfaceData[s_id][Object][a]))
			{
				DestroyDynamicObject(SurfaceData[s_id][Object][a]);
				SurfaceData[s_id][Object][a] = INVALID_OBJECT_ID;
			}
		}
	}

	SurfaceData[s_id][ObjectCount] = g_Count;

	#if defined OBJ_DBG_MSG
		printf("Created/updated %i objects at \"UpdateSurface\" function for model %i (surface ID: %i).",
			g_Count,
			SurfaceData[s_id][Model],
			s_id
		);
	#endif
	return g_Count;
}

stock Surface_FixPos(&Float:x_start, &Float:y_start, &Float:x_end, &Float:y_end)
{
	if(!x_end)
	{
		if(x_start < 0.0)	x_end = floatabs(x_start);
		else				x_end = -x_start;
	}

	if(!y_end)
	{
		if(y_start < 0.0)	y_end = floatabs(y_start);
		else				y_end = -y_start;
	}
}