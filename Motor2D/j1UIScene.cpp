#include "j1UIScene.h"
#include "j1Scene.h"
#include "j1App.h"
#include "j1Fonts.h"
#include "UI_element.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1EntityManager.h"
#include "j1Audio.h"
#include "j1Window.h"

#include "p2Log.h"
#include "j1Player.h"

j1UIScene::j1UIScene()
{
	name.create("UIScene");
}

j1UIScene::~j1UIScene()
{
}

bool j1UIScene::Awake()
{
	return true;
}

bool j1UIScene::Start()
{
	
	return true;
}

bool j1UIScene::PreUpdate()
{
	return true;
}

bool j1UIScene::Update(float dt)
{

	return true;
}

bool j1UIScene::PostUpdate(float dt)
{
	return true;
}

bool j1UIScene::OnUIEvent(UI_element* element, event_type event_type)
{
	bool ret = true;

	if (event_type == MOUSE_ENTER)
	{
		element->state = MOUSEOVER;

	}
	else if (event_type == MOUSE_LEAVE)
	{
		element->state = STANDBY;

	}
	else if (event_type == MOUSE_LEFT_CLICK)
	{
		element->state = CLICKED;

		//if (element->element_type == SWITCH)
		//{
		//	Button* tmp = (Button*)element;
		//	tmp->active = !tmp->active;
		//	newValues.fullscreen = tmp->active;
		//}
		switch (element->function)
		{
		case NEW_GAME:
		case RESTART:

			break;
		case CONTINUE:

			break;
		case SETTINGS:

			break;
		case CREDITS:

			break;
		case EXIT:
			ret = false;
			break;
		case PAUSE:
			
			break;
		case APPLY:
	
			break;
		case CANCEL:

			break;
		case BACK:

			break;
		case RESTORE:

			break;
		case HOME:

			break;
		case WEB:

		}
	}
	else if (event_type == MOUSE_LEFT_RELEASE)
	{
		if (element->parent != nullptr && element->parent->element_type == SLIDER)
		{
			//Slider* tmp = (Slider*)element->parent;
			//switch (tmp->modify)
			//{
			//case MUSIC:
			//	newValues.music = tmp->progress;
			//	break;
			//case FX:
			//	newValues.fx = tmp->progress;
			//	break;
			//}
		}
		if (element->state == CLICKED)
			element->state = MOUSEOVER;
	}
	else if (event_type == MOUSE_RIGHT_CLICK)
	{
	}
	else if (event_type == MOUSE_RIGHT_RELEASE)
	{
	}
	else if (event_type == TIMER_ZERO)
	{
		LOG("Clock reached zero");
	}

	return ret;
}

bool j1UIScene::CleanUp()
{
	p2List_item<menu*>* item;
	item = menus.start;
	while (item)
	{
		delete item->data;
		item = item->next;
	}
	menus.clear();
	current_menu = nullptr;

	return true;
}

bool j1UIScene::Load(pugi::xml_node& data)
{
	
	return true;
}

bool j1UIScene::Save(pugi::xml_node& data) const
{
	return true;
}
