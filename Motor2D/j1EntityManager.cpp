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

j1EntityManager::j1EntityManager()
{
	
}

j1EntityManager::~j1EntityManager()
{}

bool j1EntityManager::Awake(pugi::xml_node& config)
{
	this->config = config;

	return true;
}

bool j1EntityManager::Start()
{
	path_marker = App->tex->Load("maps/Non_walkable.png");

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
					//DeleteEntity(entity->data);
					continue;
				}
				// Path methods comes here
				/*if (entity->data->flip)
					App->render->Blit(entity->data->graphics, entity->data->position.x, entity->data->position.y, &entity->data->current_anim->GetCurrentFrame(), SDL_FLIP_HORIZONTAL);
				else
					App->render->Blit(entity->data->graphics, entity->data->position.x, entity->data->position.y, &entity->data->current_anim->GetCurrentFrame(), SDL_FLIP_NONE);*/

			}
		}
	}





	return true;
}

bool j1EntityManager::CleanUp()
{

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

		break;

	}
	
	ret->position.x = x; 
	ret->position.y = y;
	
	
	entities.add(ret);

	return ret;
}

bool j1EntityManager::Load(pugi::xml_node& data)
{
	CleanUp();
	getPlayer()->Load(data.child("player"));
	for (pugi::xml_node charger = data.child("chicken"); charger; charger = charger.next_sibling("chicken"))
	{
		createEntity(CHICKEN, charger.attribute("position_x").as_int(), charger.attribute("position_y").as_int());
	}

	return true;
}

bool j1EntityManager::Save(pugi::xml_node& data) const
{
	
	getPlayer()->Save(data.append_child("player"));
	for (p2List_item<Entity*>* entity = entities.start; entity; entity = entity->next)
	{
		pugi::xml_node child = data.append_child(entity->data->name.GetString());
		child.append_attribute("position_x") = entity->data->position.x;
		child.append_attribute("position_y") = entity->data->position.y;
	}
	return true;
}