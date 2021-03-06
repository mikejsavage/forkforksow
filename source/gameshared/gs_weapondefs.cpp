#include "qcommon/base.h"
#include "qcommon/qcommon.h"
#include "gameshared/gs_public.h"

#define INSTANT 0

#define WEAPONDOWN_TIME 50
#define WEAPONUP_TIME_FAST 150
#define WEAPONUP_TIME_NORMAL 350
#define WEAPONUP_TIME_SLOW 750
#define WEAPONUP_TIME_VERY_SLOW 1000

const WeaponDef gs_weaponDefs[] = {
	{ "", "", WeaponCategory_Count }, // Weapon_None

	{
		"Knife", "gb",
		WeaponCategory_Count,

		6,                              // projectiles fired each shot
		0,                              // clip size
		0,                              // reload time
		false,                          // staged reloading

		//timings (in msecs)->
		WEAPONUP_TIME_FAST,             // weapon up time
		WEAPONDOWN_TIME,                // weapon down time
		500,                            // refire time
		70,                             // projectile timeout / projectile range for instant weapons
		Vec2( 0.0f, 0.0f ),             // recoil
		Vec2( 0.0f, 0.0f ),             // recoilmin
		0.0f,                           // recoil recovery
		FiringMode_Auto,

		0.0f,                           // zoom fov
		0.0f,                           // !zoom inaccuracy

		//damages
		25,                             // damage
		0,                              // selfdamage ratio
		0,                              // knockback
		0,                              // splash radius
		0,                              // splash minimum damage
		0,                              // splash minimum knockback

		//projectile def
		INSTANT,                        // speed
		45,                             // spread
	},

	{
		"9mm", "9mm",
		WeaponCategory_Backup,

		1,                              // projectiles fired each shot
		15,                             // clip size
		1500,                           // reload time
		false,                          // staged reloading

		//timings (in msecs)
		WEAPONUP_TIME_FAST,             // weapon up time
		WEAPONDOWN_TIME,                // weapon down time
		120,                            // refire time
		HITSCAN_RANGE,                  // projectile timeout
		Vec2( 80.0f, 40.0f ),           // recoil
		Vec2( 40.0f, 20.0f ),           // recoilmin
		1000.0f,                        // recoil recovery
		FiringMode_SemiAuto,

		0.0f,                           // zoom fov
		0.0f,                           // !zoom inaccuracy

		//damages
		12,                             // damage
		0,                              // selfdamage ratio
		20,                             // knockback
		0,                              // splash radius
		0,                              // splash minimum damage
		0,                              // splash minimum knockback

		//projectile def
		INSTANT,                        // speed
		0,                              // spread
	},

	{
		"SMG", "mg",
		WeaponCategory_Secondary,

		1,                              // projectiles fired each shot
		25,                             // clip size
		1500,                           // reload time
		false,                          // staged reloading

		//timings (in msecs)
		WEAPONUP_TIME_NORMAL,           // weapon up time
		WEAPONDOWN_TIME,                // weapon down time
		75,                             // refire time
		HITSCAN_RANGE,                  // projectile timeout
		Vec2( 85.0f, 40.0f ),           // recoil
		Vec2( 50.0f, 10.0f ),           // recoilmin
		1350.0f,                        // recoil recovery
		FiringMode_Auto,

		0.0f,                           // zoom fov
		0.0f,                           // !zoom inaccuracy

		//damages
		9,                              // damage
		0,                              // selfdamage ratio
		15,                             // knockback
		0,                              // splash radius
		0,                              // splash minimum damage
		0,                              // splash minimum knockback

		//projectile def
		INSTANT,                        // speed
		0,                              // spread
	},

	{
		"Deagle", "deagle",
		WeaponCategory_Secondary,

		1,                              // projectiles fired each shot
		7,                              // clip size
		1500,                           // reload time
		false,                          // staged reloading

		//timings (in msecs)
		WEAPONUP_TIME_NORMAL,           // weapon up time
		WEAPONDOWN_TIME,                // weapon down time
		500,                            // refire time
		HITSCAN_RANGE,                  // projectile timeout
		Vec2( 425.0f, 100.0f ),         // recoil
		Vec2( 175.0f, 40.0f ),          // recoilmin
		3000.0f,                        // recoil recovery
		FiringMode_SemiAuto,

		0.0f,                           // zoom fov
		0.0f,                           // !zoom inaccuracy

		//damages
		25,                             // damage
		0,                              // selfdamage ratio
		30,                             // knockback
		0,                              // splash radius
		0,                              // splash minimum damage
		0,                              // splash minimum knockback

		//projectile def
		INSTANT,                        // speed
		0,                              // spread
	},

	{
		"Shotgun", "rg",
		WeaponCategory_Secondary,

		25,                             // projectiles fired each shot
		5,                              // clip size
		600,                            // reload time
		true,                           // staged reloading

		//timings (in msecs)
		WEAPONUP_TIME_SLOW,             // weapon up time
		WEAPONDOWN_TIME,                // weapon down time
		1400,                           // refire time
		HITSCAN_RANGE,                  // projectile timeout / projectile range for instant weapons
		Vec2( 325.0f, 120.0f ),         // recoil
		Vec2( 125.0f, 80.0f ),          // recoilmin
		1800.0f,                        // recoil recovery
		FiringMode_Auto,

		0.0f,                           // zoom fov
		0.0f,                           // !zoom inaccuracy

		//damages
		2,                              // damage
		0,                              // selfdamage ratio (rg cant selfdamage)
		6,                              // knockback
		0,                              // splash radius
		0,                              // splash minimum damage
		0,                              // splash minimum knockback

		//projectile def
		INSTANT,                        // speed
		50,                             // spread
	},

	{
		"Assault Rifle", "ar",
		WeaponCategory_Primary,

		1,                              // projectiles fired each shot
		35,                             // clip size
		1500,                           // reload time
		false,                          // staged reloading

		//timings (in msecs)
		WEAPONUP_TIME_NORMAL,           // weapon up time
		WEAPONDOWN_TIME,                // weapon down time
		160,                            // refire time
		HITSCAN_RANGE,                  // projectile timeout
		Vec2( 140.0f, 70.0f ),          // recoil
		Vec2( 50.0f, 30.0f ),           // recoilmin
		2250.0f,                        // recoil recovery
		FiringMode_Auto,

		0.0f,                           // zoom fov
		0.0f,                           // !zoom inaccuracy

		//damages
		10,                             // damage
		0,                              // selfdamage ratio
		10,                             // knockback
		0,                              // splash radius
		0,                              // splash minimum damage
		0,                              // splash minimum knockback

		//projectile def
		INSTANT,                        // speed
		0,                              // spread
	},

	{
		"Grenades", "gl",
		WeaponCategory_Secondary,

		1,                              // projectiles fired each shot
		5,                              // clip size
		1500,                           // reload time
		false,                          // staged reloading

		//timings (in msecs)
		WEAPONUP_TIME_NORMAL,           // weapon up time
		WEAPONDOWN_TIME,                // weapon down time
		500,                            // refire time
		1250,                           // projectile timeout
		Vec2( 325.0f, 60.0f ),          // recoil
		Vec2( 125.0f, 20.0f ),          // recoilmin
		2000.0f,                        // recoil recovery
		FiringMode_Auto,

		0.0f,                           // zoom fov
		0.0f,                           // !zoom inaccuracy

		//damages
		45,                             // damage
		1.0f,                           // selfdamage ratio
		100,                            // knockback
		120,                            // splash radius
		5,                              // splash minimum damage
		50,                             // splash minimum knockback

		//projectile def
		420,                            // speed
		0,                              // spread
	},

	{
		"Rockets", "rl",
		WeaponCategory_Primary,

		1,                              // projectiles fired each shot
		5,                              // clip size
		600,                            // reload time
		true,                           // staged reloading

		//timings (in msecs)
		WEAPONUP_TIME_SLOW,             // weapon up time
		WEAPONDOWN_TIME,                // weapon down time
		1000,                           // refire time
		10000,                          // projectile timeout
		Vec2( 350.0f, 100.0f ),         // recoil
		Vec2( 150.0f, 40.0f ),          // recoilmin
		2500.0f,                        // recoil recovery
		FiringMode_Auto,

		0.0f,                           // zoom fov
		0.0f,                           // !zoom inaccuracy

		//damages
		45,                             // damage
		1.0f,                           // selfdamage ratio
		100,                            // knockback
		120,                            // splash radius
		10,                             // splash minimum damage
		50,                             // splash minimum knockback

		//projectile def
		1400,                           // speed
		0,                              // spread
	},

	{
		"Plasma", "pg",
		WeaponCategory_Backup,

		1,                              // projectiles fired each shot
		30,                             // clip size
		1500,                           // reload time
		false,                          // staged reloading

		//timings (in msecs)
		WEAPONUP_TIME_NORMAL,           // weapon up time
		WEAPONDOWN_TIME,                // weapon down time
		50,                             // refire time
		10000,                          // projectile timeout
		Vec2( 60.0f, 50.0f ),           // recoil
		Vec2( 30.0f, 10.0f ),           // recoilmin
		1350.0f,                        // recoil recovery
		FiringMode_Auto,

		0.0f,                           // zoom fov
		0.0f,                           // !zoom inaccuracy

		//damages
		6,                              // damage
		0,                              // selfdamage ratio
		30,                             // knockback
		45,                             // splash radius
		6,                              // splash minimum damage
		5,                              // splash minimum knockback

		//projectile def
		3500,                           // speed
		0.0f,                           // spread
	},

	{
		"BubbleGun", "bg",
		WeaponCategory_Backup,

		1,                              // projectiles fired each shot
		14,                             // clip size
		1500,                           // reload time
		false,                          // staged reloading

		//timings (in msecs)
		WEAPONUP_TIME_NORMAL,           // weapon up time
		WEAPONDOWN_TIME,                // weapon down time
		150,                            // refire time
		10000,                          // projectile timeout
		Vec2( 80.0f, 60.0f ),           // recoil
		Vec2( 50.0f, 20.0f ),           // recoilmin
		1350.0f,                        // recoil recovery
		FiringMode_Auto,

		0.0f,                           // zoom fov
		0.0f,                           // !zoom inaccuracy

		//damages
		15,                             // damage
		0,                              // selfdamage ratio
		50,                             // knockback
		80,                             // splash radius
		1,                              // splash minimum damage
		6,                              // splash minimum knockback

		//projectile def
		600,                            // speed
		0,                              // spread
	},

	{
		"Laser", "lg",
		WeaponCategory_Primary,

		1,                              // projectiles fired each shot
		50,                             // clip size
		1000,                           // reload time
		false,                          // staged reloading

		//timings (in msecs)
		WEAPONUP_TIME_FAST,             // weapon up time
		WEAPONDOWN_TIME,                // weapon down time
		50,                             // refire time
		900,                            // projectile timeout / projectile range for instant weapons
		Vec2( 0.0f, 0.0f ),             // recoil
		Vec2( 0.0f, 0.0f ),             // recoilmin
		0.0f,                           // recoil recovery
		FiringMode_Smooth,

		0.0f,                           // zoom fov
		0.0f,                           // !zoom inaccuracy

		//damages
		5,                              // damage
		0,                              // selfdamage ratio
		14,                             // knockback
		0,                              // splash radius
		0,                              // splash minimum damage
		0,                              // splash minimum knockback

		//projectile def
		INSTANT,                        // speed
		0,                              // spread
	},

	{
		"Railgun", "eb",
		WeaponCategory_Secondary,

		1,                              // projectiles fired each shot
		5,                              // clip size
		600,                            // reload time
		true,                           // staged reloading

		//timings (in msecs)
		WEAPONUP_TIME_SLOW,             // weapon up time
		WEAPONDOWN_TIME,                // weapon down time
		600,                            // refire time
		HITSCAN_RANGE,                  // range
		Vec2( 120.0f, 40.0f ),          // recoil
		Vec2( 60.0f, 0.0f ),            // recoilmin
		1000.0f,                        // recoil recovery
		FiringMode_SemiAuto,

		0.0f,                           // zoom fov
		0.0f,                           // !zoom inaccuracy

		//damages
		25,                             // damage
		0,                              // selfdamage ratio
		60,                             // knockback
		100,                            // splash radius
		0,                              // minimum damage
		60,                             // minimum knockback

		//projectile def
		INSTANT,                        // speed
		0,                              // spread
	},

	{
		"Sniper", "sniper",
		WeaponCategory_Primary,

		1,                              // projectiles fired each shot
		1,                              // clip size
		2000,                           // reload time
		false,                          // staged reloading

		//timings (in msecs)
		WEAPONUP_TIME_VERY_SLOW,        // weapon up time
		WEAPONDOWN_TIME,                // weapon down time
		500,                            // refire time
		HITSCAN_RANGE,                  // range
		Vec2( 500.0f, 150.0f ),         // recoil
		Vec2( 200.0f, 50.0f ),          // recoilmin
		2500.0f,                        // recoil recovery
		FiringMode_Auto,

		25.0f,                          // zoom fov
		30.0f,                          // !zoom inaccuracy

		//damages
		50,                             // damage
		0,                              // selfdamage ratio
		100,                            // knockback
		0,                              // splash radius
		0,                              // minimum damage
		0,                              // minimum knockback

		//projectile def
		INSTANT,                        // speed
		0,                              // spread
	},

	{
		"Rifle", "rifle",
		WeaponCategory_Secondary,

		1,                              // projectiles fired each shot
		5,                              // clip size
		2000,                           // reload time
		false,                          // staged reloading

		//timings (in msecs)
		WEAPONUP_TIME_NORMAL,           // weapon up time
		WEAPONDOWN_TIME,                // weapon down time
		600,                            // refire time
		10000,                          // range
		Vec2( 275.0f, 60.0f ),          // recoil
		Vec2( 125.0f, 20.0f ),          // recoilmin
		2000.0f,                        // recoil recovery
		FiringMode_SemiAuto,

		0.0f,                           // zoom fov
		0.0f,                           // !zoom inaccuracy

		//damages
		38,                             // damage
		0,                              // selfdamage ratio
		50,                             // knockback
		0,                              // splash radius
		0,                              // minimum damage
		0,                              // minimum knockback

		//projectile def
		5500,                           // speed
		0,                              // spread
	},
};

STATIC_ASSERT( ARRAY_COUNT( gs_weaponDefs ) == Weapon_Count );

const WeaponDef * GS_GetWeaponDef( WeaponType weapon ) {
	assert( weapon < Weapon_Count );
	return &gs_weaponDefs[ weapon ];
}
