#include "j1Player.h"
#include "SDL/include/SDL.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Textures.h"
#include "j1Input.h"
#include "j1App.h"
#include "j1Module.h"
#include "j1Map.h"
#include "j1Collisions.h"

j1Player::~j1Player()
{
	name.create("player");
}

bool j1Player::Awake(pugi::xml_node& config)
{
	bool ret = true;

	gravity = config.child("gravity").attribute("value").as_float();

	position.x = config.child("position").attribute("x").as_float();
	position.y = config.child("position").attribute("y").as_float();

	run_speed.x = config.child("run_speed").attribute("x").as_float();
	run_speed.y = config.child("run_speed").attribute("y").as_float();

	jump_speed.x = config.child("jump_speed").attribute("x").as_float();
	jump_speed.y = config.child("jump_speed").attribute("y").as_float();

	dash_speed.x = config.child("dash_speed").attribute("x").as_float();
	dash_speed.y = config.child("dash_speed").attribute("y").as_float();

	animation_speed = config.child("animation_speed").attribute("value").as_float();

	if (gravity == NULL)
	{
		return ret;
	}
	//SightCollider.x = 0;
	//SightCollider.y = 0;
	//SightCollider.w = 1;
	//SightCollider.h = 1;
	return ret;
}
j1Player::j1Player() : j1Module()
{
	idle_anim.PushBack({ 14, 6, 19, 30 });
	idle_anim.PushBack({ 64, 6, 19, 30 });
	idle_anim.PushBack({ 114, 6, 19, 30 });
	idle_anim.PushBack({ 164, 6, 20, 30 });
	idle_anim.speed = 0.05f;

	run_anim.PushBack({ 67, 45, 20, 28 });
	run_anim.PushBack({ 116, 45, 20, 28 });
	run_anim.PushBack({ 166, 45, 20, 28 });
	run_anim.PushBack({ 217, 45, 23, 28 });
	run_anim.PushBack({ 266, 45, 20, 28 });
	run_anim.PushBack({ 316, 45, 20, 28 });
	run_anim.speed = 0.05f;

	jump_anim.PushBack({ 117, 79, 19, 30 });

	crouch_anim.PushBack({215,14,20,22});
	crouch_anim.PushBack({265,14,20,22});
	crouch_anim.PushBack({315,14,20,22});
	crouch_anim.speed = 0.03f;

	crouchwalk_anim.PushBack({168,351,19,18});
	crouchwalk_anim.PushBack({218,351,19,18});
	crouchwalk_anim.speed = 0.01f;	
	
	fall_anim.PushBack({68,112,18,31});
	fall_anim.PushBack({118,112,18,31});
	fall_anim.speed = 0.03f;

	dash_anim.PushBack({205,571,30,20});
	dash_anim.PushBack({242,571,30,20});
	dash_anim.PushBack({280,571,30,20});
	dash_anim.PushBack({324,571,30,20});
	dash_anim.speed = 0.05f;
}




bool j1Player::Start()
{
	/*if (graphics == nullptr)
	{
		
	}*/
	graphics = App->tex->Load("textures/Player_Spritesheet.png");
	current_anim = &idle_anim;
	return true;
}
bool j1Player::PreUpdate()
{
	if (dead == false)
	{
		if (is_grounded == true)
		{
			if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
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
			if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
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
				if (pState == CROUCHING)
				{
					pState = IDLE;
				}
			}
			if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN)
			{
				if (is_dashing == false)
				{
					if (pState == JUMPING || pState == FALLING)
					{
						if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
						{
							pState = DASHING;
							/*player_velocity = dashDirection(pState);*/
						}
						else if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_UP)
						{
							if (pState == DASHING)
							{
								pState = FALLING;

							}
						}
					}
					else
					{
						pState = JUMPING;
					}
				}
				else if (App->input->GetKey(SDL_SCANCODE_W) == KEY_UP)
				{
					pState = FALLING;
				}
			}
		}
		else // Grounded = false
		{
			pState = FALLING;
		}
	}
	return true;
}



bool j1Player::Update()
{
	switch (pState)
	{
	case IDLE:
		player_velocity.x = 0;
		player_velocity.y = 0;
		current_anim = &idle_anim;
	case RIGHT:
		player_velocity = run_speed;
		current_anim = &run_anim;
	case LEFT:
		player_velocity.x = -run_speed.x;
		current_anim = &run_anim;
	case CROUCHING:
		current_anim = &crouch_anim;
	case JUMPING:
		is_jumping = true;
		player_velocity = jump_speed;
		current_anim = &jump_anim;
		// If player release space in mid jump, the character won't reach max height
		/*if (!double_jump && v.y > (jump_force * 2 / 3) / 2)
		{
			v.y = (jump_force * 2 / 3) / 2;
		}*/
	case FALLING:
		player_velocity.x = jump_speed.x;
		player_velocity.y = gravity;
	}
	return true;
}
bool j1Player::PostUpdate()
{
	App->render->Blit(graphics, position.x, position.y,&current_anim->GetCurrentFrame());
	App->render->DrawQuad(current_anim->GetCurrentFrame(), 255, 0, 0, 0, true);
	/*PositionCameraOnPlayer();*/
	return true;
}


bool j1Player::PositionCameraOnPlayer()
{
	App->render->camera.x = position.x - App->render->camera.w / 3;
	if (App->render->camera.x < 0)
	{
		App->render->camera.x = 0;
		App->render->camera.y = position.y - App->render->camera.h / 2;
	}
	if ((App->render->camera.y + App->win->height) > (App->map->data.height * App->map->data.tile_height))
	{
		App->render->camera.y =App->map->data.height * App->map->data.tile_height - App->win->height;
	}
	return true;
}


//iPoint dashDirection(player_state pState)
//{
//	iPoint newDash;
//	if (pState == RIGHT)
//	{
//		return App->player->dash_speed;
//	}
//	else if (pState == LEFT)
//	{
//		newDash.x = -App->player->dash_speed.x;
//		newDash.y = -App->player->dash_speed.y;
//		return newDash;
//	}
//}