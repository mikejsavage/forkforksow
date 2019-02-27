/*
Copyright (C) 1997-2001 Id Software, Inc.

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/
#include "g_local.h"

//QUAKED target_explosion (1 0 0) (-8 -8 -8) (8 8 8)
//Spawns an explosion temporary entity when used.
//
//"delay"		wait this long before going off
//"dmg"		how much radius damage should be done, defaults to 0
static void target_explosion_explode( edict_t *self ) {
	float save;
	int radius;

	G_RadiusDamage( self, self->activator, NULL, NULL, MOD_EXPLOSIVE );

	if( ( self->projectileInfo.radius * 1 / 8 ) > 255 ) {
		radius = ( self->projectileInfo.radius * 1 / 16 ) & 0xFF;
		if( radius < 1 ) {
			radius = 1;
		}
		G_SpawnEvent( EV_EXPLOSION2, radius, self->s.origin );
	} else {
		radius = ( self->projectileInfo.radius * 1 / 8 ) & 0xFF;
		if( radius < 1 ) {
			radius = 1;
		}
		G_SpawnEvent( EV_EXPLOSION1, radius, self->s.origin );
	}

	save = self->delay;
	self->delay = 0;
	G_UseTargets( self, self->activator );
	self->delay = save;
}

static void use_target_explosion( edict_t *self, edict_t *other, edict_t *activator ) {
	self->activator = activator;

	if( !self->delay ) {
		target_explosion_explode( self );
		return;
	}

	self->think = target_explosion_explode;
	self->nextThink = level.time + self->delay * 1000;
}

void SP_target_explosion( edict_t *self ) {
	self->use = use_target_explosion;
	self->r.svflags = SVF_NOCLIENT;

	self->projectileInfo.maxDamage = max( self->dmg, 1 );
	self->projectileInfo.minDamage = min( self->dmg, 1 );
	self->projectileInfo.maxKnockback = self->projectileInfo.maxDamage;
	self->projectileInfo.minKnockback = self->projectileInfo.minDamage;
	self->projectileInfo.radius = st.radius;
	if( !self->projectileInfo.radius ) {
		self->projectileInfo.radius = self->dmg + 100;
	}
}

//==========================================================

//QUAKED target_laser (0 .5 0) (-8 -8 -8) (8 8 8) START_ON RED GREEN BLUE YELLOW ORANGE FAT
//When triggered, fires a laser.  You can either set a target or a direction.
//-------- KEYS --------
//angles: alternate "pitch, yaw, roll" angles method of aiming laser (default 0 0 0).
//target : point this to a target_position entity to set the laser's aiming direction.
//targetname : the activating trigger points to this.
//notsingle : when set to 1, entity will not spawn in Single Player mode
//notfree : when set to 1, entity will not spawn in "Free for all" and "Tournament" modes.
//notduel : when set to 1, entity will not spawn in "Teamplay" and "CTF" modes. (jal: todo)
//notteam : when set to 1, entity will not spawn in "Teamplay" and "CTF" modes.
//-------- SPAWNFLAGS --------
//START_ON : when set, the laser will start on in the game.
//RED :
//GREEN : BLUE :
//YELLOW :
//ORANGE :
//FAT :
static void target_laser_think( edict_t *self ) {
	edict_t *ignore;
	vec3_t start;
	vec3_t end;
	trace_t tr;
	vec3_t point;
	vec3_t last_movedir;
	int count;

	// our lifetime has expired
	if( self->delay && ( self->wait * 1000 < level.time ) ) {
		if( self->r.owner && self->r.owner->use ) {
			G_CallUse( self->r.owner, self, self->activator );
		}

		G_FreeEdict( self );
		return;
	}

	if( self->spawnflags & 0x80000000 ) {
		count = 8;
	} else {
		count = 4;
	}

	if( self->enemy ) {
		VectorCopy( self->moveinfo.movedir, last_movedir );
		VectorMA( self->enemy->r.absmin, 0.5, self->enemy->r.size, point );
		VectorSubtract( point, self->s.origin, self->moveinfo.movedir );
		VectorNormalize( self->moveinfo.movedir );
		if( !VectorCompare( self->moveinfo.movedir, last_movedir ) ) {
			self->spawnflags |= 0x80000000;
		}
	}

	ignore = self;
	VectorCopy( self->s.origin, start );
	VectorMA( start, 2048, self->moveinfo.movedir, end );
	VectorClear( tr.endpos ); // shut up compiler
	while( 1 ) {
		G_Trace( &tr, start, NULL, NULL, end, ignore, MASK_SHOT );
		if( tr.fraction == 1 ) {
			break;
		}

		// hurt it if we can
		if( ( game.edicts[tr.ent].takedamage ) && !( game.edicts[tr.ent].flags & FL_IMMUNE_LASER ) ) {
			if( game.edicts[tr.ent].r.client && self->activator->r.client ) {
				if( !GS_TeamBasedGametype() ||
					game.edicts[tr.ent].s.team != self->activator->s.team ) {
					G_Damage( &game.edicts[tr.ent], self, self->activator, self->moveinfo.movedir, self->moveinfo.movedir, tr.endpos, self->dmg, 1, 0, self->count );
				}
			} else {
				G_Damage( &game.edicts[tr.ent], self, self->activator, self->moveinfo.movedir, self->moveinfo.movedir, tr.endpos, self->dmg, 1, 0, self->count );
			}
		}

		// if we hit something that's not a monster or player or is immune to lasers, we're done
		if( !game.edicts[tr.ent].r.client ) {
			if( self->spawnflags & 0x80000000 ) {
				edict_t *event;

				self->spawnflags &= ~0x80000000;

				event = G_SpawnEvent( EV_LASER_SPARKS, DirToByte( tr.plane.normal ), tr.endpos );
				event->s.counterNum = count;
				event->s.colorRGBA = self->s.colorRGBA;
			}
			break;
		}

		ignore = &game.edicts[tr.ent];
		VectorCopy( tr.endpos, start );
	}

	VectorCopy( tr.endpos, self->s.origin2 );
	G_SetBoundsForSpanEntity( self, 8 );

	GClip_LinkEntity( self );

	self->nextThink = level.time + 1;
}

static void target_laser_on( edict_t *self ) {
	if( !self->activator ) {
		self->activator = self;
	}
	self->spawnflags |= 0x80000001;
	self->r.svflags &= ~SVF_NOCLIENT;
	self->wait = ( level.time * 0.001 ) + self->delay;
	target_laser_think( self );
}

static void target_laser_off( edict_t *self ) {
	self->spawnflags &= ~1;
	self->r.svflags |= SVF_NOCLIENT;
	self->nextThink = 0;
}

static void target_laser_use( edict_t *self, edict_t *other, edict_t *activator ) {
	self->activator = activator;
	if( self->spawnflags & 1 ) {
		target_laser_off( self );
	} else {
		target_laser_on( self );
	}
}

void target_laser_start( edict_t *self ) {
	edict_t *ent;

	self->movetype = MOVETYPE_NONE;
	self->r.solid = SOLID_NOT;
	self->s.type = ET_BEAM;
	self->s.modelindex = 1;     // must be non-zero
	self->r.svflags = 0;

	// set the beam diameter
	if( self->spawnflags & 64 ) {
		self->s.frame = 16;
	} else {
		self->s.frame = 4;
	}

	// set the color
	if( self->spawnflags & 2 ) {
		self->s.colorRGBA = COLOR_RGBA( 220, 0, 0, 76 );
	} else if( self->spawnflags & 4 ) {
		self->s.colorRGBA = COLOR_RGBA( 0, 220, 0, 76 );
	} else if( self->spawnflags & 8 ) {
		self->s.colorRGBA = COLOR_RGBA( 0, 0, 220, 76 );
	} else if( self->spawnflags & 16 ) {
		self->s.colorRGBA = COLOR_RGBA( 220, 220, 0, 76 );
	} else if( self->spawnflags & 32 ) {
		self->s.colorRGBA = COLOR_RGBA( 255, 255, 0, 76 );
	}

	if( !self->enemy ) {
		if( self->target ) {
			ent = G_Find( NULL, FOFS( targetname ), self->target );
			if( !ent ) {
				if( developer->integer ) {
					G_Printf( "%s at %s: %s is a bad target\n", self->classname, vtos( self->s.origin ), self->target );
				}
			}
			self->enemy = ent;
		} else {
			G_SetMovedir( self->s.angles, self->moveinfo.movedir );
		}
	}
	self->use = target_laser_use;
	self->think = target_laser_think;

	if( !self->dmg ) {
		self->dmg = 1;
	}

	if( self->spawnflags & 1 ) {
		target_laser_on( self );
	} else {
		target_laser_off( self );
	}
}

void SP_target_laser( edict_t *self ) {
	// let everything else get spawned before we start firing
	self->think = target_laser_start;
	self->nextThink = level.time + 1000;
	self->count = MOD_TARGET_LASER;
}

//QUAKED target_position (0 .5 0) (-8 -8 -8) (8 8 8)
//Aiming target for entities like light and trigger_push (jump pads) in particular.
//-------- KEYS --------
//targetname : the entity that requires an aiming direction points to this.
//notsingle : when set to 1, entity will not spawn in Single Player mode
//notfree : when set to 1, entity will not spawn in "Free for all" and "Tournament" modes.
//notduel : when set to 1, entity will not spawn in "Teamplay" and "CTF" modes. (jal: todo)
//notteam : when set to 1, entity will not spawn in "Teamplay" and "CTF" modes.
//-------- NOTES --------
//To make a jump pad, place this entity at the highest point of the jump and target it with a trigger_push entity.

void SP_target_position( edict_t *self ) {
	self->r.svflags |= SVF_NOCLIENT;
}

//QUAKED target_print (0 .5 0) (-8 -8 -8) (8 8 8) SAMETEAM OTHERTEAM PRIVATE
//This will print a message on the center of the screen when triggered. By default, all the clients will see the message.
//-------- KEYS --------
//message : text string to print on screen.
//targetname : the activating trigger points to this.
//notsingle : when set to 1, entity will not spawn in Single Player mode
//notfree : when set to 1, entity will not spawn in "Free for all" and "Tournament" modes.
//notduel : when set to 1, entity will not spawn in "Teamplay" and "CTF" modes. (jal: todo)
//notteam : when set to 1, entity will not spawn in "Teamplay" and "CTF" modes.
//-------- SPAWNFLAGS --------
//SAMETEAM : &1 only players in activator's team will see the message.
//OTHERTEAM : &2 only players in other than activator's team will see the message.
//PRIVATE : &4 only the player that activates the target will see the message.

static void SP_target_print_use( edict_t *self, edict_t *other, edict_t *activator ) {
	if( activator->r.client && ( self->spawnflags & 4 ) ) {
		G_CenterPrintMsg( activator, "%s", self->message );
		return;
	}

	// print to team
	if( activator->r.client && self->spawnflags & 3 ) {
		edict_t *e;
		for( e = game.edicts + 1; PLAYERNUM( e ) < gs.maxclients; e++ ) {
			if( e->r.inuse && e->s.team ) {
				if( self->spawnflags & 1 && e->s.team == activator->s.team ) {
					G_CenterPrintMsg( e, "%s", self->message );
				}
				if( self->spawnflags & 2 && e->s.team != activator->s.team ) {
					G_CenterPrintMsg( e, "%s", self->message );
				}
			}
		}
		return;
	}

	for( int i = 1; i <= gs.maxclients; i++ ) {
		edict_t *player = &game.edicts[i];
		if( !player->r.inuse ) {
			continue;
		}

		G_CenterPrintMsg( player, "%s", self->message );
	}
}

void SP_target_print( edict_t *self ) {
	if( !self->message ) {
		G_FreeEdict( self );
		return;
	}

	self->use = SP_target_print_use;
}


//==========================================================

static void target_delay_think( edict_t *ent ) {
	G_UseTargets( ent, ent->activator );
}

static void target_delay_use( edict_t *ent, edict_t *other, edict_t *activator ) {
	ent->nextThink = level.time + 1000 * ( ent->wait + ent->random * crandom() );
	ent->think = target_delay_think;
	ent->activator = activator;
}

//QUAKED target_delay (1 0 0) (-8 -8 -8) (8 8 8)
//"wait" seconds to pause before firing targets.
//"random" delay variance, total delay = delay +/- random seconds
void SP_target_delay( edict_t *ent ) {
	// check the "delay" key for backwards compatibility with Q3 maps
	if( ent->delay ) {
		ent->wait = ent->delay;
	}
	if( !ent->wait ) {
		ent->wait = 1.0;
	}

	ent->delay = 0;
	ent->use = target_delay_use;
}


//==========================================================

//QUAKED target_teleporter (1 0 0) (-8 -8 -8) (8 8 8)
//The activator will be teleported away.
//-------- KEYS --------
//target : point this to a misc_teleporter_dest entity to set the teleport destination.
//targetname : activating trigger points to this.
//notsingle : when set to 1, entity will not spawn in Single Player mode
//notfree : when set to 1, entity will not spawn in "Free for all" and "Tournament" modes.
//notduel : when set to 1, entity will not spawn in "Teamplay" and "CTF" modes. (jal: todo)
//notteam : when set to 1, entity will not spawn in "Teamplay" and "CTF" modes.
//-------- SPAWNFLAGS --------
//SPECTATOR : &1 only teleport players moving in spectator mode

static void target_teleporter_use( edict_t *self, edict_t *other, edict_t *activator ) {
	edict_t *dest;

	if( !G_PlayerCanTeleport( activator ) ) {
		return;
	}

	if( ( self->s.team != TEAM_SPECTATOR ) && ( self->s.team != activator->s.team ) ) {
		return;
	}
	if( self->spawnflags & 1 && activator->r.client->ps.pmove.pm_type != PM_SPECTATOR ) {
		return;
	}

	dest = G_Find( NULL, FOFS( targetname ), self->target );
	if( !dest ) {
		if( developer->integer ) {
			G_Printf( "Couldn't find destination.\n" );
		}
		return;
	}

	G_TeleportPlayer( activator, dest );
}

void SP_target_teleporter( edict_t *self ) {
	self->r.svflags |= SVF_NOCLIENT;

	if( !self->targetname ) {
		if( developer->integer ) {
			G_Printf( "untargeted %s at %s\n", self->classname, vtos( self->s.origin ) );
		}
	}

	if( st.gameteam >= TEAM_SPECTATOR && st.gameteam < GS_MAX_TEAMS ) {
		self->s.team = st.gameteam;
	} else {
		self->s.team = TEAM_SPECTATOR;
	}

	self->use = target_teleporter_use;
}


//==========================================================

//QUAKED target_kill (.5 .5 .5) (-8 -8 -8) (8 8 8)
//Kills the activator.

static void target_kill_use( edict_t *self, edict_t *other, edict_t *activator ) {
	G_Damage( activator, self, world, vec3_origin, vec3_origin, activator->s.origin, 100000, 0, DAMAGE_NO_PROTECTION, MOD_TRIGGER_HURT );
}

void SP_target_kill( edict_t *self ) {
	self->r.svflags |= SVF_NOCLIENT;
	self->use = target_kill_use;
}
