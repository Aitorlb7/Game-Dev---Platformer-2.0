#include "UI_Button.h"
#include "j1App.h"
#include "j1Render.h"
#include "Brofiler\Brofiler.h"

void Button::setOutlined(bool isOutlined)
{
	if (text != nullptr)
		text->setOutlined(isOutlined);
}

void Button::appendChild(int x, int y, UI_element * child)
{
	child->localPosition = { x, y };
	child->setOriginalPos(x, y);
	child->parent = this;
	text = (Text*)child;
}

void Button::appendChildAtCenter(UI_element * child)
{
	iPoint child_pos(section.w / (2 / App->gui->UI_scale), section.h / (2 / App->gui->UI_scale));
	child_pos.x -= child->section.w / (2/App->gui->UI_scale);
	child_pos.y -= child->section.h / (2 / App->gui->UI_scale);
	child->localPosition = { child_pos.x, child_pos.y };
	child->setOriginalPos(child_pos.x, child_pos.y);
	child->parent = this;
	text = (Text*)child;
}

void Button::BlitElement()
{
	BROFILER_CATEGORY("Button Blit", Profiler::Color::DarkKhaki);
	iPoint globalPos = calculateAbsolutePosition();
	switch (state)
	{
	case STANDBY:
		if (!active)
			App->render->Blit(texture, globalPos.x, globalPos.y, &section, SDL_FLIP_NONE, App->gui->UI_scale);
		else
			App->render->Blit(texture, globalPos.x, globalPos.y, &sectionActive, SDL_FLIP_NONE, App->gui->UI_scale);
		break;
	case MOUSEOVER:
		if (!active)
			App->render->Blit(texture, globalPos.x, globalPos.y, &OnMouse, SDL_FLIP_NONE, App->gui->UI_scale);
		else
			App->render->Blit(texture, globalPos.x, globalPos.y, &OnMouseActive, SDL_FLIP_NONE, App->gui->UI_scale);
		break;
	case CLICKED:
		App->render->Blit(texture, globalPos.x, globalPos.y, &OnClick, SDL_FLIP_NONE, App->gui->UI_scale);
		break;
	}

	if (text != nullptr)
	{		
		text->BlitElement();
	}
}
