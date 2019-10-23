#pragma once
#ifndef __j1PLAYER_H__
#define __j1PLAYER_H__

#include "j1Module.h"
#include "j1Animation.h"
#include "j1App.h"
#include "p2Point.h"

struct SDL_Texture;
struct Collider;

enum player_state
{
	IDLE = 0,
	RIGHT,
	LEFT,
	CROUCHING,
	DASHING,
	JUMPING,
	FALLING,
};

class j1Player : public j1Module
{

public:

	j1Player();
	virtual ~j1Player();
	bool Awake(pugi::xml_node&);
	bool Start();
	bool PreUpdate();
	bool Update();
	bool PostUpdate();
	//bool CleanUp();

	
	
	void CheckAnimation(); // It will check and if needed change any animation
	bool PositionCameraOnPlayer();
	/*iPoint dashDirection(player_state);*/ //Check the player direction to dash forwards
	void Player_Move();

	Animation*			current_anim;
	Animation			idle_anim;
	Animation			run_anim;
	Animation			jump_anim;
	Animation			attack_anim;
	Animation			crouch_anim;
	Animation			crouchwalk_anim;
	Animation			dash_anim;
	Animation			fall_anim;

public:
	player_state		pState;
	SDL_Texture*		graphics = nullptr;

	bool				dead;
	bool				is_dashing;
	bool				is_jumping;
	bool				is_grounded;

	p2Point<float>		position;
	p2Point<float>		player_velocity;
	p2Point<float>		run_speed;

	p2Point<float>		jump_speed;
	p2Point<float>		dash_speed;

	float				gravity;
	float				animation_speed;

	Collider*			player_col = nullptr;
	SDL_Rect			visual_col;


};
#endif // __j1PLAYER_H__