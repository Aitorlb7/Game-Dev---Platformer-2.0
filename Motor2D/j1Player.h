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
	STOP_JUMPING,
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
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

	void j1Player::Player_Colliding(Collider* c1, Collider* c2);
	void CheckAnimation(); // It will check and if needed change any animation
	void jumpMovement();
	void Dash_Movement();
	bool PositionCameraOnPlayer();
	/*iPoint dashDirection(player_state);*/ //Check the player direction to dash forwards


public:
	player_state		pState;

	SDL_Texture*		graphics = nullptr;
	p2SString			spritesheet;
	
	bool				is_dead;
	bool				is_dashing;
	bool				is_jumping;
	bool				is_grounded;
	bool				able_superjump = false;
	bool				flip = false;


	int jumpInit_pos;

	p2Point<float>		before_colliding;
	p2Point<float>		position;

	p2Point<float>		player_velocity;
	p2Point<float>		run_speed;
	p2Point<float>		max_speed;

	
	p2Point<float>		dash_speed;

	float				gravity;
	float				jump_force;

	Collider*			player_col = nullptr;

	Animation*			current_anim;
	Animation			idle_anim;
	Animation			run_anim;
	Animation			jump_anim;
	Animation			attack_anim;
	Animation			crouch_anim;
	Animation			crouchwalk_anim;
	Animation			dash_anim;
	Animation			fall_anim;

	p2SString			jump_SFX;
	p2SString			run_SFX;
	p2SString			dash_SFX;


};
#endif // __j1PLAYER_H__