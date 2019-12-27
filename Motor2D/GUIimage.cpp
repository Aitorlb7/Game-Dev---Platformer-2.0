#include "GUIimage.h"
#include "j1App.h"


GUIimage::GUIimage() {

	this->type = GUItype::GUI_IMAGE;

	texture = App->gui->GetAtlas();
}

GUIimage::~GUIimage() {

}


bool GUIimage::Awake(pugi::xml_node&)
{

	return true;
}

bool GUIimage::PreUpdate()
{

	return true;
}

bool GUIimage::PostUpdate()
{	
	if(enabled)
		Draw();

	return true;
}

bool GUIimage::CleanUp()
{
	return true;
}

