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
	_TTF_Font* text_font = App->font->Load("fonts/mini_pixel/mini_pixel-7.ttf", 20);

	SDL_Color dark_green = { 10, 51, 10, 255 };
	SDL_Color black = { 0, 0, 0, 255 };

	float music_progress = (float)App->audio->getMusicVolume() / 128;
	float fx_progress = (float)App->audio->getFxVolume() / 128;

	menu* startMenu = new menu(START_MENU);
	{
	
		App->pause = true;

		UI_element* back_img = App->gui->createImage(0, 0, App->tex->Load("gui/Background.png"), this);

		//NEW GAME
		UI_element* new_game = App->gui->createButton(OFFSET_TO_CENTER * App->gui->UI_scale, 150 *App->gui->UI_scale, NULL, { 4,167,146,55 }, { 151,167,146,55 }, { 300,167,146,55 }, this);
		new_game->function = NEW_GAME;

		UI_element* new_text = App->gui->createText(" NEW GAME", 200, 200, buttons_font, dark_green);
		new_text->setOutlined(true);
		new_game->appendChildAtCenter(new_text);

		//CONTINUE GAME
		UI_element* continue_button = App->gui->createButton(OFFSET_TO_CENTER * App->gui->UI_scale, 250 * App->gui->UI_scale, NULL, { 4,167,146,55 }, { 151,167,146,55 }, { 300,167,146,55 }, this);
		continue_button->function = CONTINUE;

		UI_element* continue_text = App->gui->createText(" CONTINUE", 200, 200, buttons_font, dark_green);
		continue_text->setOutlined(true);
		continue_button->appendChildAtCenter(continue_text);

		//EXIT GAME
		UI_element* exit_game = App->gui->createButton(OFFSET_TO_CENTER *App->gui->UI_scale, 550 * App->gui->UI_scale, NULL, { 4,167,146,55 }, { 151,167,146,55 }, { 300,167,146,55 }, this);
		exit_game->function = EXIT;

		UI_element* exit_text = App->gui->createText(" EXIT", 200, 200, buttons_font, dark_green);
		exit_text->setOutlined(true);
		exit_game->appendChildAtCenter(exit_text);

		//CREDITS
		UI_element* credits = App->gui->createButton(OFFSET_TO_CENTER * App->gui->UI_scale, 450 * App->gui->UI_scale, NULL, { 4,167,146,55 }, { 151,167,146,55 }, { 300,167,146,55 }, this);
		credits->function = CREDITS;

		UI_element* credits_text = App->gui->createText(" CREDITS", 200, 200, buttons_font, dark_green);
		credits_text->setOutlined(true);
		credits->appendChildAtCenter(credits_text);

		//SETTINGS
		UI_element* settings_start_menu = App->gui->createButton(OFFSET_TO_CENTER *App->gui->UI_scale, 350 * App->gui->UI_scale, NULL, { 4,167,146,55 }, { 151,167,146,55 }, { 300,167,146,55 }, this);
		settings_start_menu->function = SETTINGS;

		UI_element* settings_text = App->gui->createText(" SETTINGS", 200, 200, buttons_font, dark_green);
		settings_text->setOutlined(true);
		settings_start_menu->appendChildAtCenter(settings_text);

		startMenu->elements.add(back_img);
		startMenu->elements.add(new_game);
		startMenu->elements.add(new_text);
		startMenu->elements.add(continue_button);
		startMenu->elements.add(continue_text);
		startMenu->elements.add(exit_game);
		startMenu->elements.add(exit_text);
		startMenu->elements.add(credits);
		startMenu->elements.add(credits_text);
		startMenu->elements.add(settings_start_menu);
		startMenu->elements.add(settings_text);
		menus.add(startMenu);
	}
	menu* noMenu = new menu(NO_MENU);
	{
		menus.add(noMenu);
	}

	menu* settingstMenu = new menu(SETTINGS_MENU);
	{

		UI_element* settings_window = App->gui->createWindow(320 * App->gui->UI_scale, 120 * App->gui->UI_scale, App->tex->Load("gui/menuwindow.png"), { 0,0,418,398 }, this);

		//MUSIC TEXT
		UI_element* music_txt = App->gui->createText("MUSIC", 0, 0, text_font, dark_green, this);
		music_txt->setOutlined(true);
		settings_window->appendChild(50 * App->gui->UI_scale, 50 * App->gui->UI_scale, music_txt);

		//FX TEXT
		UI_element* fx_txt = App->gui->createText("FX", 0, 0, text_font, dark_green, this);
		fx_txt->setOutlined(true);
		settings_window->appendChild(50 * App->gui->UI_scale, 100 * App->gui->UI_scale, fx_txt);

		//MUSIC SLIDER
		Button* slider_button = App->gui->createButton(0, 0, NULL, { 1,247,25,38 }, { 27,247,25,38 }, { 27,247,25,38 }, this);
		Slider* slider = App->gui->createSlider(0, 0, NULL, { 1,235, 275,12 }, { 1,223, 275,12 }, slider_button, text_font, dark_green, music_progress);
		slider->modify = MUSIC;
		settings_window->appendChild(100 * App->gui->UI_scale, 55 * App->gui->UI_scale, slider);

		//FX SLIDER
		Button* fx_slider_button = App->gui->createButton(0, 0, NULL, { 1,247,25,38 }, { 27,247,25,38 }, { 27,247,25,38 }, this);
		Slider* fx_slider = App->gui->createSlider(0, 0, NULL, { 1,235, 275,12 }, { 1,223, 275,12 }, fx_slider_button, text_font, dark_green, fx_progress);
		fx_slider->modify = FX;
		settings_window->appendChild(100 * App->gui->UI_scale, 105 * App->gui->UI_scale, fx_slider);

		//APPLY BUTTON
		UI_element* apply_button = App->gui->createButton(0, 0, NULL, { 4,167,146,55 }, { 151,167,146,55 }, { 300,167,146,55 }, this);
		settings_window->appendChild(130 * App->gui->UI_scale, 210 * App->gui->UI_scale, apply_button);
		apply_button->function = APPLY;

		UI_element* apply_text = App->gui->createText(" APPLY", 200, 200, buttons_font, dark_green);
		apply_text->setOutlined(true);
		apply_button->appendChildAtCenter(apply_text);

		//CANCEL BUTTON
		UI_element* cancel_button = App->gui->createButton(0,0, NULL, { 4,167,146,55 }, { 151,167,146,55 }, { 300,167,146,55 }, this);
		settings_window->appendChild(230 * App->gui->UI_scale, 300 * App->gui->UI_scale, cancel_button);
		cancel_button->function = CANCEL;

		UI_element* cancel_text = App->gui->createText(" CANCEL", 200, 200, buttons_font, dark_green);
		cancel_text->setOutlined(true);
		cancel_button->appendChildAtCenter(cancel_text);

		//RESTORE BUTTON
		UI_element* restore_button = App->gui->createButton(0, 0, NULL, { 4,167,146,55 }, { 151,167,146,55 }, { 300,167,146,55 }, this);
		settings_window->appendChild(50 * App->gui->UI_scale, 300 * App->gui->UI_scale, restore_button);
		restore_button->function = RESTORE;

		UI_element* restore_text = App->gui->createText(" RESTORE", 200, 200, buttons_font, dark_green);
		restore_text->setOutlined(true);
		restore_button->appendChildAtCenter(restore_text);
		
		settingstMenu->elements.add(settings_window);
		settingstMenu->elements.add(music_txt);
		settingstMenu->elements.add(fx_txt);
		settingstMenu->elements.add(slider_button);
		settingstMenu->elements.add(slider);
		settingstMenu->elements.add(fx_slider_button);
		settingstMenu->elements.add(fx_slider);
		settingstMenu->elements.add(cancel_button);
		settingstMenu->elements.add(cancel_text);
		settingstMenu->elements.add(apply_button);
		settingstMenu->elements.add(apply_text);
		settingstMenu->elements.add(restore_button);
		settingstMenu->elements.add(restore_text);


		menus.add(settingstMenu);
	}
	menu* inGameMenu = new menu(INGAME_MENU);
	{
		UI_element* window = App->gui->createWindow(320 * App->gui->UI_scale, 120 * App->gui->UI_scale, App->tex->Load("gui/menuwindow.png"), { 0,0,418,398 }, this);
		
		//CONTINUE
		UI_element* continue_button = App->gui->createButton(0, 0, NULL, { 4,167,146,55 }, { 151,167,146,55 }, { 300,167,146,55 }, this);
		window->appendChild(230 * App->gui->UI_scale, 260 * App->gui->UI_scale, continue_button);
		continue_button->function = CONTINUE;

		UI_element* continue_text = App->gui->createText("  CONTINUE", 200, 200, buttons_font, dark_green);
		continue_text->setOutlined(true);
		continue_button->appendChildAtCenter(continue_text);

		//HOME
		UI_element* home_button = App->gui->createButton(0, 0, NULL, { 4,167,146,55 }, { 151,167,146,55 }, { 300,167,146,55 }, this);
		window->appendChild(50 * App->gui->UI_scale, 260 * App->gui->UI_scale, home_button);
		home_button->function = HOME;

		UI_element* home_text = App->gui->createText("  HOME", 200, 200, buttons_font, dark_green);
		home_text->setOutlined(true);
		home_button->appendChildAtCenter(home_text);

		//SETTINGS
		UI_element* settings_button = App->gui->createButton(0, 0, NULL, { 4,167,146,55 }, { 151,167,146,55 }, { 300,167,146,55 }, this);
		window->appendChild(130 * App->gui->UI_scale, 120 * App->gui->UI_scale, settings_button);
		settings_button->function = SETTINGS;

		UI_element* settings_text = App->gui->createText("  SETTINGS", 200, 200, buttons_font, dark_green);
		settings_text->setOutlined(true);
		settings_button->appendChildAtCenter(settings_text);

		inGameMenu->elements.add(window);
		inGameMenu->elements.add(continue_button);
		inGameMenu->elements.add(continue_text);
		inGameMenu->elements.add(home_button);
		inGameMenu->elements.add(settings_button);
		inGameMenu->elements.add(settings_text);

		menus.add(inGameMenu);

	}
	menu* creditsMenu = new menu(CREDITS_MENU);
	{
		UI_element* window = App->gui->createWindow(320 * App->gui->UI_scale, 120 * App->gui->UI_scale, App->tex->Load("gui/credits.png"), { 0,0,418,398 }, this);
		
		
		//BACK
		UI_element* back_button = App->gui->createButton(0, 0, NULL, { 307,31,87,58 }, { 394,31,87,58 }, { 394,31,87,58 }, this);
		window->appendChild(230 * App->gui->UI_scale, 310 * App->gui->UI_scale, back_button);
		back_button->function = BACK;

		UI_element* back_text = App->gui->createText("  BACK", 200, 200, buttons_font, dark_green);
		back_text->setOutlined(true);
		back_button->appendChildAtCenter(back_text);

		//WEB BUTTON
		UI_element* web_button = App->gui->createButton(0, 0, NULL, { 4,167,146,55 }, { 151,167,146,55 }, { 300,167,146,55 }, this);
		window->appendChild(50 * App->gui->UI_scale, 310 * App->gui->UI_scale, web_button);
		web_button->function = WEB;

		UI_element* web_text = App->gui->createText("  WEB", 200, 200, buttons_font, dark_green);
		web_text->setOutlined(true);
		web_button->appendChildAtCenter(web_text);

		creditsMenu->elements.add(window);
		creditsMenu->elements.add(back_button);
		creditsMenu->elements.add(back_text);
		creditsMenu->elements.add(web_button);
		creditsMenu->elements.add(web_text);
		menus.add(creditsMenu);
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
		switch (element->function)
		{
		case NEW_GAME:
		case RESTART:
			loadMenu(NO_MENU);
			App->pause = false;
			break;
		case CONTINUE:
			App->pause = false;
			loadMenu(NO_MENU);
			break;
		case SETTINGS:
			App->pause = true;
			loadMenu(SETTINGS_MENU);
			break;
		case CREDITS:
			loadMenu(CREDITS_MENU);
			break;
		case EXIT:
			ret = false;
			break;
		case PAUSE:
			if (!App->pause)
			{
				App->pause = true;
				loadMenu(PAUSE_MENU);
			}
			else
			{
				App->pause = true;
				loadMenu(INGAME_MENU);
			}
			break;
		case APPLY:
			applySettings(newValues);
			loadMenu(previous_menu);
			break;
		case CANCEL:
			newValues = startValues;
			applySettings(startValues);
			loadMenu(previous_menu);
			break;
		case BACK:
			loadMenu(previous_menu);
			break;
		case RESTORE:
			applySettings(defaultValues);
			loadMenu(previous_menu);
			break;
		case HOME:
			loadMenu(START_MENU);
			break;

		case WEB:
			App->RequestBrowser("https://frangv98.github.io/Game-Dev---Platformer-2.0/");
		}
		
		
	}
	else if (event_type == MOUSE_LEFT_RELEASE)
	{
		if (element->parent != nullptr && element->parent->element_type == SLIDER)
		{
			Slider* tmp = (Slider*)element->parent;
			switch (tmp->modify)
			{
			case MUSIC:
				newValues.music = tmp->progress;
				break;
			case FX:
				newValues.fx = tmp->progress;
				break;
			}
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

bool j1UIScene::loadMenu(menu_id id)
{
	bool ret = false;

	previous_menu = current_menu->id;
	for (p2List_item<menu*>* item = menus.start; item; item = item->next)
	{
		if (item->data->id == id)
		{
			current_menu = item->data;
			ret = true;
			if (id == SETTINGS_MENU)
			{
				for (p2List_item<UI_element*>* item2 = current_menu->elements.start; item2; item2 = item2->next)
				{
					if (item2->data->element_type == SLIDER)
					{
						Slider* slider = (Slider*)item2->data;
						switch (slider->modify)
						{
						case MUSIC:
							startValues.music = slider->getProgress();
							break;
						case FX:
							startValues.fx = slider->getProgress();
							break;
						}
					}
				}
			}
			break;
		}
	}
	return ret;
}
void j1UIScene::applySettings(settings_values values)
{
	Uint32 flag = 0;
	if (values.fullscreen)
		flag = SDL_WINDOW_FULLSCREEN;
	SDL_SetWindowFullscreen(App->win->window, flag);

	App->audio->setMusicVolume(values.music);
	App->audio->setFxVolume(values.fx);

	for (p2List_item<UI_element*>* item = current_menu->elements.start; item; item = item->next)
	{
		if (item->data->element_type == SLIDER)
		{
			Slider* slider = (Slider*)item->data;
			switch (slider->modify)
			{
			case MUSIC:
				slider->setProgress(values.music);
				break;
			case FX:
				slider->setProgress(values.fx);
				break;
			}
			slider->button->localPosition.x = ((slider->section.w * App->gui->UI_scale) - 5 - slider->button->section.w / (2 / App->gui->UI_scale)) * slider->progress;
		}
	}
}
menu* j1UIScene::getMenu(menu_id id) const
{
	for (p2List_item<menu*>* item = menus.start; item; item = item->next)
	{
		if (item->data->id == id)
			return item->data;
	}

	LOG("Menu with given id not found");
	return nullptr;
}