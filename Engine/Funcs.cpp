#include "Funcs.h"

Vec::Vec( float x,float y )
	:
	x( x ),
	y( y )
{
}

Bullet::Bullet( float x,float y,const Vec& dir )
	:
	Vec( x,y ),
	d( dir )
{
}

Rec::Rec( float x,float y,float w,float h )
	:
	Vec( x,y ),
	w( w ),
	h( h )
{
}

bool IsDown( const Keyboard& kbd,int keycode )
{
	return kbd.KeyIsPressed( keycode );
}

float AmountDown( const Keyboard& kbd,int keycode )
{
	return ( IsDown( kbd,keycode ) ? 1.0f : 0.0f );
}

float KeyDelta( const Keyboard& kbd,int kc1,int kc2 )
{
	return( -( AmountDown( kbd,kc1 ) -
		AmountDown( kbd,kc2 ) ) );
}

Vec AddVel( const Vec& vel,float xAmount,float yAmount )
{
	return Vec( vel.x + xAmount,vel.y + yAmount );
}

Vec Add( const Vec& v1,const Vec& v2 )
{
	return Vec( v1.x + v2.x,v1.y + v2.y );
}

Vec Subtract( const Vec& v1,const Vec& v2 )
{
	return Vec( v1.x - v2.x,v1.y - v2.y );
}

Vec Multiply( const Vec& v1,float amount )
{
	return Vec( v1.x * amount,v1.y * amount );
}

Vec Divide( const Vec& v1,float amount )
{
	return Vec( v1.x / amount,v1.y / amount );
}

float Min( float val1,float val2 )
{
	return( val1 < val2 ? val1 : val2 );
}

float Max( float val1,float val2 )
{
	return( val1 > val2 ? val1 : val2 );
}

Vec Clamp( const Vec& v,float min,float max )
{
	return Vec( Min( Max( min,v.x ),max ),
		Min( Max( min,v.y ),max ) );
}

Vec Clamp2D( const Vec& v,float minX,float minY,
	float maxX,float maxY )
{
	return Vec( Min( Max( minX,v.x ),maxX ),
		Min( Max( minY,v.y ),maxY ) );
}

Vec Normalize( const Vec& v )
{
	const float len = sqrt( v.x * v.x + v.y * v.y );

	return ( len != 0.0f
		? Multiply( v,1.0f / len )
		: Vec( 0.0f,0.0f ) );
}

Vec GetCenter( float x,float y,float width,float height )
{
	return Vec( x + width / 2.0f,y + height / 2.0f );
}

bool IsContainedBy( const Rec& r1,const Rec& r2 )
{
	return ( r1.x > r2.x && r1.y > r2.y &&
		r1.x + r1.w < r2.x + r2.w &&
		r1.y + r1.h < r2.y + r2.h );
}

Rec GetScreenArea()
{
	return Rec( 0,0,float( Graphics::ScreenWidth ),
		float( Graphics::ScreenHeight ) );
}

Rec GetExpanded( const Rec& r,float amount )
{
	return Rec( r.x - amount / 2.0f,r.y - amount / 2.0f,
		r.w + amount / 2.0f,r.h + amount / 2.0f );
}

Vec GetRectCenter( const Rec& r )
{
	return Vec( ( r.x + r.x + r.w ) / 2,
		( r.y + r.y + r.h ) / 2 );
}

std::vector<Rec> LoadIntoArray( const std
	::string& source,float rectSize,
	const Vec& startPos )
{
	// const dest = [];
	std::vector<Rec> dest;
	
	float x = startPos.x;
	float y = startPos.y;

	// for( var i in source )
	for( const char& s : source )
	{
		x += rectSize;
		if( s == '1' )
		{
			// dest.push( Rect( x,y,rectSize,rectSize ) );
			dest.emplace_back( Rec( x,y,
				rectSize,rectSize ) );
		}
		else if( s == 'n' )
		{ // New "line".
			y += rectSize;
			x = startPos.x;
		}
	}

	return dest;
}

std::vector<Rec> CreateRandomMap( float rectSize,
	const Vec& startPos )
{
	std::vector<Rec> dest;

	const float maxX = float( Graphics::ScreenWidth ) +
		float( Graphics::ScreenWidth ) * 0.75f;
	const float maxY = float( Graphics::ScreenHeight ) +
		float( Graphics::ScreenHeight ) * 0.75f;

	// for( var y = startPos.y; y < maxY; y += rectSize )
	for( float y = startPos.y; y < maxY; y += rectSize )
	{
		// for( var x = startPos.x; x < maxX; x += rectSize )
		for( float x = startPos.x; x < maxX; x += rectSize )
		{
			if( x == startPos.x || y == startPos.y ||
				x == maxX - rectSize ||
				y == maxY - rectSize )
			{
				// dest.push( Rect( x,y,rectSize,rectSize ) );
				dest.emplace_back( Rec( x,y,
					rectSize,rectSize ) );
			}

			// if( Math.random() < 0.1 )
			if( Random::RangeF( 0.0f,10.0f ) < 1.0f )
			{
				// dest.push( Rect( x,y,rectSize,rectSize ) );
				dest.emplace_back( Rec( x,y,
					rectSize,rectSize ) );
			}
		}
	}

	return dest;
}

float GetLenSq( const Vec& v )
{
	return( v.x * v.x + v.y * v.y );
}

float GetLen( const Vec& v )
{
	return( sqrt( GetLenSq( v ) ) );
}

bool IsOverlap( const Rec& r1,const Rec& r2 )
{
	return( r1.x + r1.w > r2.x &&
		r1.x < r2.x + r2.w &&
		r1.y + r1.h > r2.y &&
		r1.y < r2.y + r2.h );
}

bool CheckOverlapForArr( const Rec& r1,
	const std::vector<Rec>& rectArr )
{
	// for( var i in rectArr )
	for( const auto& ri : rectArr )
	{
		if( IsOverlap( r1,ri ) ) return true;
	}

	return false;
}
