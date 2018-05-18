#pragma once

#include "Keyboard.h"
#include "Graphics.h"
#include <vector>
#include <string>
#include "Random.h"

struct Vec
{
	Vec( float x,float y );
	float x;
	float y;
};

struct Bullet
	:
	public Vec
{
	Bullet( float x,float y,const Vec& dir );
	Vec d; // dir
};

struct Rec
	:
	public Vec
{
	Rec( float x,float y,float w,float h );
	float w;
	float h;
};

bool IsDown( const Keyboard& kbd,int keycode );
float AmountDown( const Keyboard& kbd,int keycode );
float KeyDelta( const Keyboard& kbd,int kc1,int kc2 );
Vec AddVel( const Vec& vel,float xAmount,float yAmount );
Vec Add( const Vec& v1,const Vec& v2 );
Vec Subtract( const Vec& v1,const Vec& v2 );
Vec Multiply( const Vec& v1,float amount );
Vec Divide( const Vec& v1,float amount );
float Min( float val1,float val2 );
float Max( float val1,float val2 );
Vec Clamp( const Vec& v,float min,float max );
Vec Clamp2D( const Vec& v,float minX,float minY,
	float maxX,float maxY );
Vec Normalize( const Vec& v );
Vec GetCenter( float x,float y,float width,float height );
bool IsContainedBy( const Rec& r1,const Rec& r2 );
Rec GetScreenArea();
Rec GetExpanded( const Rec& r,float amount );
Vec GetRectCenter( const Rec& r );
std::vector<Rec> LoadIntoArray( const std
	::string& source,float rectSize,
	const Vec& startPos );
std::vector<Rec> CreateRandomMap( float rectSize,
	const Vec& startPos );
template<typename T>
void MoveAll( std::vector<T>& arr,const Vec& amount )
{
	for( T& x : arr )
	{
		x.x += amount.x;
		x.y += amount.y;
	}
}
float GetLenSq( const Vec& v );
float GetLen( const Vec& v );
bool IsOverlap( const Rec& r1,const Rec& r2 );
bool CheckOverlapForArr( const Rec& r1,const std
	::vector<Rec>& rectArr );

