#ifndef __ModuleChicken__
#define __ModuleChicken__

#define RADAR_RANGE 200

#include "j1Module.h"
#include "j1Entity.h"
#include "j1Animation.h"

class Chicken : public Entity
{
public:
	Chicken();
	~Chicken();

	bool Awake(pugi::xml_node&);
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

	void Chicken::Entity_OnCollision(Collider* C1, Collider* C2);
	void Move_entity();

	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;
public:

	bool					followed_path;

	Animation				walk_anim;
	p2DynArray<iPoint>*		follow_path;
};


#endif // __ModuleChicken__