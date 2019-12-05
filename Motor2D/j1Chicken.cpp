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
	collider = App->collisions->AddCollider({ (int)position.x, (int)position.y, 30, 45 }, COLLIDER_ENEMY, this);

	walk_anim.PushBack({ 50, 56, 38, 45 });
	walk_anim.PushBack({ 88, 56, 38, 45 });
	walk_anim.PushBack({ 126, 56, 38, 45 });
	walk_anim.PushBack({ 164, 56, 38, 45 });
	walk_anim.PushBack({ 202, 56, 38, 45 });
	walk_anim.PushBack({ 240, 56, 38, 45 });
	walk_anim.speed = 0.2f;

	current_anim = &walk_anim;
}

Chicken::~Chicken()
{

}
bool Chicken::Save(pugi::xml_node& node) const
{
	node.append_attribute("EntityType") = "GROUND_ENEMY";


	return true;
}
bool Chicken::Load(pugi::xml_node& node)
{


	return true;
}

bool Chicken::Awake(pugi::xml_node& config)
{
	return true;
}

bool Chicken::Update(float dt)
{

	collider->SetPos(position.x, position.y);
	return true;
}

bool Chicken::PostUpdate()
{

	return true;
}

bool Chicken::CleanUp()
{
	App->tex->UnLoad(graphics);
	graphics = nullptr;

	App->collisions->EraseCollider(collider);
	return true;
	return true;
}
void Chicken::Entity_OnCollision(Collider* C1, Collider* C2)
{
	if (C1->type == COLLIDER_ATTACK || C2->type == COLLIDER_ATTACK)
	{
		dead = true;
		CleanUp();
	}
	Entity_CollisionManager(C1, C2);
}
void Chicken::Move_entity()
{

}
