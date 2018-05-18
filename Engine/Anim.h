#pragma once

#include "Surface.h"
#include "Graphics.h"
#include <vector>

class Anim
{
public:
	Anim( int x,int y,int width,int height,int count,
		const Surface& sheet,float holdTime,Color chroma = Colors::Magenta );
	Anim( const Anim& other );
	Anim& operator=( const Anim& other );

	void Update( float dt );
	void UpdateUntilDone( float dt );
	void Draw( const Vei2& pos,Graphics& gfx,bool reversed = false ) const;
	void Draw( const Vei2& pos,Graphics& gfx,const RectI& clip,bool reversed = false ) const;
	void Finish();
	void UnFinish();
	void SetFrame( int nFrame );

	bool IsFinished() const;
	int GetWidth() const;
	int GetHeight() const;
private:
	void Advance();
private:
	const int width;
	const int height;
	const Color chroma;
	const Surface& sprite;
	std::vector<RectI> frames;
	int iCurFrame = 0;
	const float holdTime;
	float curFrameTime = 0.0f;
	bool finished = false;
};