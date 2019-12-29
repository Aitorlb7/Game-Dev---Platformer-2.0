#include "GUIlabel.h"
#include "j1App.h"
#include "j1Fonts.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Input.h"

GUIlabel::GUIlabel() 
{
	this->type = GUItype::GUI_LABEL;
}

GUIlabel::~GUIlabel() {

}


bool GUIlabel::Awake(pugi::xml_node&)
{
	
	return true;
}

bool GUIlabel::Start()
{	
	texture = App->font->Print(text);
	return true;
}


bool GUIlabel::PreUpdate() 
{
	CleanUp();
	App->font->CalcSize(App->input->GetText().GetString(), rect.w, rect.h);
	return true;
}

bool GUIlabel::Update(float dt) 
{	
	
	texture = App->font->Print(text);
	return true;
}

bool GUIlabel::PostUpdate()
{		
	if (enabled)
	{
		App->render->Blit(texture, globalPosition.x + localPosition.x, globalPosition.y + localPosition.y, nullptr);
	}
			

	return true;
}

bool GUIlabel::CleanUp()
{
	App->tex->UnLoad(texture);
	return true;
}
