#ifndef __GUILABEL__
#define __GUILABEL__

#include "j1Module.h"
#include"j1GUIelement.h"
#include "j1GUI.h"

struct SDL_Texture;

class GUIlabel : public j1GUIelement
{
public:

	GUIlabel();
	~GUIlabel();

	bool Awake(pugi::xml_node&);
	bool Start();

	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

	bool Load(pugi::xml_node&) { return true; };
	bool Save(pugi::xml_node&) const  const { return true; };

private:	

};

#endif // !__GUILABEL__

