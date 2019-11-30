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
	return true;
}

bool Alien::PostUpdate()
{

	return true;
}

bool Alien::CleanUp()
{
	return true;
}
void Alien::OnCollision(Collider* c1, Collider* c2)
{
	Entity_CollisionManager(c1, c2);
}
bool Alien::Load(pugi::xml_node&)
{
	return true;
}
bool Alien::Save(pugi::xml_node&) const
{
	return true;
}