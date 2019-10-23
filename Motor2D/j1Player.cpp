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

j1Player::j1Player() : j1Module()
{
	name.create("player");

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

	crouch_anim.PushBack({ 215,6,20,30 });
	crouch_anim.PushBack({ 265,6,20,30 });
	crouch_anim.PushBack({ 315,6,20,30 });
	crouch_anim.speed = 0.03f;

	crouchwalk_anim.PushBack({ 168,351,19,18 });
	crouchwalk_anim.PushBack({ 218,351,19,18 });
	crouchwalk_anim.speed = 0.01f;

	fall_anim.PushBack({ 68,112,18,31 });
	fall_anim.PushBack({ 118,112,18,31 });
	fall_anim.speed = 0.03f;

	dash_anim.PushBack({ 205,571,30,20 });
	dash_anim.PushBack({ 242,571,30,20 });
	dash_anim.PushBack({ 280,571,30,20 });
	dash_anim.PushBack({ 324,571,30,20 });
	dash_anim.speed = 0.05f;
	dash_anim.lock = true;
}
j1Player::~j1Player()
{
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

	return ret;
}




bool j1Player::Start()
{
	graphics = App->tex->Load("textures/Player_Spritesheet.png");
	dead = false;
	is_grounded = true;
	is_dashing = false;
	current_anim = &idle_anim;

	player_col = App->collisions->AddCollider({ (int)position.x, (int)position.y, 18, 27 }, COLLIDER_PLAYER, this);

	return true;
}

bool j1Player::PreUpdate()
{
	pState = IDLE;
	if (dead == false)
	{
		if (is_grounded == true)
		{
			if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
			{
				pState = RIGHT;

			}
			else if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
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
				pState = JUMPING;
			}
		}		
		else // Grounded = false
		{
			pState = FALLING;
		}

		if (is_dashing == false)
		{
			if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
			{
				pState = DASHING;
			}
		}
	}
	return true;
}



bool j1Player::Update(float dt)
{
	player_col->SetPos(position.x, position.y);

	switch (pState)
	{
	case IDLE:
		player_velocity.x = 0;
		player_velocity.y = 0;
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
		current_anim = &crouch_anim;
		player_col->rect.h = 20;
		break;
	case DASHING:
		player_velocity.x += dash_speed.x;
		current_anim = &dash_anim;
		break;
	case JUMPING:
		is_jumping = true;
		player_velocity = jump_speed;
		current_anim = &jump_anim;
		break;
		// If player release space in mid jump, the character won't reach max height
		/*if (!double_jump && v.y > (jump_force * 2 / 3) / 2)
		{
			v.y = (jump_force * 2 / 3) / 2;
		}*/
	case FALLING:
		position.y += gravity;
		break;
	}
	position.x += player_velocity.x;
	position.y += player_velocity.y;
	return true;
}

bool j1Player::PostUpdate()
{
	App->render->Blit(graphics, position.x, position.y,&current_anim->GetCurrentFrame());
	PositionCameraOnPlayer();
	return true;
}


bool j1Player::PositionCameraOnPlayer()
{
	App->render->camera.x = -position.x + App->render->camera.w / 3;
	App->render->camera.y = -position.y + App->render->camera.h;

	if (App->render->camera.x >= 0)
	{
		App->render->camera.x = 0;
	}
	if (App->render->camera.y >= 0)
	{
		App->render->camera.y = 0;
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