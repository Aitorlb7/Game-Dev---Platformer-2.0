#include "GUIinputBox.h"
#include "j1App.h"
#include "j1Fonts.h"
#include "j1Render.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Window.h"

GUIinputBox::GUIinputBox(char* text) 
{
	this->type = GUItype::GUI_INPUTBOX;	
	texture = App->gui->GetAtlas();
}

GUIinputBox::~GUIinputBox() {

}


bool GUIinputBox::Awake(pugi::xml_node&)
{
	
	return true;
}

bool GUIinputBox::Start()
{
	string = App->gui->AddGUIelement(GUItype::GUI_LABEL, this, globalPosition, { 0,3 }, true, enabled, { localPosition.x,localPosition.y,50,50 }, text);
	return true;
}


bool GUIinputBox::PreUpdate()
{	
	string->enabled = enabled;	

	if (focus && enabled) 
	{
		App->input->EnableTextInput();		
		App->tex->UnLoad(string->texture);
		string->texture = App->fonts->Print(App->input->GetText().GetString());
	}

	else if (!focus || !enabled)
		App->input->DisableTextInput();

	above = OnAbove();

	return true;
}

bool GUIinputBox::Update(float dt)
{

	if (above) 
	{
		if (App->input->GetMouseButtonDown(1) == KEY_DOWN)
			OnClick();
	}	
	else {
		if (App->input->GetMouseButtonDown(1) == KEY_DOWN)
			focus=false;
	}
	

	return true;
}

bool GUIinputBox::PostUpdate()
{	
	if (enabled) {
		Draw();

		//Draws the cursor(rectangle) -------------
		if (focus)
		{
			SDL_Rect rect = { (string->globalPosition.x + App->input->GetCursorPosition()) * App->win->GetScale() , (string->globalPosition.y + localPosition.y) * App->win->GetScale(), 2,  string->rect.h + 10 };
			App->render->DrawQuad(rect, 255, 255, 255, 255, true, false);
		}
	}
	

	return true;
}

bool GUIinputBox::CleanUp()
{
	return true;
}

void GUIinputBox::OnClick()
{
	focus = true;	
	
}

void GUIinputBox::OnRelease()
{

}
