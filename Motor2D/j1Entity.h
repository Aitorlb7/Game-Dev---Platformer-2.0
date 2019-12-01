#ifndef __J1ENTITY__
#define __J1ENTITY__

#include "j1Module.h"
#include "j1Animation.h"
#include "p2Point.h"
#include "p2DynArray.h"
#include "j1Timer.h"

#define DRAW_LIMIT 200

struct SDL_Texture;

enum entity_state
{
	IDLE = 0,
	RIGHT,
	LEFT,
	CROUCHING,
	DASHING,
	JUMPING,
	STOP_JUMPING,
	FALLING,
	ATTACKING,
};

enum entity_type
{
	PLAYER = 0,
	ALIEN,
	CHICKEN,
};

class Entity : public j1Module
{
public:

	Entity(const char* name);
	virtual ~Entity();

	bool Entity_Update(float dt);

	void Entity_CollisionManager(Collider* C1, Collider* C2);
	void Entity_OnCollision(Collider* c1, Collider* c2) {}
	virtual void Move_entity(p2DynArray<iPoint>& path, float dt) {};

public:

	p2Point<float>		position;
	p2Point<float>		initial_position;
	p2Point<float>		velocity;

	entity_state		state;

	p2DynArray<iPoint>	path;
	iPoint				currentPathtile = { 0,0 };
	int					pathIndex;
	
	bool				dead = false;
	bool				is_grounded = false;
	bool				flip = false;
	bool				canFly = false;
	bool				chase = false;

	SDL_Texture*		graphics = nullptr;
	p2SString			spritesheet;

	//Path logic
	p2DynArray<iPoint>	entityPath;
	bool				god_mode = false;

	//Common animations	
	Animation*			current_anim;
	Animation			idle_anim;
	Animation			run_anim;
	Animation			jump_anim;
	Animation*			death = nullptr;

	Collider*			collider = nullptr;

	float				gravity;
	float				jump_force;

	entity_type			type;
	
};

#endif // __J1ENTITY__
