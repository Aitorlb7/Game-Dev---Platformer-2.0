#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Fonts.h"
#include "j1Input.h"
#include "j1Gui.h"

#include "j1GUIelement.h"

j1Gui::j1Gui() : j1Module()
{
	name.create("gui");
}

// Destructor
j1Gui::~j1Gui()
{}

// Called before render is available
bool j1Gui::Awake(pugi::xml_node& conf)
{
	LOG("Loading GUI atlas");
	bool ret = true;

	atlas_file_name = conf.child("atlas").attribute("file").as_string("");

	return ret;
}

// Called before the first frame
bool j1Gui::Start()
{
	atlas = App->tex->Load(atlas_file_name.GetString());

	return true;
}

// Update all guis
bool j1Gui::PreUpdate()
{
	return true;
}

// Called after all Updates
bool j1Gui::PostUpdate()
{
	return true;
}

// Called before quitting
bool j1Gui::CleanUp()
{
	LOG("Freeing GUI");

	return true;
}

// const getter for atlas
SDL_Texture* j1Gui::GetAtlas() const
{
	return atlas;
}

// class Gui ---------------------------------------------------
j1GUIelement* j1Gui::AddGUIelement(GUItype type, j1GUIelement* parent, iPoint globalPosition, iPoint localPosition, bool interactable, bool enabled, SDL_Rect section, char* text, j1Module* listener, bool X_drag, bool Y_drag)
{
	j1GUIelement* tmp = nullptr;

	//switch (type)
	//{

	//case GUItype::GUI_BUTTON:
	//	tmp = new j1GUIButton();
	//	break;
	//case GUItype::GUI_INPUTBOX:
	//	tmp = new j1GUIinputBox(text);
	//	break;
	//case GUItype::GUI_LABEL:
	//	tmp = new j1GUIlabel();
	//	break;
	//case GUItype::GUI_IMAGE:
	//	tmp = new j1GUIimage();
	//	break;
	//case GUItype::GUI_SCROLLBAR:
	//	tmp = new j1GUIscrollBar();
	//	break;
	//}

	if (tmp)
	{

		tmp->parent = parent;
		tmp->globalPosition = globalPosition;
		tmp->localPosition = localPosition;
		tmp->listener = listener;
		tmp->interactable = interactable;

		tmp->X_drag = X_drag;
		tmp->Y_drag = Y_drag;

		tmp->enabled = enabled;
		tmp->rect = section;
		tmp->text = text;

		GUIelementList.add(tmp)->data->Start();
	}


	return tmp;
}

