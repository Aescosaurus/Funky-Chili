#include "Anim.h"
#include "SpriteEffect.h"

Anim::Anim( int x,int y,int width,int height,int count,
	const Surface& sheet,float holdTime,Color chroma )
	:
	sprite( sheet ),
	holdTime( holdTime ),
	chroma( chroma ),
	width( width ),
	height( height )
{
	for( int i = 0; i < count; ++i )
	{
		frames.emplace_back( x + i * width,
			x + ( i + 1 ) * width,y,y + height );
	}
}

Anim::Anim( const Anim& other )
	:
	chroma( other.chroma ),
	sprite( other.sprite ),
	holdTime( other.holdTime ),
	width( other.width ),
	height( other.height )
{
	*this = other;
}

Anim& Anim::operator=( const Anim& other )
{
	iCurFrame = other.iCurFrame;
	curFrameTime = other.curFrameTime;
	frames = other.frames;
	return( *this );
}

void Anim::Update( float dt )
{
	finished = false;
	curFrameTime += dt;
	while( curFrameTime >= holdTime )
	{
		Advance();
		curFrameTime -= holdTime;
	}
}

void Anim::UpdateUntilDone( float dt )
{
	finished = false;
	curFrameTime += dt;
	while( iCurFrame > 0 && curFrameTime >= holdTime )
	{
		Advance();
		curFrameTime -= holdTime;
	}
}

void Anim::Draw( const Vei2& pos,Graphics& gfx,bool reversed ) const
{
	Draw( pos,gfx,Graphics::GetScreenRect(),reversed );
}

void Anim::Draw( const Vei2& pos,Graphics& gfx,const RectI& clip,bool reversed ) const
{
	gfx.DrawSprite( int( pos.x ),int( pos.y ),
		frames[iCurFrame],clip,sprite,
		SpriteEffect::Chroma{ chroma },reversed );
}

void Anim::Finish()
{
	finished = true;
}

void Anim::UnFinish()
{
	finished = false;
}

void Anim::SetFrame( int nFrame )
{
	iCurFrame = nFrame;
}

bool Anim::IsFinished() const
{
	return finished;
}

int Anim::GetWidth() const
{
	return width;
}

int Anim::GetHeight() const
{
	return height;
}

void Anim::Advance()
{
	if( ++iCurFrame >= int( frames.size() ) )
	{
		iCurFrame = 0;
		finished = true;
	}
}
