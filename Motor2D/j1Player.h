#pragma once
#ifndef __j1PLAYER_H__
#define __j1PLAYER_H__

#include "j1Module.h"
#include "j1Animation.h"
#include "j1App.h"
#include "p2Point.h"
#include "j1Entity.h"

#define CAMERA_LIMIT 2400

struct SDL_Texture;
struct Collider;

class j1Player : public Entity
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
	bool Save(pugi::xml_node&) const;
	bool Load(pugi::xml_node&);

	void j1Player::Entity_OnCollision(Collider* c1, Collider* c2);
	void jumpMovement(float dt);
	void Load_Level();
	void Attack();
	void Dash_Movement();
	bool PositionCameraOnPlayer();
	void God_Mode();


public:

	bool				is_dashing;
	bool				is_jumping;
	bool				is_attacking;
	bool				able_superjump = false;

	int					jumpInit_pos;

	p2Point<float>		run_speed;
	p2Point<float>		max_speed;	
	p2Point<float>		dash_speed;

	j1Timer				Alive_Time;
	float				startime = 0;
	float				Timer;
	float				Attack_Timer;
	float				dash_acceleration;
	

	Animation			crouch_anim;
	Animation			crouchwalk_anim;
	Animation			dash_anim;
	Animation			fall_anim;
	Animation			god_anim;
	Animation			attack_anim;

	p2SString			jump_SFX;
	p2SString			run_SFX;
	p2SString			dash_SFX;

	Collider*			attack_col;


};
#endif // __j1PLAYER_H__