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

	void Alien::Entity_OnCollision(Collider* C1, Collider* C2);

	void PathfindingLogic();
	void Move_entity(entity_state state, float dt);
	void SetState(iPoint enemyPos, iPoint playerPos);

	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;
public:
	iPoint player_goal;
	bool					followed_path;
	bool					targetPlayer;
	const p2DynArray<iPoint>* entity_path;
	Animation				fly_anim;
	p2DynArray<iPoint>*		follow_path;
};


#endif // __ModuleAlien__
