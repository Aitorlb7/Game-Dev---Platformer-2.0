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

	jump_force = config.child("jump_force").attribute("value").as_float();

	dash_speed.x = config.child("dash_speed").attribute("x").as_float();
	dash_speed.y = config.child("dash_speed").attribute("y").as_float();

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
	current_anim = &idle_anim;
	player_col = App->collisions->AddCollider({ (int)position.x, (int)position.y, 20, 35}, COLLIDER_PLAYER, this);
	is_dead = false;
	is_grounded = false;
	is_jumping == false;
	position = { 100,300 };

	App->audio->LoadFx(jump_SFX.GetString());
	App->audio->LoadFx(dash_SFX.GetString());
	App->audio->LoadFx(run_SFX.GetString());

	return true;
}

bool j1Player::PreUpdate()
{
	if (is_dead == false)
	{
		if (is_grounded == true && is_jumping == false)
		{
			pState = IDLE;
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
				able_superjump = false;
				pState = IDLE;
			}
			if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN)
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
			if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
			{
				player_velocity.x = run_speed.x * 0.5f;
			}

			if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
			{
				player_velocity.x = -run_speed.x * 0.5f;
			}
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
	before_colliding = position;
	switch (pState)
	{
	case IDLE:
		if (is_dashing == false)
		{
			player_velocity.x = 0;
			player_velocity.y = 0;
		}
		player_col->rect.h = 40;
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
		player_col->SetPos(position.x, position.y + 20);
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
		//player_velocity.y = 0;
		is_jumping == false;
	case FALLING:
		player_velocity.y += gravity;
		current_anim = &fall_anim;
		break;
	}
	if(player_velocity.x < 0)
	{
		flip = true;
	}
	else if (player_velocity.x > 0)
	{
		flip = false;
	}

	jumpMovement(); 
	Dash_Movement();
	position.x += player_velocity.x;
	position.y += player_velocity.y;
	return true;
}

bool j1Player::PostUpdate()
{
	if(flip)
	{
		App->render->Blit(graphics, position.x, position.y,&current_anim->GetCurrentFrame(), SDL_FLIP_HORIZONTAL);
	}
	else
	{
		App->render->Blit(graphics, position.x, position.y, &current_anim->GetCurrentFrame(), SDL_FLIP_NONE);
	}
	PositionCameraOnPlayer();
	return true;
}

bool j1Player::CleanUp()
{
	/*App->tex->UnLoad(graphics);
	graphics = nullptr;
	player_col->to_delete;
	*/
	
	return true;
}

void j1Player::Player_Colliding(Collider* C1, Collider* C2)
{
	if (C2->type == COLLIDER_TYPE::COLLIDER_PLAYER)
	{
		return;
	}
	
	if (C1->type == COLLIDER_PLAYER && C2->type == COLLIDER_UNPENETRABLE)
	{
		//Collision from below
		if (C2->rect.y + C2->rect.h - App->player->before_colliding.y < 0) //need to review -- Purpouse: Dont act when it collides from top
		{
			if (App->player->before_colliding.y < (C2->rect.y + C2->rect.h))
			{
				App->player->position.y = C2->rect.y + C2->rect.h;
			}
		}

		//Collision from top
		//else if ((App->player->before_colliding.y + App->player->player_col->rect.y) > (C2->rect.y))
		else if ((App->player->before_colliding.y + App->player->player_col->rect.h) > (C2->rect.y))
		{
			/*App->player->position.y = C2->rect.y - App->player->player_col->rect.h + 1;*/ // LA HE COMENTADO PARA QUE VAYA EL JUMP (NO SÉ SI HACE ALGO ÚTIL)
			if (player_velocity.y > 0)
			{
				player_velocity.y = 0;
			}
			is_grounded = true;

		}
		//Collision from the right
		else if (App->player->position.y + (App->player->player_col->rect.h * 3.0f / 4.0f) > C2->rect.y + C2->rect.h
			&& App->player->position.y + (App->player->player_col->rect.h * 3.0f / 4.0f) < C2->rect.y)
		{
			if ((App->player->player_col->rect.x + App->player->player_col->rect.w) < (C2->rect.x + C2->rect.w / 2))
			{
				App->player->position.x = C2->rect.x - App->player->player_col->rect.w;
			}
			//Collision from the left
			else if (App->player->player_col->rect.x < (C2->rect.x + C2->rect.w))
			{
				App->player->position.x = C2->rect.x + C2->rect.w - 20;
			}
		}
	}
	else if (C1->type == COLLIDER_PLAYER && C2->type == COLLIDER_PENETRABLE)
	{
		//Collision from below
		if (App->player->before_colliding.y > (C2->rect.y - 1))
		{
			App->player->position.y = C2->rect.y;

		}
		//Collision from top
		else if ((App->player->before_colliding.y + App->player->player_col->rect.y) < (C2->rect.y))
		{
			App->player->position.y = C2->rect.y;
			if (player_velocity.y > 0)
			{
				player_velocity.y = 0;
			}
			is_grounded = true;

		}
		////Collision from the right
		//if ((App->player->player_col->rect.x + App->player->player_col->rect.w) < (C2->rect.x + C2->rect.w / 2))
		//{
		//	App->player->position.x = C2->rect.x - App->player->player_col->rect.w;
		//}
		////Collision from the left
		//else if (App->player->player_col->rect.x < (C2->rect.x + C2->rect.w))
		//{
		//	App->player->position.x = C2->rect.x + C2->rect.w - 20;
		//}
	}

}



// Keep the camera focused on the player
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
		
		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_UP)
		{
			pState = STOP_JUMPING;
		}		
	}
	
}

void j1Player::Dash_Movement()
{
	if (is_dashing == true)
	{
		if (player_velocity.x > dash_speed.x)
		{
			player_velocity.x = 0;
			is_dashing = false;
		}
		else
		{
			player_velocity.x += 0.2f;
			current_anim = &dash_anim;
		}
	}
	
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