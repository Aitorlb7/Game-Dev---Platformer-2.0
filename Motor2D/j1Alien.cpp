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
void Alien::PathfindingLogic()
{
	iPoint enemyPos(App->map->WorldToMap(position.x, position.y));
	iPoint playerPos(App->map->WorldToMap(App->player->position.x, App->player->position.y));
	
	enemyPos.y += 1;

	if (enemyPos.DistanceNoSqrt(playerPos) <= RADAR_RANGE)
	{
		App->pathfinding->CreatePath(enemyPos, playerPos);
		targetPlayer = true;

		if (targetPlayer == false)
		{
			targetPlayer = true;
		}

		if (targetPlayer == true)
		{
			App->pathfinding->CreatePath(enemyPos, playerPos);

			entity_path = App->pathfinding->GetLastPath();

			for (int i = 0; i < entity_path->Count(); ++i)
			{
				if (enemyPos.x != entity_path->At(i)->x || enemyPos.y != entity_path->At(i)->y)	
				{
					iPoint nextStep(entity_path->At(i)->x, entity_path->At(i)->y); 
					SetState(enemyPos, nextStep);
				}
			}
		}
	}
}
void Alien::Move_entity(entity_state state, float dt)
{
	switch (state)
	{
	case entity_state::UP:

		position.y -= velocity.y * dt;

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
void Alien::SetState(iPoint enemyPos, iPoint playerPos)
{
	iPoint checkRight(enemyPos.x + 1, enemyPos.y);
	iPoint checkLeft(enemyPos.x - 1, enemyPos.y);
	iPoint checkUp(enemyPos.x, enemyPos.y - 1);
	iPoint checkDown(enemyPos.x, enemyPos.y + 1);

	if (playerPos.y < enemyPos.y && playerPos.x == enemyPos.x && App->pathfinding->IsWalkable(checkUp))
	{
		state = entity_state::UP;
	}

	if (playerPos.y > enemyPos.y && playerPos.x == enemyPos.x && App->pathfinding->IsWalkable(checkDown))
	{
		state = entity_state::DOWN;
	}

	if (playerPos.x > enemyPos.x && playerPos.y == enemyPos.y && App->pathfinding->IsWalkable(checkRight))
	{
		state = entity_state::RIGHT;
	}

	if (playerPos.x < enemyPos.x && playerPos.y == enemyPos.y && App->pathfinding->IsWalkable(checkLeft))
	{
		state = entity_state::LEFT;
	}

	if (playerPos.x > enemyPos.x && playerPos.y < enemyPos.y)
	{
		if (App->pathfinding->IsWalkable(checkUp) && App->pathfinding->IsWalkable(checkRight))
		{
			state = entity_state::UP_RIGHT;
		}
		else if (!App->pathfinding->IsWalkable(checkUp) && App->pathfinding->IsWalkable(checkRight))
		{
			state = entity_state::RIGHT;
		}
		else if (App->pathfinding->IsWalkable(checkUp) && !App->pathfinding->IsWalkable(checkRight))
		{
			state = entity_state::UP;
		}
	}

	if (playerPos.x < enemyPos.x && playerPos.y < enemyPos.y)
	{
		if (App->pathfinding->IsWalkable(checkUp) && App->pathfinding->IsWalkable(checkLeft))
		{
			state = entity_state::UP_LEFT;
		}
		else if (!App->pathfinding->IsWalkable(checkUp) && App->pathfinding->IsWalkable(checkLeft))
		{
			state = entity_state::LEFT;
		}
		else if (App->pathfinding->IsWalkable(checkUp) && !App->pathfinding->IsWalkable(checkLeft))
		{
			state = entity_state::UP;
		}
	}

	if (playerPos.x > enemyPos.x && playerPos.y > enemyPos.y)
	{
		if (App->pathfinding->IsWalkable(checkDown) && App->pathfinding->IsWalkable(checkRight))
		{
			state = entity_state::DOWN_RIGHT;
		}
		else if (!App->pathfinding->IsWalkable(checkDown) && App->pathfinding->IsWalkable(checkRight))
		{
			state = entity_state::RIGHT;
		}
		else if (App->pathfinding->IsWalkable(checkDown) && !App->pathfinding->IsWalkable(checkRight))
		{
			state = entity_state::DOWN;
		}
	}

	if (playerPos.x < enemyPos.x && playerPos.y > enemyPos.y)
	{
		if (App->pathfinding->IsWalkable(checkDown) && App->pathfinding->IsWalkable(checkLeft))
		{
			state = entity_state::DOWN_LEFT;
		}
		else if (!App->pathfinding->IsWalkable(checkDown) && App->pathfinding->IsWalkable(checkLeft))
		{
			state = entity_state::LEFT;
		}
		else if (App->pathfinding->IsWalkable(checkDown) && !App->pathfinding->IsWalkable(checkLeft))
		{
			state = entity_state::DOWN;
		}
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