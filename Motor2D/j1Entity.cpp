#include "j1Entity.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Collisions.h"
#include "j1Input.h"
#include "j1Audio.h"
#include "p2Log.h"
#include "j1EntityManager.h"
#include "j1Window.h"
#include "j1Textures.h"
#include "j1PathFinding.h"
#include "j1Player.h"

Entity::Entity(const char* name)
{
	velocity.x = 0;
	velocity.y = 0;

}

Entity::~Entity()
{
	App->tex->UnLoad(graphics);
	graphics = nullptr;
	if (collider != nullptr)
	{
		collider->to_delete = true;
		collider = nullptr;
	}
}

bool Entity::Entity_Update(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
	{
		god_mode = !god_mode;
	}

	return true;
}


void Entity::Entity_CollisionManager(Collider* C1, Collider* C2)
{
	if ( C2->type == COLLIDER_UNPENETRABLE && god_mode == false)
	{
		if (C1->rect.x + C1->rect.w >= C2->rect.x && C1->rect.x <= C2->rect.x + C2->rect.w)
		{
			//Collision from top
			if ((C1->rect.y + C1->rect.h) >= (C2->rect.y) && (C1->rect.y < C2->rect.y))
			{
				velocity.y = 0;
				position.y = C2->rect.y - C1->rect.h + 1;
				is_grounded = true;

			}
			//Collision from below
			else if (C1->rect.y < (C2->rect.y + C2->rect.h) && (C1->rect.y + C1->rect.h)>(C2->rect.y + C2->rect.h))
			{
				velocity.y = 0;
				position.y = C2->rect.y + C2->rect.h;
			}
		}

		if (C1->rect.y < C2->rect.y + C2->rect.h && C1->rect.y + C1->rect.h  > C2->rect.y && C1->rect.y > C2->rect.y)
		{
			//Collision from the right
			if ((C1->rect.x + C1->rect.w) > (C2->rect.x) && C1->rect.x < (C2->rect.x))
			{
				position.x = C2->rect.x - C1->rect.w - 1;
				velocity.x = 0;

			}
			//Collision from the left
			else if (C1->rect.x  < (C2->rect.x + C2->rect.w) && C1->rect.x >(C2->rect.x))
			{
				position.x = C2->rect.x + C2->rect.w + 1;
				velocity.x = 0;

			}
		}
	}
	else if ( C2->type == COLLIDER_PENETRABLE && god_mode == false)
	{
		//Collision from top and below
		if ((C1->rect.y + C1->rect.h) > (C2->rect.y) && C1->rect.y < C2->rect.y && C1->rect.y + C1->rect.h - 8 < C2->rect.y && velocity.y >= 0)
		{
			position.y = C2->rect.y - C1->rect.h + 1;
			velocity.y = 0;
			is_grounded = true;

		}
	}
}






