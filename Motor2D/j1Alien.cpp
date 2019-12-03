#include "j1App.h"
#include "j1Player.h"
#include "j1Textures.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Collisions.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1Scene.h"
#include "j1Audio.h"
#include "j1PathFinding.h"
#include "p2Log.h"
#include "j1Alien.h"
#include "j1Entity.h"
#include "j1EntityManager.h"

Alien::Alien() : Entity("Alien")
{
	name.create("Alien");

	graphics = App->tex->Load("textures/Alien_Spritesheet.png");
	collider = App->collisions->AddCollider({ (int)position.x, (int)position.y, 15, 15 }, COLLIDER_ENEMY, this);

	fly_anim.PushBack({ 0, 40, 18, 15 });
	fly_anim.PushBack({ 20, 40, 18, 15 });
	fly_anim.PushBack({ 40, 40, 18, 15 });
	fly_anim.PushBack({ 60, 40, 18, 15 });
	fly_anim.speed = 0.2f;
	state = IDLE;
	current_anim = &fly_anim;
}

Alien::~Alien()
{

}

bool Alien::Awake(pugi::xml_node&)
{
	return true;
}

bool Alien::Update(float dt)
{
	
	if ((player_goal.x - position.x) < RADAR_RANGE
		&& (player_goal.x - position.x) > -RADAR_RANGE)
	{


	}
		
	
	
	position.x += velocity.x * App->dt;
	position.y += velocity.y * App->dt;
	Move_entity(state,dt);
	collider->SetPos(position.x, position.y);
	return true;
}

bool Alien::PostUpdate()
{

	return true;
}
void Alien::Move_entity(entity_state state, float dt)
{
	switch (state)
	{
	case entity_state::UP:

		position.y -= velocity.y * dt;
		App->audio->PlayFx(13, 0);

		break;

	case entity_state::DOWN:

		position.y += velocity.y * dt;
		break;

	case entity_state::RIGHT:

		position.x += velocity.x * dt;
		break;

	case entity_state::LEFT:

		position.x -= velocity.x * dt;
		break;

	case entity_state::UP_RIGHT:

		position.x += velocity.x * dt;
		position.y -= velocity.y * dt;
		break;

	case entity_state::UP_LEFT:

		position.x -= velocity.x * dt;
		position.y -= velocity.y * dt;

		break;

	case entity_state::DOWN_RIGHT:

		position.x += velocity.x * dt;
		position.y += velocity.y * dt;

		break;

	case entity_state::DOWN_LEFT:

		position.x -= velocity.x * dt;
		position.y += velocity.y * dt;

		break;
	}
}


bool Alien::CleanUp()
{
	App->tex->UnLoad(graphics);
	graphics = nullptr;

	App->collisions->EraseCollider(collider);
	return true;
}
void Alien::Entity_OnCollision(Collider* C1, Collider* C2)
{
	if (C1->type == COLLIDER_ATTACK || C2->type == COLLIDER_ATTACK)
	{
		dead = true;
		CleanUp();
	}
	Entity_CollisionManager(C1, C2);
}
bool Alien::Load(pugi::xml_node&)
{
	return true;
}
bool Alien::Save(pugi::xml_node&) const
{
	return true;
}