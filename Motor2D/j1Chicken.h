#ifndef __ModuleChicken__
#define __ModuleChicken__


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
	void OnCollision(Collider* c1, Collider* c2);

	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;


};


#endif // __ModuleChicken__