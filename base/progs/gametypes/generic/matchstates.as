/*
Copyright (C) 2009-2010 Chasseur de bots

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

void GENERIC_SetUpWarmup()
{
	int j;
	Team @team;

	gametype.shootingDisabled = false;
	gametype.readyAnnouncementEnabled = true;
	gametype.scoreAnnouncementEnabled = false;
	gametype.countdownEnabled = false;

	if ( gametype.isTeamBased )
	{
		bool anyone = false;
		int t;

		for ( t = TEAM_ALPHA; t < GS_MAX_TEAMS; t++ )
		{
			@team = @G_GetTeam( t );

			if ( team.unlock() )
				anyone = true;
		}

		if ( anyone )
			G_PrintMsg( null, "Teams unlocked.\n" );
	}
	else
	{
		@team = @G_GetTeam( TEAM_PLAYERS );

		if ( team.unlock() )
			G_PrintMsg( null, "Teams unlocked.\n" );
	}
}

void GENERIC_SetUpCountdown()
{
	gametype.shootingDisabled = false;
	gametype.readyAnnouncementEnabled = false;
	gametype.scoreAnnouncementEnabled = false;
	gametype.countdownEnabled = true;

	// lock teams
	bool anyone = false;
	if ( gametype.isTeamBased )
	{
		for ( int team = TEAM_ALPHA; team < GS_MAX_TEAMS; team++ )
		{
			if ( G_GetTeam( team ).lock() )
				anyone = true;
		}
	}
	else
	{
		if ( G_GetTeam( TEAM_PLAYERS ).lock() )
			anyone = true;
	}

	if ( anyone )
		G_PrintMsg( null, "Teams locked.\n" );

	// Countdowns should be made entirely client side, because we now can

	uint64 sound = Hash64( "sounds/announcer/countdown/get_ready_to_fight0" + random_uniform( 1, 3 ) );
	G_AnnouncerSound( null, sound, GS_MAX_TEAMS, false, null );
}

void GENERIC_SetUpMatch()
{
	int i, j;
	Entity @ent;
	Team @team;

	G_RemoveAllProjectiles();
	gametype.shootingDisabled = true;  // avoid shooting before "FIGHT!"
	gametype.readyAnnouncementEnabled = false;
	gametype.scoreAnnouncementEnabled = true;
	gametype.countdownEnabled = true;

	// clear player stats and scores, team scores and respawn clients in team lists

	match.alphaScore = 0;
	match.betaScore = 0;

	for ( i = TEAM_PLAYERS; i < GS_MAX_TEAMS; i++ )
	{
		@team = @G_GetTeam( i );

		// respawn all clients inside the playing teams
		for ( j = 0; @team.ent( j ) != null; j++ )
		{
			@ent = @team.ent( j );
			ent.client.stats.clear(); // clear player scores & stats
			ent.client.respawn( false );
		}
	}

	G_RemoveDeadBodies();

	// Countdowns should be made entirely client side, because we now can
	uint64 sound = Hash64( "sounds/announcer/countdown/fight0" + random_uniform( 1, 3 ) );
	G_AnnouncerSound( null, sound, GS_MAX_TEAMS, false, null );
	G_CenterPrintMsg( null, "FIGHT!" );

	// now we can enable shooting
	gametype.shootingDisabled = false;
}

void GENERIC_SetUpEndMatch()
{
	Client @client;

	gametype.shootingDisabled = true;
	gametype.readyAnnouncementEnabled = false;
	gametype.scoreAnnouncementEnabled = false;
	gametype.countdownEnabled = false;

	for ( int i = 0; i < maxClients; i++ )
	{
		@client = @G_GetClient( i );

		if ( client.state() >= CS_SPAWNED ) {
			client.respawn( true ); // ghost them all
		}
	}

	GENERIC_UpdateMatchScore();

	// print scores to console
	if ( gametype.isTeamBased )
	{
		G_PrintMsg( null, S_COLOR_YELLOW + "Final score: " + match.getScore() + "\n" );
	}

	uint64 sound = Hash64( "sounds/announcer/postmatch/game_over0" + random_uniform( 1, 3 ) );
	G_AnnouncerSound( null, sound, GS_MAX_TEAMS, true, null );
}

///*****************************************************************
/// MISC UTILS (this should get its own generic file
///*****************************************************************

// returns false if the target wasn't visible
bool GENERIC_LookAtEntity( Vec3 &in origin, Vec3 &in angles, Entity @lookTarget, int ignoreNum, bool lockPitch, int backOffset, int upOffset, Vec3 &out lookOrigin, Vec3 &out lookAngles )
{
	if ( @lookTarget == null )
		return false;

	bool visible = true;

	Vec3 start, end, mins, maxs, dir;
	Trace trace;

	start = end = origin;
	if ( upOffset != 0 )
	{
		end.z += upOffset;
		trace.doTrace( start, vec3Origin, vec3Origin, end, ignoreNum, MASK_OPAQUE );
		if ( trace.fraction < 1.0f )
		{
			start = trace.endPos + ( trace.planeNormal * 0.1f );
		}
	}

	lookTarget.getSize( mins, maxs );
	end = lookTarget.origin + ( 0.5 * ( maxs + mins ) );

	if ( !trace.doTrace( start, vec3Origin, vec3Origin, end, ignoreNum, MASK_OPAQUE ) )
	{
		if ( trace.entNum != lookTarget.entNum )
			visible = false;
	}

	if ( lockPitch )
		end.z = lookOrigin.z;

	if ( backOffset != 0 )
	{
		// trace backwards from dest to origin projected to backoffset
		dir = start - end;
		dir.normalize();
		Vec3 newStart = start + ( dir * backOffset );

		trace.doTrace( start, vec3Origin, vec3Origin, newStart, ignoreNum, MASK_OPAQUE );
		start = trace.endPos;
		if ( trace.fraction < 1.0f )
		{
			start += ( trace.planeNormal * 0.1f );
		}
	}

	dir = end - start;

	lookOrigin = start;
	lookAngles = dir.toAngles();

	return visible;
}

Entity @GENERIC_SelectBestRandomSpawnPoint( Entity @self, String &className )
{
	array<Entity @> @spawnents = G_FindByClassname( className );
	if( spawnents.size() == 0 )
		return null;
	return spawnents[ random_uniform( 0, spawnents.size() ) ];
}

void GENERIC_UpdateMatchScore()
{
	if( gametype.isTeamBased ) {
		match.setScore( match.alphaScore + " : " + match.betaScore );
		return;
	}

	match.setScore( "" );
}

void GENERIC_Think()
{
	GENERIC_UpdateMatchScore();
}
