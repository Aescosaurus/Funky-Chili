/******************************************************************************************
 *	Chili DirectX Framework Version 16.07.20											  *
 *	Game.cpp																			  *
 *	Copyright 2016 PlanetChili.net <http://www.planetchili.net>							  *
 *																						  *
 *	This file is part of The Chili DirectX Framework.									  *
 *																						  *
 *	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
 *	it under the terms of the GNU General Public License as published by				  *
 *	the Free Software Foundation, either version 3 of the License, or					  *
 *	(at your option) any later version.													  *
 *																						  *
 *	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
 *	GNU General Public License for more details.										  *
 *																						  *
 *	You should have received a copy of the GNU General Public License					  *
 *	along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
 ******************************************************************************************/
#include "MainWindow.h"
#include "Game.h"

Game::Game( MainWindow& wnd )
	:
	wnd( wnd ),
	gfx( wnd )
{
	// walls = LoadIntoArray( maps._1,wallSize,
	// 	Vec2( -ScreenWidth * 0.75,-ScreenHeight * 0.75 ) );
	walls = CreateRandomMap( wallSize,Vec( -float( Graphics
		::ScreenWidth ) * 0.75f,-float( Graphics
			::ScreenHeight ) * 0.75f ) );
}

void Game::Go()
{
	gfx.BeginFrame();
	UpdateModel();
	ComposeFrame();
	gfx.EndFrame();
}

void Game::UpdateModel()
{
	const float dt = FrameTimer::Mark();

	if( calledLastFrame )
	{
		calledLastFrame = false;
		return;
	}
	calledLastFrame = true;

	const Vec move = Normalize( Vec( KeyDelta(
		wnd.kbd,'A','D' ),KeyDelta( wnd.kbd,'W','S' ) ) );
	const Vec look = Normalize( Vec( KeyDelta(
		wnd.kbd,VK_LEFT,VK_RIGHT ),
		KeyDelta( wnd.kbd,VK_UP,VK_DOWN ) ) );
	const Vec aim = Normalize( Add( look,
		Divide( vel,maxSpeed ) ) );

	dir = ( aim.x != 0.0f || aim.y != 0.0f ) ? aim : dir;

	vel = AddVel( vel,move.x,move.y );
	vel = Clamp( vel,-maxSpeed,maxSpeed );

	Vec moveAmount = Multiply( vel,-1.0f );
	MoveAll( walls,Multiply( vel,-1.0f ) );

	player = Add( player,vel );

	hitbox.x = player.x + hitboxAdd.x;
	hitbox.y = player.y + hitboxAdd.y;

	// while( CheckOverlapForArr( hitbox,walls ) &&
	// 	GetLen( vel ) > 0.01 )
	while( CheckOverlapForArr( hitbox,walls ) &&
		GetLen( vel ) > 0.01f )
	{
		// 	player = Subtract( player,Multiply( vel,0.1 ) );
		player = Subtract( player,Multiply( vel,0.1f ) );
		hitbox.x = player.x + hitboxAdd.x;
		hitbox.y = player.y + hitboxAdd.y;
	}

	vel = Multiply( vel,slowdown );

	const Vec offsetAmount = Divide( Subtract(
		GetRectCenter( GetScreenArea() ),GetCenter(
			player.x,player.y,plw,plh ) ),20.0f );

	moveAmount = Add( moveAmount,offsetAmount );
	MoveAll( walls,offsetAmount );
	player = Add( player,offsetAmount );

	player = Clamp2D( GetCenter( player.x,player.y,
		plw,plh ),
		minClampArea.x,minClampArea.y,
		maxClampArea.x,maxClampArea.y );
	// player.x -= plw / 2;
	// player.y -= plh / 2;
	player.x -= plw / 2.0f;
	player.y -= plh / 2.0f;

	++shotTimer;
	const Vec shotPos = GetCenter( player.x,player.y,
		plw,plh );

	// if( ( look.x != 0.0 || look.y != 0.0 ) &&
	// 	shotTimer > refireTime )
	if( ( look.x != 0.0f || look.y != 0.0f ) &&
		shotTimer > refireTime )
	{
		// 	bullets.push( Bullet( shotPos.x - bulletSize / 2,
		// 		shotPos.y - bulletSize / 2,dir ) );
		bullets.emplace_back( Bullet( shotPos.x -
			bulletSize / 2.0f,shotPos.y -
			bulletSize / 2.0f,dir ) );

		shotTimer = 0.0;
	}
	// 
	// for( var i in bullets )
	for( auto& it = bullets.begin();
		it < bullets.end(); ++it )
	{
		const Vec temp = AddVel( *it,
			it->d.x * bulletSpeed,
			it->d.y * bulletSpeed );

		// *it = Bullet( temp.x,temp.y,it->d );
		it->x = temp.x;
		it->y = temp.y;

		const Vec temp2 = Add( temp,moveAmount );
		// *it = Bullet( temp2.x,temp2.y,it->d );
		it->x = temp2.x;
		it->y = temp2.y;

		if( !IsContainedBy( Rec( it->x,it->y,
			bulletSize,bulletSize ),
			GetScreenArea() ) )
		{
			// bullets.splice( i,1 );
			it = bullets.erase( it );
			if( it >= bullets.end() ) break;
		}
	}

	legs.Update( GetLenSq( vel ) );
	if( GetLenSq( vel ) < 0.1f ) legs.SetFrame( 0 );
}

void Game::ComposeFrame()
{
	// for( var i in walls )
	for( const auto& w : walls )
	{
		if( Rect( { w.x,w.y },w.w,w.h )
			.IsContainedBy( Graphics::GetScreenRect()
				.GetExpanded( -5 ) ) )
		{
			gfx.DrawRect( int( w.x ),int( w.y ),
				int( w.w ),int( w.h ),Colors::Gray );
			gfx.DrawHitbox( Rect( { w.x,w.y },w.w,w.h ) );
		}
	}

	if( Rect( { player.x,player.y },plw,plh )
		.IsContainedBy( Graphics::GetScreenRect()
			.GetExpanded( -5 ) ) )
	{
		legs.Draw( Vei2( int( hitbox.x + hitbox.w / 2.0f ) -
			legs.GetWidth() / 2,int( hitbox.y -
			hitbox.h / 1.2f ) ),gfx,vel.x < 0.0f );

		gfx.DrawRect( int( player.x ),int( player.y ),
			int( plw ),int( plh ),Colors::Pink );

		gfx.DrawHitbox( Rect( { hitbox.x,hitbox.y },
			hitbox.w,hitbox.h ),Colors::Cyan );
	}

	// for( var i in bullets )
	for( const auto& b : bullets )
	{
		gfx.DrawRect( int( b.x ),int( b.y ),
			int( bulletSize ),int( bulletSize ),
			Colors::Red );
	}
}
