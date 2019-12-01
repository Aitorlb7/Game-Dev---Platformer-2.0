#include "j1EntityManager.h"
#include "p2Log.h"
#include "j1Render.h"
#include "j1Map.h"
#include "j1App.h"
#include "j1Collisions.h"
#include "j1Input.h"
#include "j1Player.h"
#include "j1Window.h"
#include "j1Chicken.h"
#include "j1Alien.h"

j1EntityManager::j1EntityManager()
{
	name.create("entityManager");

}

j1EntityManager::~j1EntityManager()
{}



bool j1EntityManager::Awake(pugi::xml_node& config_)
{
	for (p2List_item<Entity*>* entity = entities.start; entity; entity = entity->next)
	{
		if (entity->data->position.x <= App->win->width && entity->data->position.x >= -50)
		{
			entity->data->Awake(config_);
		}
	}
	return true;
}

bool j1EntityManager::Start()
{
	path_marker = App->tex->Load("maps/Non_walkable.png");
	return true;
}
bool j1EntityManager::PreUpdate()
{
	for (p2List_item<Entity*>* entity = entities.start; entity; entity = entity->next)
	{
		if (entity->data->position.x <= App->win->width && entity->data->position.x >= -50)
		{
			entity->data->PreUpdate();
		}
	}
	return true;
}

bool j1EntityManager::Update(float dt)
{
	for (p2List_item<Entity*>* entity = entities.start; entity; entity = entity->next)
	{
		if (entity->data->position.x <= App->win->width + DRAW_LIMIT && entity->data->position.x >= -DRAW_LIMIT)
		{
			entity->data->Update(dt);
		}
	}

	// Future draw of the enemy path (needs pathfinding)
	if (App->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
		draw_path = !draw_path;

	return true;
}

bool j1EntityManager::PostUpdate()
{
	for (p2List_item<Entity*>* entity = entities.start; entity; entity = entity->next)
	{
		if (entity->data->position.x <= App->win->width && entity->data->position.x >= -50)
		{
			entity->data->PostUpdate();
			if (entity->data->type != PLAYER)
			{
				if (entity->data->position.y > App->map->data.height* App->map->data.tile_height)
				{
					DeleteEntity(entity->data);
					continue;
				}

				else if (entity->data->flip)
					App->render->Blit(entity->data->graphics, entity->data->position.x, entity->data->position.y, &entity->data->current_anim->GetCurrentFrame(), SDL_FLIP_NONE);
				else
					App->render->Blit(entity->data->graphics, entity->data->position.x, entity->data->position.y, &entity->data->current_anim->GetCurrentFrame(), SDL_FLIP_HORIZONTAL);

			}
		}
	}
	return true;
}

void j1EntityManager::Entity_OnCollision(Collider* c1, Collider* c2)
{
	for (p2List_item<Entity*>* entity = entities.start; entity; entity = entity->next)
	{
			entity->data->Entity_OnCollision(c1, c2);
			break;

	}
}

bool j1EntityManager::CleanUp()
{
	Entity* player = getPlayer();

	p2List_item<Entity*>* item;
	item = entities.start->next; //Skip first entity, player

	while (item != NULL)
	{
		RELEASE(item->data);
		item = item->next;
	}
	entities.clear();
	entities.add(player);

	return true;
}

Entity* j1EntityManager::getPlayer() const
{
	for (p2List_item<Entity*>* entity = entities.start; entity; entity = entity->next)
	{
		if (entity->data->type == PLAYER)
		{
			return entity->data;
			break;
		}
	}
}

Entity* j1EntityManager::createEntity(entity_type type, int x, int y)
{
	Entity* ret = nullptr;

	switch (type)
	{
	case CHICKEN:
		ret = new Chicken();
		break;
	case ALIEN:
		ret = new Alien();
		break;
	}
	
	ret->position.x = x; 
	ret->position.y = y;
	ret->type = type;
	entities.add(ret);

	return ret;
}
void j1EntityManager::DeleteEntity(Entity* entity_to_delete)
{
	p2List_item<Entity*>* entity_finder = entities.start;
	while (entity_finder != NULL)
	{
		if (entity_finder->data == entity_to_delete)
		{
			if (entity_finder->data == getPlayer())
				getPlayer()->CleanUp();
			entities.del(entity_finder);
			RELEASE(entity_finder->data);
			break;
		}
		entity_finder = entity_finder->next;
	}
}

bool j1EntityManager::Load(pugi::xml_node& data)
{
	return true;
}

bool j1EntityManager::Save(pugi::xml_node& data) const
{
	return true;
}