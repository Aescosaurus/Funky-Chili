/****************************************************************************************** 
 *	Chili DirectX Framework Version 16.07.20											  *	
 *	Game.h																				  *
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
#pragma once

#include "Keyboard.h"
#include "Mouse.h"
#include "Graphics.h"
#include "Funcs.h"
#include "Anim.h"
#include "FrameTimer.h"

class Game
{
public:
	Game( class MainWindow& wnd );
	Game( const Game& ) = delete;
	Game& operator=( const Game& ) = delete;
	void Go();
private:
	void ComposeFrame();
	void UpdateModel();
	/********************************/
	/*  User Functions              */
	/********************************/
private:
	MainWindow& wnd;
	Graphics gfx;
	/********************************/
	/*  User Variables              */
	bool calledLastFrame = false;

	const Surface sprSheet = Surface( "Images/Walk.bmp" )
		.GetInterpolated( 180 * 2,14 * 2 );
	Anim legs = Anim( 0,0,18 * 2,14 * 2,10,sprSheet,20.0f );
	// Vei2 drawPos = { int( plw / 2.0f ) -
	// 	legs.GetWidth() / 2,int( plh / 4.0f ) };

	float plw = 32.0f;
	float plh = 24.0f;
	Vec player = Vec( float( Graphics::ScreenWidth ) /
		2.0f - plw / 2.0f,float( Graphics::ScreenHeight ) /
		2.0f - plh / 2.0f );
	Rec hitbox = GetExpanded( Rec( player.x,player.y,
		plw,plw ),-( plw ) );
	Vec hitboxAdd = Vec( plw / 2.0f - hitbox.w / 2.0f,
		plh * 1.2f );
	Vec vel = Vec( 0.0f,0.0f );
	Vec dir = Vec( 1.0f,1.0f );
	const float slowdown = 0.85f;
	const float maxSpeed = 5.2f;
	const Vec minClampArea = Vec( 128.0f,128.0f );
	const Vec maxClampArea = Vec( float( Graphics
		::ScreenWidth ) - minClampArea.x,
		float( Graphics::ScreenHeight ) - minClampArea.y );

	// const bullets = [];
	std::vector<Bullet> bullets;
	const float bulletSize = 10.0f;
	const float bulletSpeed = 12.2f;
	const float refireTime = 0.2f * 30.0f;
	float shotTimer = 0.0f;

	// let walls = [];
	std::vector<Rec> walls;
	const float wallSize = 32.0f;
	/********************************/
};