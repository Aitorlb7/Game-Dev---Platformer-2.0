#include "GUIscrollBar.h"
#include "j1App.h"
#include "j1Fonts.h"
#include "j1Render.h"
#include "j1Input.h"


GUIscrollBar::GUIscrollBar() {

	this->type = GUItype::GUI_SCROLLBAR;	
}

GUIscrollBar::~GUIscrollBar() 
{

}


bool GUIscrollBar::Awake(pugi::xml_node&)
{

	return true;
}


bool GUIscrollBar::Start()
{
	scrollButton = App->gui->AddGUIelement(GUItype::GUI_BUTTON, this, globalPosition, localPosition, true, true, { 864, 72, 28 , 32 }, nullptr, this->listener, true, false);
	scrollButton->globalPosition.y = globalPosition.y - scrollButton->rect.h / 2 + this->rect.h / 2;
	value = 0;

	texture = App->gui->GetAtlas();
	return true;
}

bool GUIscrollBar::PreUpdate()
{
	
	scrollButton->enabled = enabled;
	above = OnAbove();

	return true;
}

bool GUIscrollBar::Update(float dt)
{	
	if (interactable) {
		if (above)
		{
			if (App->input->GetMouseButtonDown(1) == KEY_DOWN)
				OnClick();
		}
	}
	
	return true;
}

bool GUIscrollBar::PostUpdate()
{	
	ScrollLimits();
	value = -((float(-scrollButton->localPosition.x) / (float(-this->rect.w) + float(scrollButton->rect.w))) * 128);

	if(enabled)
		Draw();

	return true;
}



bool GUIscrollBar::CleanUp()
{
	return true;
}


void GUIscrollBar::ScrollLimits() {

	if (scrollButton->localPosition.x > 0)
	{
		scrollButton->localPosition.x = 0;

		scrollButton->globalPosition.x = scrollButton->parent->globalPosition.x - scrollButton->localPosition.x;
		//scrollButton->globalPosition.y = scrollButton->parent->globalPosition.y - scrollButton->localPosition.y;
	}
	else if (scrollButton->localPosition.x < (-this->rect.w + scrollButton->rect.w))
	{
		scrollButton->localPosition.x = -this->rect.w + scrollButton->rect.w;

		scrollButton->globalPosition.x = scrollButton->parent->globalPosition.x - scrollButton->localPosition.x;
		//scrollButton->globalPosition.y = scrollButton->parent->globalPosition.y - scrollButton->localPosition.y;
	}

}