#include "j1EntityManager.h"
#include "p2Log.h"
#include "j1Render.h"
#include "j1Map.h"
#include "j1App.h"
#include "j1Collisions.h"
#include "j1Input.h"
#include "j1Player.h"
#include "j1Window.h"

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

	return true;
}

bool j1EntityManager::CleanUp()
{

	return true;
}

Entity* j1EntityManager::createEntity(entity_type type, int x, int y, int id)
{
	Entity* ret = nullptr;

	switch (type)
	{
	case CHICKEN:

		break;
	case ALIEN:

		break;

	}
	entities.add(ret);

	return ret;
}

bool j1EntityManager::Load(pugi::xml_node& data)
{
	return true;
}

bool j1EntityManager::Save(pugi::xml_node& data) const
{
	return true;
}