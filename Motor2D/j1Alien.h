#ifndef __ModuleAlien__
#define __ModuleAlien__
#define RADAR_RANGE 200

#include "j1Module.h"
#include "j1Entity.h"
#include "j1Animation.h"

class Alien : public Entity
{
public:
	Alien();
	~Alien();

	bool Awake(pugi::xml_node&);
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

	void OnCollision(Collider* c1, Collider* c2);
	void Move_entity();

	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;
public:
	iPoint player_goal;
	bool					followed_path;

	Animation				fly_anim;
	p2DynArray<iPoint>*		follow_path;
};


#endif // __ModuleAlien__
