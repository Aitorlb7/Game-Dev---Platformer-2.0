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
#include "j1App.h"
#include "j1FadeToBlack.h"

#include "UI_Button.h"
#include "UI_Image.h"
#include "UI_Slider.h"
#include "UI_Text.h"
#include "UI_Window.h"

#include "p2Log.h"
#include "j1Player.h"

j1UIScene::j1UIScene()
{
	name.create("UIScene");
	canPause = false;
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
	_TTF_Font* buttons_font = App->font->Load("fonts/mini_pixel/mini_pixel-7.ttf", 30);

	SDL_Color dark_green = { 0, 51, 0, 255 };

	float music_progress = (float)App->audio->getMusicVolume() / 128;
	float fx_progress = (float)App->audio->getFxVolume() / 128;

	menu* startMenu = new menu(START_MENU);
	{
		App->pause = true;

		UI_element* back_img = App->gui->createImage(0, 0, App->tex->Load("gui/Background.png"), this);

		//TITLE
		UI_element* title_img = App->gui->createImageFromAtlas(App->gui->UI_scale,App->gui->UI_scale, { 30,30,30,30 }, this);

		//NEW GAME
		UI_element* new_game = App->gui->createButton(OFFSET_TO_CENTER * App->gui->UI_scale, 200 *App->gui->UI_scale, NULL, { 30,30,30,30 }, { 30,30,30,30 }, { 30,30,30,30 }, this);
		new_game->function = NEW_GAME;

		UI_element* new_text = App->gui->createText("NEW GAME", 200, 200, buttons_font, dark_green);
		new_text->setOutlined(true);
		new_game->appendChildAtCenter(new_text);

		//CONTINUE GAME
		continueButton = App->gui->createButton(OFFSET_TO_CENTER * App->gui->UI_scale, 300 * App->gui->UI_scale, NULL, { 30,30,30,30 }, { 30,30,30,30 }, { 30,30,30,30 }, this);
		continueButton->function = CONTINUE;

		UI_element* continue_text = App->gui->createText("CONTINUE", 200, 200, buttons_font, dark_green);
		continue_text->setOutlined(true);
		continueButton->appendChildAtCenter(continue_text);

		//EXIT GAME
		UI_element* exit_game = App->gui->createButton(OFFSET_TO_CENTER *App->gui->UI_scale, 600 * App->gui->UI_scale, NULL, { 30,30,30,30 }, { 230,30,30,30 }, { 30,30,30,30 }, this);
		exit_game->function = EXIT;

		UI_element* exit_text = App->gui->createText("EXIT", 200, 200, buttons_font, dark_green);
		exit_text->setOutlined(true);
		exit_game->appendChildAtCenter(exit_text);

		//CREDITS
		UI_element* credits = App->gui->createButton(OFFSET_TO_CENTER * App->gui->UI_scale, 500 * App->gui->UI_scale, NULL, { 30,30,30,30 }, { 30,30,30,30 }, { 30,30,30,30 }, this);
		credits->function = CREDITS;

		UI_element* credits_text = App->gui->createText("CREDITS", 200, 200, buttons_font, dark_green);
		credits_text->setOutlined(true);
		credits->appendChildAtCenter(credits_text);

		//SETTINGS
		UI_element* settings_start_menu = App->gui->createButton(OFFSET_TO_CENTER *App->gui->UI_scale, 400 * App->gui->UI_scale, NULL, { 30,30,30,30 }, { 30,30,30,30 }, { 30,30,30,30 }, this);
		settings_start_menu->function = SETTINGS;

		UI_element* settings_text = App->gui->createText("SETTINGS", 200, 200, buttons_font, dark_green);
		settings_text->setOutlined(true);
		settings_start_menu->appendChildAtCenter(settings_text);

		startMenu->elements.add(back_img);
		startMenu->elements.add(title_img);
		startMenu->elements.add(new_game);
		startMenu->elements.add(new_text);
		startMenu->elements.add(continueButton);
		startMenu->elements.add(continue_text);
		startMenu->elements.add(exit_game);
		startMenu->elements.add(exit_text);
		startMenu->elements.add(credits);
		startMenu->elements.add(credits_text);
		startMenu->elements.add(settings_start_menu);
		startMenu->elements.add(settings_text);
		menus.add(startMenu);
	}

	current_menu = startMenu;

	defaultValues.fx = fx_progress;
	defaultValues.music = music_progress;
	newValues = defaultValues;
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

bool j1UIScene::UIEvent(UI_element* element, event_type event_type)
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
			App->pause = false;
			App->fade_to_black->FadeToBlack("Level1.tmx", 3.0f);
			break;
		case RESTART:
			App->pause = false;
			App->fade_to_black->FadeToBlack("Level1.tmx", 3.0f);
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
			App->RequestBrowser("https://frangv98.github.io/Game-Dev---Platformer-2.0/");
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
