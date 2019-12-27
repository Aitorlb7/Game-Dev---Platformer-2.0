#ifndef __GUIINPUTBOX__
#define __GUIINPUTBOX__

#include "j1Module.h"
#include"j1GUIelement.h"
#include "j1GUI.h"

struct SDL_Texture;

class GUIinputBox : public j1GUIelement
{
public:


	GUIinputBox(char* text);
	~GUIinputBox();

	bool Awake(pugi::xml_node&);
	bool Start();

	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();

	bool CleanUp();

	bool Load(pugi::xml_node&) { return true; };
	bool Save(pugi::xml_node&) const  const { return true; };
	
	void OnClick();
	void OnRelease();
private:
		
	j1GUIelement* string = nullptr;
	j1GUIelement* background = nullptr;	
};






#endif // !__GUIINPUTBOX__

