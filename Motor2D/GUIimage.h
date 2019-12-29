#ifndef __GUIIMAGE__
#define __GUIIMAGE__

#include "j1Module.h"
#include"j1GUIelement.h"
#include "j1GUI.h"

struct SDL_Texture;

class GUIimage : public j1GUIelement
{
public:

	GUIimage();
	~GUIimage();

	bool Awake(pugi::xml_node&);
		
	bool PreUpdate();
	bool PostUpdate();

	bool CleanUp();

	bool Load(pugi::xml_node&) { return true; };
	bool Save(pugi::xml_node&) const  const { return true; };

};


#endif // !__GUIIMAGE__

