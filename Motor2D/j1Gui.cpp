#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Fonts.h"
#include "j1Input.h"
#include "j1Gui.h"
#include "j1Window.h"
#include "j1UIScene.h"
#include "UI_Image.h"

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
	UI_scale = App->win->GetScale();
	UI_scale = 1 / UI_scale;

	return true;
}

// Update all guis
bool j1Gui::PreUpdate()
{
	return true;
}

bool j1Gui::Update(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_F8) == KEY_DOWN)
		UI_Debug = !UI_Debug;

	return true;
}

// Called after all Updates
bool j1Gui::PostUpdate()
{
	if (App->ui_scene->current_menu != nullptr)
	{
		for (p2List_item<UI_element*>* item = App->ui_scene->current_menu->elements.start; item; item = item->next)
		{

		}
	}
	if (UI_Debug)
		UIDebugDraw();

	return true;
}

// Called before quitting
bool j1Gui::CleanUp()
{
	LOG("Freeing GUI");

	p2List_item<UI_element*>* item;
	item = UI_elements.start;
	while (item != NULL)
	{
		RELEASE(item->data);
		item = item->next;
	}
	UI_elements.clear();
	return true;
}

// const getter for atlas
const SDL_Texture* j1Gui:: GetAtlas() const
{
	return atlas;
}

Image* j1Gui::createImage(int x, int y, SDL_Texture* texture, j1Module* callback)
{
	uint tex_width, tex_height;
	App->tex->GetSize(texture, tex_width, tex_height);
	Image* ret = new Image(texture, x, y, { 0, 0, (int)tex_width, (int)tex_height }, callback);
	UI_elements.add(ret);

	return ret;
}


