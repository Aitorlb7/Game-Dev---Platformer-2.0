#include "j1EntityManager.h"
#include "p2Log.h"
#include "j1Render.h"
#include "j1Map.h"
#include "j1App.h"
#include "j1Collisions.h"
#include "Brofiler\Brofiler.h"
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

bool j1EntityManager::Load(pugi::xml_node& data)
{
	return true;
}

bool j1EntityManager::Save(pugi::xml_node& data) const
{
	return true;
}