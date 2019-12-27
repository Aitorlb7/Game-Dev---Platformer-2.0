#ifndef __j1GUI_H__
#define __j1GUI_H__

#include "p2SString.h"
#include "j1App.h"
#include "j1Module.h"
#include "j1Render.h"
#include "p2Point.h"
#include "SDL/include/SDL_rect.h"
#include "p2List.h"
#include "j1Textures.h"
#include "j1Fonts.h"

#define CURSOR_WIDTH 2

// TODO 1: Create your structure of classes
class Entity;
// ---------------------------------------------------
class j1Gui : public j1Module
{
public:

	j1Gui();

	// Destructor
	virtual ~j1Gui();

	// Called when before render is available
	bool Awake(pugi::xml_node&);

	// Call before first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called after all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	// TODO 2: Create the factory methods
	j1GUIelement* AddGUIelement(GUItype type, j1GUIelement* parent, iPoint globalPosition, iPoint localPosition, bool interactable, bool enabled, SDL_Rect section, char* text = nullptr, j1Module* listener = nullptr, bool X_drag = false, bool Y_drag = false);
	SDL_Texture* j1Gui::GetAtlas() const;

	p2List<j1GUIelement*>	GUIelementList;
	// Gui creation functions

private:

	SDL_Texture* atlas;

	p2SString atlas_file_name;

	//p2List<UI*> UI_list;
};

#endif // __j1GUI_H__