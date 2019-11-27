#include "j1App.h"
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
#include "j1Chicken.h"
#include "j1Entity.h"
#include "j1EntityManager.h"

Chicken::Chicken() : Entity("chicken")
{
	name.create("chicken");

	graphics = App->tex->Load("textures/Chicken_Spritesheet.png");
	collider = App->collisions->AddCollider({ (int)position.x, (int)position.y, 60, 60 }, COLLIDER_ENEMY, this);

	walk_anim.PushBack({ 58, 56, 38, 45 });
	walk_anim.PushBack({ 96, 56, 38, 45 });
	walk_anim.PushBack({ 134, 56, 38, 45 });
	walk_anim.PushBack({ 172, 56, 38, 45 });
	walk_anim.PushBack({ 210, 56, 38, 45 });
	walk_anim.PushBack({ 248, 56, 38, 45 });
	walk_anim.speed = 0.2f;

	current_anim = &walk_anim;
}

Chicken::~Chicken()
{

}

bool Chicken::Awake(pugi::xml_node&)
{
	return true;
}

bool Chicken::Update(float dt)
{
	if (!dead)
	{
		if (!App->entity_manager->getPlayer()->dead) {
		if ((App->entity_manager->getPlayer()->position.x - position.x) < RADAR_RANGE
			&& (App->entity_manager->getPlayer()->position.x - position.x) > -RADAR_RANGE
			&& App->entity_manager->getPlayer()->collider->type == COLLIDER_PLAYER)
		{
			iPoint origin = { App->map->WorldToMap((int)position.x , (int)position.y ) };
			iPoint destination;

			if (position.x < App->entity_manager->getPlayer()->position.x)
			{
				destination = { App->map->WorldToMap((int)(App->entity_manager->getPlayer()->position.x + App->entity_manager->getPlayer()->collider->rect.w), 
					(int)(App->entity_manager->getPlayer()->position.y + App->entity_manager->getPlayer()->collider->rect.h / 2)) };

			}
			else
				destination = { App->map->WorldToMap((int)(App->entity_manager->getPlayer()->position.x), (int)(App->entity_manager->getPlayer()->position.y + App->entity_manager->getPlayer()->collider->rect.h)) };

			follow_path = App->pathfinding->CreatePath(origin, destination);
			Move_entity(*follow_path, dt);
			followed_path = true;
		}

			else if (followed_path)
			{
				follow_path->Clear();
				followed_path = false;
			}
		}
	}

	if (flip)
		App->render->Blit(graphics, position.x, position.y, &current_anim->GetCurrentFrame(), SDL_FLIP_HORIZONTAL, false);
	else
		App->render->Blit(graphics, position.x, position.y, &current_anim->GetCurrentFrame(), SDL_FLIP_NONE, false);
	return true;
}

bool Chicken::PostUpdate()
{

	return true;
}

bool Chicken::CleanUp()
{
	return true;
}
void Chicken::OnCollision(Collider* c1, Collider* c2)
{
	Entity_CollisionManager(c1, c2);
}
//void Chicken::Move_entity(p2DynArray<iPoint>& path, float dt)
//{
//	if (path.Count() >= 2)
//	{
//		iPoint tile = path[0];
//		iPoint next_tile = path[1];
//
//		int x_difference = next_tile.x - tile.x;
//		int y_difference = next_tile.y - tile.y;
//
//		if (x_difference == 1) // going right
//		{
//			current_anim = &walk_anim;
//			position.x = velocity.x * dt;
//			flip = false;
//		}
//		else if (x_difference == -1)// going left
//		{
//			current_anim = &walk_anim;
//			position.x = -velocity.x * dt;
//			flip = false;
//		}
//		else if (y_difference == 1);// yet to complete
//		else if (y_difference == -1);// yet to complete
//	}
//}

bool Chicken::Load(pugi::xml_node&)
{
	return true;
}
bool Chicken::Save(pugi::xml_node&) const
{
	return true;
}