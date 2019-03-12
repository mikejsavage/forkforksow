#pragma once

// ints and size_t
#include <stdint.h>
#include <stddef.h>

typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

// allocator interface
struct Allocator {
	virtual ~Allocator() { }
	virtual void * try_allocate( size_t size, size_t alignment, const char * func, const char * file, int line ) = 0;
	virtual void * try_reallocate( void * ptr, size_t current_size, size_t new_size, size_t alignment, const char * func, const char * file, int line ) = 0;
	void * allocate( size_t size, size_t alignment, const char * func, const char * file, int line );
	void * reallocate( void * ptr, size_t current_size, size_t new_size, size_t alignment, const char * func, const char * file, int line );
	virtual void deallocate( void * ptr, const char * func, const char * file, int line ) = 0;
};

// span
template< typename T >
struct Span {
	constexpr Span() : ptr( NULL ), n( 0 ) { }
	constexpr Span( T * ptr, size_t n ) : ptr( ptr ), n( n ) { }

	// TODO: member functions

	T * ptr;
	size_t n;
};

// linear algebra
struct Vec2 {
	float x, y;

	Vec2() { }
	explicit constexpr Vec2( float xy ) : x( xy ), y( xy ) { }
	constexpr Vec2( float x_, float y_ ) : x( x_ ), y( y_ ) { }
};

struct Vec3 {
	float x, y, z;

	Vec3() { }
	explicit constexpr Vec3( float xyz ) : x( xyz ), y( xyz ), z( xyz ) { }
	constexpr Vec3( Vec2 xy, float z_ ) : x( xy.x ), y( xy.y ), z( z_ ) { }
	constexpr Vec3( float x_, float y_, float z_ ) : x( x_ ), y( y_ ), z( z_ ) { }

	Vec2 xy() const { return Vec2( x, y ); }
};

struct Vec4 {
	float x, y, z, w;

	Vec4() { }
	explicit constexpr Vec4( float xyzw ) : x( xyzw ), y( xyzw ), z( xyzw ), w( xyzw ) { }
	constexpr Vec4( Vec2 xy, float z_, float w_ ) : x( xy.x ), y( xy.y ), z( z_ ), w( w_ ) { }
	constexpr Vec4( Vec3 xyz, float w_ ) : x( xyz.x ), y( xyz.y ), z( xyz.z ), w( w_ ) { }
	constexpr Vec4( float x_, float y_, float z_, float w_ ) : x( x_ ), y( y_ ), z( z_ ), w( w_ ) { }

	Vec2 xy() const { return Vec2( x, y ); }
	Vec3 xyz() const { return Vec3( x, y, z ); }
};

struct Mat2 {
	Vec2 col0, col1;

	Mat2() { }
	constexpr Mat2( Vec2 c0, Vec2 c1 ) : col0( c0 ), col1( c1 ) { }
	constexpr Mat2( float e00, float e01, float e10, float e11 ) : col0( e00, e10 ), col1( e01, e11 ) { }

	Vec2 row0() const { return Vec2( col0.x, col1.x ); }
	Vec2 row1() const { return Vec2( col0.y, col1.y ); }

	static constexpr Mat2 I() { return Mat2( 1, 0, 0, 1 ); }
};

struct Mat3 {
	Vec3 col0, col1, col2;

	Mat3() { }
	constexpr Mat3( Vec3 c0, Vec3 c1, Vec3 c2 ) : col0( c0 ), col1( c1 ), col2( c2 ) { }
	constexpr Mat3(
		float e00, float e01, float e02,
		float e10, float e11, float e12,
		float e20, float e21, float e22
	) : col0( e00, e10, e20 ), col1( e01, e11, e21 ), col2( e02, e12, e22 ) { }

	Vec3 row0() const { return Vec3( col0.x, col1.x, col2.x ); }
	Vec3 row1() const { return Vec3( col0.y, col1.y, col2.y ); }
	Vec3 row2() const { return Vec3( col0.z, col1.z, col2.z ); }

	static constexpr Mat3 I() {
		return Mat3(
			1, 0, 0,
			0, 1, 0,
			0, 0, 1
		);
	}
};

struct alignas( 16 ) Mat4 {
	Vec4 col0, col1, col2, col3;

	Mat4() { }
	constexpr Mat4( Vec4 c0, Vec4 c1, Vec4 c2, Vec4 c3 ) : col0( c0 ), col1( c1 ), col2( c2 ), col3( c3 ) { }
	constexpr Mat4(
		float e00, float e01, float e02, float e03,
		float e10, float e11, float e12, float e13,
		float e20, float e21, float e22, float e23,
		float e30, float e31, float e32, float e33
	) : col0( e00, e10, e20, e30 ), col1( e01, e11, e21, e31 ), col2( e02, e12, e22, e32 ), col3( e03, e13, e23, e33 ) { }

	Vec4 row0() const { return Vec4( col0.x, col1.x, col2.x, col3.x ); }
	Vec4 row1() const { return Vec4( col0.y, col1.y, col2.y, col3.y ); }
	Vec4 row2() const { return Vec4( col0.z, col1.z, col2.z, col3.z ); }
	Vec4 row3() const { return Vec4( col0.w, col1.w, col2.w, col3.w ); }

	static constexpr Mat4 I() {
		return Mat4(
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		);
	}
};

// colors
// struct RGB8 {
//         u8 r, g, b;
//
// 	RGB8() { }
//         constexpr RGB8( u8 r_, u8 g_, u8 b_ ) : r( r_ ), g( g_ ), b( b_ ) { }
// };
//
// struct RGBA8 {
//         u8 r, g, b, a;
//
// 	RGBA8() { }
//         constexpr RGB8( u8 r_, u8 g_, u8 b_, u8 a_ ) : r( r_ ), g( g_ ), b( b_ ), a( a_ ) { }
// };

// TODO: asset types?