#include "g_local.h"

static void SpikesTouched( edict_t * self, edict_t * other, cplane_t * plane, int surfFlags ) {
	if( self->s.linearMovementTimeStamp == 0 ) {
		self->nextThink = level.time + 1000;
		self->s.linearMovementTimeStamp = max( 1, game.serverTime );
	}
}

static void SpikesTrigger( edict_t * self );
static void SpikesRearm( edict_t * self ) {
	self->s.linearMovementTimeStamp = 0;
	self->think = SpikesTrigger;
}

static void SpikesTrigger( edict_t * self ) {
	KillBox( self );
	self->think = SpikesRearm;
	self->nextThink = level.time + 1000;
}

void SP_spikes( edict_t * spikes ) {
	spikes->r.svflags &= ~SVF_NOCLIENT | SVF_PROJECTILE;
	spikes->r.solid = SOLID_TRIGGER;

	spikes->s.angles[ PITCH ] += 90;
	vec3_t forward, right, up;
	AngleVectors( spikes->s.angles, forward, right, up );
	
	vec3_t mins, maxs;
	VectorSet( mins, 0, 0, 0 );
	VectorSet( maxs, 0, 0, 0 );
	VectorMA( mins, -64, forward, mins );
	VectorMA( mins, -64, right, mins );
	VectorMA( mins, 48, up, mins );
	VectorMA( maxs, 64, forward, maxs );
	VectorMA( maxs, 64, right, maxs );

	for( int i = 0; i < 3; i++ ) {
		spikes->r.mins[ i ] = min( mins[ i ], maxs[ i ] );
		spikes->r.maxs[ i ] = max( mins[ i ], maxs[ i ] );
	}

	spikes->s.modelindex = trap_ModelIndex( "models/objects/spikes/spikes.md3" );
	spikes->s.type = ET_SPIKES;

	spikes->touch = SpikesTouched;
	spikes->think = SpikesTrigger;

	GClip_LinkEntity( spikes );

	edict_t * base = G_Spawn();
	base->r.svflags &= ~SVF_NOCLIENT;
	VectorCopy( spikes->s.origin, base->s.origin );
	VectorCopy( spikes->s.angles, base->s.angles );
	base->s.modelindex = trap_ModelIndex( "models/objects/spikes/base.md3" );
	GClip_LinkEntity( base );
}