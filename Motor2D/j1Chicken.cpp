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

bool Chicken::Load(pugi::xml_node&)
{
	return true;
}
bool Chicken::Save(pugi::xml_node&) const
{
	return true;
}