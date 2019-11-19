#include "j1Player.h"
#include "SDL/include/SDL.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Textures.h"
#include "j1Input.h"
#include "j1App.h"
#include "j1Audio.h"
#include "j1Module.h"
#include "j1Map.h"
#include "j1Collisions.h"
#include "j1FadeToBlack.h"
#include "j1Scene.h"

#include "Brofiler/Brofiler.h"
//
//
//
//Checkpoints, "dolphin dive"
//
//

j1Player::j1Player() : j1Module()
{
	name.create("player");

	god_anim.PushBack({ 350, 6, 19, 30 });


	idle_anim.PushBack({ 14, 6, 19, 30 });
	idle_anim.PushBack({ 64, 6, 19, 30 });
	idle_anim.PushBack({ 114, 6, 19, 30 });
	idle_anim.PushBack({ 164, 6, 20, 30 });
	idle_anim.speed = 0.2f;

	run_anim.PushBack({ 67, 45, 20, 28 });
	run_anim.PushBack({ 116, 45, 20, 28 });
	run_anim.PushBack({ 166, 45, 20, 28 });
	run_anim.PushBack({ 217, 45, 23, 28 });
	run_anim.PushBack({ 266, 45, 20, 28 });
	run_anim.PushBack({ 316, 45, 20, 28 });
	run_anim.speed = 0.2f;

	jump_anim.PushBack({ 117, 79, 19, 30 });

	crouch_anim.PushBack({ 215,6,20,30 });
	crouch_anim.PushBack({ 265,6,20,30 });
	crouch_anim.PushBack({ 315,6,20,30 });
	crouch_anim.speed = 0.03f;

	crouchwalk_anim.PushBack({ 168,351,19,18 });
	crouchwalk_anim.PushBack({ 218,351,19,18 });
	crouchwalk_anim.speed = 0.01f;

	fall_anim.PushBack({ 68,112,18,31 });
	fall_anim.PushBack({ 118,112,18,31 });
	fall_anim.speed = 0.1f;

	dash_anim.PushBack({ 205,571,30,20 });
	dash_anim.PushBack({ 242,571,30,20 });
	dash_anim.PushBack({ 280,571,30,20 });
	dash_anim.PushBack({ 324,571,30,20 });
	dash_anim.speed = 0.13f;
	dash_anim.lock = true;
}
j1Player::~j1Player()
{
}

bool j1Player::Save(pugi::xml_node& node) const
{
	pugi::xml_node coordinates = node.append_child("coordinates");

	coordinates.append_child("position").append_attribute("x") = position.x;
	coordinates.child("position").append_attribute("y") = position.y;

	node.append_attribute("player_state") = pState;

	return true;
}

bool j1Player::Load(pugi::xml_node& node)
{
	pugi::xml_node coordinates = node.child("coordinates");

	position.x = coordinates.child("position").attribute("x").as_float();
	position.y = coordinates.child("position").attribute("y").as_float();

	pugi::xml_node state = node.child("state");

	pState = (player_state)node.attribute("player_state").as_int();
	return true;
}

bool j1Player::Awake(pugi::xml_node& config)
{
	bool ret = true;

	gravity = config.child("gravity").attribute("value").as_float();

	position.x = config.child("position").attribute("x").as_float();
	position.y = config.child("position").attribute("y").as_float();

	initial_position.x = config.child("position").attribute("x").as_float();
	initial_position.y = config.child("position").attribute("y").as_float();

	run_speed.x = config.child("run_speed").attribute("x").as_float();
	run_speed.y = config.child("run_speed").attribute("y").as_float();

	jump_force = config.child("jump_force").attribute("value").as_float();

	max_speed.x = config.child("max_speed").attribute("x").as_float();
	max_speed.y = config.child("max_speed").attribute("y").as_float();
	dash_acceleration = config.child("dash_acceleration").attribute("value").as_float();

	is_grounded = config.child("is_grounded").attribute("value").as_bool();
	is_dead = config.child("is_dead").attribute("value").as_bool();
	is_dashing = config.child("is_dashing").attribute("value").as_bool();

	jump_SFX = config.child("jump_SFX").attribute("source").as_string();
	dash_SFX = config.child("dash_SFX").attribute("source").as_string();
	run_SFX = config.child("run_SFX").attribute("source").as_string();

	spritesheet = config.child("spritesheet").attribute("source").as_string();

	return ret;
}




bool j1Player::Start()
{
	graphics = App->tex->Load(spritesheet.GetString());
	player_col = App->collisions->AddCollider({ (int)position.x, (int)position.y, 30, 47}, COLLIDER_PLAYER, this);
	is_dead = false;
	is_grounded = false;
	is_jumping == false;
	flip = false;
	position = initial_position;
	App->audio->LoadFx(jump_SFX.GetString());
	App->audio->LoadFx(dash_SFX.GetString());
	App->audio->LoadFx(run_SFX.GetString());

	return true;
}

bool j1Player::PreUpdate()
{
	if (App->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
	{
		god_mode = !god_mode;
	}
	if (is_dead == false && god_mode == false)
	{
		
		if (is_grounded  && !is_jumping && !is_dashing)
		{
			pState = IDLE;
			if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && is_dashing == false)
			{
				pState = RIGHT;

			}
			else if (App->input->GetKey(SDL_SCANCODE_D) == KEY_UP)
			{
				if (pState == RIGHT)
				{
					pState = IDLE;
				}
			}
			if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && is_dashing == false)
			{
				pState = LEFT;
			}
			else if (App->input->GetKey(SDL_SCANCODE_A) == KEY_UP)
			{
				if (pState == LEFT)
				{
					pState = IDLE;
				}
			}
			if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
			{
				pState = CROUCHING;
			}
			else if (App->input->GetKey(SDL_SCANCODE_S) == KEY_UP)
			{
				able_superjump = false;
				pState = IDLE;
			}
			if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
			{
				App->audio->PlayFx(App->audio->LoadFx(jump_SFX.GetString()));
				if (able_superjump == true)
				{
					player_velocity.y = jump_force * 1.5f;
					able_superjump = false;
				}
				else
				{
					player_velocity.y = jump_force;
				}
				pState = JUMPING;
			}
		}	

		if (is_jumping == false && is_grounded == false)
		{
			pState = FALLING;		
		}


		if (is_dashing == false)
		{
			if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN)
			{
				App->audio->PlayFx(App->audio->LoadFx(dash_SFX.GetString()));
				pState = DASHING;
			}
		}
	}
	return true;
}



bool j1Player::Update(float dt)
{
	
	
	if (!is_dashing && !is_jumping)
		is_grounded = false;

	switch (pState)
	{
	case IDLE:
		if (!is_dashing)
		{
			player_velocity.x = 0;
			player_velocity.y = 0;
		}
		current_anim = &idle_anim;
		break;
	case RIGHT:
		player_velocity.x = run_speed.x;
		current_anim = &run_anim;
		break;
	case LEFT:
		player_velocity.x = -run_speed.x;
		current_anim = &run_anim;
		break;
	case CROUCHING:
		player_velocity.x = 0;
		current_anim = &crouch_anim;
		able_superjump = true;
		break;
	case DASHING:
		is_dashing = true;
		current_anim = &dash_anim;
		break;
	case JUMPING:		
		is_jumping = true;
		is_grounded = false;
		break;
	case STOP_JUMPING:
		is_jumping = false;
	case FALLING:
		
		if(player_velocity.y < max_speed.y)
			player_velocity.y += gravity;

		current_anim = &fall_anim;

		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
			player_velocity.x = run_speed.x * 0.8f;

		else if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
			player_velocity.x = -run_speed.x * 0.8f;

		if (position.y > App->win->height + player_col->rect.h)
			is_dead = true;
		break;
	}

	if(player_velocity.x < 0)
		flip = true;
	else if (player_velocity.x > 0)
		flip = false;

	jumpMovement(); 
	Dash_Movement();
	Load_Level();
	God_Mode();
	// Limit of the screen left border
	if (position.x < App->render->camera.x && flip) 
	{
		player_velocity.x = 0;
	}
	//Don't move in the y axis while dashing
	if (!is_dashing && !is_dead)
	{
		position.y += player_velocity.y;
	}
	// Death behaviour
	else if (is_dead)
	{
		position.y -= max_speed.y;
		current_anim = &god_anim;
	}
	position.x += player_velocity.x;
	player_col->SetPos(position.x, position.y);
	return true;
}

bool j1Player::PostUpdate()
{
	BROFILER_CATEGORY("Player PostUpdate", Profiler::Color::Red)
	
	if (graphics != nullptr)
	{
		if (flip)
		{
			App->render->Blit(graphics, position.x, position.y, &current_anim->GetCurrentFrame(), SDL_FLIP_HORIZONTAL, true);
		}
		else
		{
			App->render->Blit(graphics, position.x, position.y, &current_anim->GetCurrentFrame(), SDL_FLIP_NONE, true);
		}
		PositionCameraOnPlayer();
	}
	
	return true;
}

bool j1Player::CleanUp()
{
	App->tex->UnLoad(graphics);
	graphics = nullptr;

	App->collisions->EraseCollider(player_col);

	is_grounded = false;
	is_jumping == false;
	
	return true;
}

void j1Player::Player_Colliding(Collider* C1, Collider* C2)
{
	if (C1->type == COLLIDER_PLAYER && C2->type == COLLIDER_UNPENETRABLE && god_mode == false)
	{
		if (C1->rect.x + C1->rect.w >= C2->rect.x  && C1->rect.x  <= C2->rect.x + C2->rect.w)
		{
			//Collision from top
			if ((C1->rect.y + C1->rect.h) >= (C2->rect.y) && (C1->rect.y < C2->rect.y))
			{
				player_velocity.y = 0;
				position.y = C2->rect.y - C1->rect.h + 1;
				is_grounded = true;

			}
			//Collision from below
			else if (C1->rect.y < (C2->rect.y + C2->rect.h) && (C1->rect.y + C1->rect.h)>(C2->rect.y + C2->rect.h))
			{
				player_velocity.y = 0;
				App->player->position.y = C2->rect.y + C2->rect.h;
			}
		}
		
		if (C1->rect.y < C2->rect.y + C2->rect.h && C1->rect.y + C1->rect.h  > C2->rect.y && C1->rect.y > C2->rect.y)
		{
			//Collision from the right
			if ((C1->rect.x + C1->rect.w ) > (C2->rect.x) && C1->rect.x < (C2->rect.x))
			{
				App->player->position.x = C2->rect.x - C1->rect.w - 1;
				player_velocity.x = 0;

			}
			//Collision from the left
			else if (C1->rect.x  < (C2->rect.x + C2->rect.w) && C1->rect.x > (C2->rect.x))
			{
				App->player->position.x = C2->rect.x + C2->rect.w + 1;
				player_velocity.x = 0;

			}
		}
	}
	else if (C1->type == COLLIDER_PLAYER && C2->type == COLLIDER_PENETRABLE && god_mode == false)
	{
		//Collision from top and below
		if ((C1->rect.y + C1->rect.h  ) > (C2->rect.y) && C1->rect.y < C2->rect.y && C1->rect.y + C1->rect.h - 8 < C2->rect.y && player_velocity.y >= 0)
		{
				App->player->position.y = C2->rect.y - C1->rect.h + 1;
				player_velocity.y = 0;
				is_grounded = true;

		}
	}
	//Collision against spikes
	else if (C1->type == COLLIDER_PLAYER && C2->type == COLLIDER_SPIKES && god_mode == false)
	{
		is_dead = true;

	}
	//Win condition
	else if (C1->type == COLLIDER_PLAYER && C2->type == COLLIDER_WIN)
	{
		
		if (App->map->data.map_name == "Level1.tmx")
			App->fade_to_black->FadeToBlack("Level2.tmx", 3.0f);
		if (App->map->data.map_name == "Level2.tmx")
			App->scene->ret = false;
			
	}

}



// Keep the camera focused on the player
bool j1Player::PositionCameraOnPlayer()
{
	App->render->camera.x = -position.x + App->render->camera.w / 3;
	if (App->render->camera.x >= 0)
	{
		App->render->camera.x = 0;
	}
	if (App->render->camera.x <= -2400)
	{
		App->render->camera.x = -2400;
	}

	return true;
}

void j1Player::jumpMovement()
{
	if (is_jumping == true)
	{
		if (player_velocity.y < 0)
		{
			player_velocity.y += gravity;
			current_anim = &jump_anim;
		}
		else
		{
			is_jumping = false;
		}
		
		if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_UP)
		{
			pState = STOP_JUMPING;
		}		
	}
	
}

void j1Player::Dash_Movement()
{
	if (is_dashing == true)
	{
		if (flip == false)//Right Dash
		{
			if (player_velocity.x > max_speed.x)
			{
				player_velocity.x = player_velocity.x / 6;
				is_dashing = false;

			}
			else
			{
				player_velocity.x += dash_acceleration;
				current_anim = &dash_anim;
			}
		}
		else//Left Dash
		{
			if (player_velocity.x < -max_speed.x)
			{
				player_velocity.x = player_velocity.x / 6;
				is_dashing = false;
			}
			else
			{
				player_velocity.x -= dash_acceleration;
				current_anim = &dash_anim;
			}
		}		
	}
	
}

void j1Player :: God_Mode()
{
	if (is_dead == false && god_mode == true)
	{
		current_anim = &god_anim;
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		{
			player_velocity.x = run_speed.x;
		}
		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		{
			player_velocity.x = -run_speed.x;
		}
		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
		{
			player_velocity.y = -run_speed.x;
		}
		if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
		{
			player_velocity.y = run_speed.x;
		}
	}	
}
void j1Player::Load_Level()
{
	if (is_dead == true)
	{
		player_velocity.y = 0;
		player_velocity.x = 0;
		if (App->map->data.map_name == "Level1.tmx")
			App->fade_to_black->FadeToBlack("Level1.tmx", 3.0f);
		if (App->map->data.map_name == "Level2.tmx")
			App->fade_to_black->FadeToBlack("Level2.tmx", 3.0f);
	}
}