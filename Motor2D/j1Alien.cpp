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

	velocity = { 100,100 };
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
	Move_entity();
	collider->SetPos(position.x, position.y);
	return true;
}

bool Alien::PostUpdate()
{

	return true;
}
void Alien::Move_entity()
{
	player_goal.x = App->player->position.x;
	player_goal.y = App->player->position.y;

	if ((player_goal.x - position.x) < RADAR_RANGE
		&& (player_goal.x - position.x) > -RADAR_RANGE) {

		//Find the closest tile to current position
		App->pathfinding->CreatePath(iPoint(position.x, position.y), player_goal);

		const p2DynArray<iPoint>* Path = App->pathfinding->GetLastPath();
		LOG("PATH COUNT: %d", Path->Count());

		const iPoint* tile;
		if (Path->Count() != 0) {
			if (Path->Count() > 1) {
				tile = Path->At(1);
			}
			else
			{
				tile = Path->At(0);
			}

			iPoint center = App->map->MapToWorld(tile->x + App->map->data.tile_width / 2, tile->y + App->map->data.tile_height / 2);

			if (center.x > position.x) {
				position.x += velocity.x * App->dt;
				flip = true;
			}
			else if (center.x < position.x) {
				position.x -= velocity.x * App->dt;
				flip = false;
			}


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