#pragma once
#include "Main.h"
#include <Windows.h>

//
struct CVars		//101xd
{
	/*
	cvar_s *knifebot;
	cvar_s *knifebot_aim;
	cvar_s *knifebot_aim_fov;
	cvar_s *knifebot_aim_wp;
	cvar_s *knifebot_team;
	cvar_s *knifebot_attack;
	cvar_s *knifebot_dist_attack;
	cvar_s *knifebot_dist_attack2;
	cvar_s *knifebot_fps;
	cvar_s *knifebot_fps_time;*/

	cvar_s* lj_stats_log;


	/*
	cvar_s *wallhack_xqz;
	cvar_s *whitewalls;*/

	cvar_s* bhop;					//bunu yap
	cvar_s* bhop_nsd;				//bunu yap
	cvar_s* bhop_cnt;				
	cvar_s* bhop_autoduck;			//bunu yap
	cvar_s* bhop_cnt_rand;
	cvar_s* bhop_cnt_rand_min;
	cvar_s* bhop_cnt_rand_max;


	cvar_s* cam;
	cvar_s* cam_up;
	cvar_s* cam_side;
	cvar_s* cam_back;




	/*cvar_s *waybot_speed;
	cvar_s *waybot_color;*/
	cvar_s* waybot_speed_fps;
	cvar_s* waybot_speed_fps_file;




	cvar_s* show;
	cvar_s* show_fps;
	cvar_s* show_time;
	cvar_s* show_keys;
	cvar_s* show_color;
	cvar_s* show_info;
	cvar_s* show_info_main;
	cvar_s* show_info_strafe;
	cvar_s* show_info_helper;
	cvar_s* show_info_waybot;

	cvar_s* show_kz;
	cvar_s* show_kz_edge;
	cvar_s* show_kz_speed;
	cvar_s* show_kz_height;
	cvar_s* show_kz_damage;
	cvar_s* show_kz_ljstats;
	cvar_s* show_kz_fallspeed;

	cvar_s* show_kz_color;

	cvar_s* show_glow_model;
	cvar_s* show_glow_model_color;
	cvar_s* show_glow_model_ticknes;
	cvar_s* show_glow_model_team;

	cvar_s* show_glow_weapon;
	cvar_s* show_glow_weapon_color;
	cvar_s* show_glow_weapon_tickness;



	cvar_s* freelook_speed;
	cvar_s* fps_slowto;
	cvar_s* fps_helper;
	cvar_s* fixedyaw;
	cvar_s* fixedpitch;

	cvar_s* speed;
	cvar_s* speed_net;

	cvar_s* strafe_aa;

	cvar_s* strafe_fps;
	cvar_s* strafe_temp;
	cvar_s* strafe_dir;
	cvar_s* strafe_dir_auto;
	cvar_s* strafe_invisible;

	cvar_s* strafe_speed;
	cvar_s* strafe_crazy;
	cvar_s* strafe_helper;
	cvar_s* strafe_helper_add_strafe;
	cvar_s* strafe_helper_max_strafe;
	cvar_s* strafe_helper_max_strafe_rand;
	cvar_s* strafe_helper_max_strafe_rand_min;
	cvar_s* strafe_helper_max_strafe_rand_max;
	cvar_s* strafe_helper_boost;
	cvar_s* strafe_helper_move;
	cvar_s* strafe_helper_move_rand;
	cvar_s* strafe_helper_move_rand_min;
	cvar_s* strafe_helper_move_rand_max;
	cvar_s* strafe_helper_main;

	cvar_s* gstrafe_bhop;
	cvar_s* gstrafe_standup;
	cvar_s* gstrafe_noslowdown;



	//cvar_s *duck;

	cvar_s* jb_auto;

	cvar_s* debug_sound;

	cvar_s* SpinHack;
	cvar_s* SpinHackSpeed;
	///////////////////////////////////////////////////////

	cvar_s* myspeed; //benim cvar
	cvar_s* strafehack;
	//
	cvar_s* myaimbot;
	cvar_s* aimbotfov;
	cvar_s* aimbotsmooth;
	//
	cvar_s* name_stealer;
    //			fakelag
	cvar_s*	fakelag_enabled;
	cvar_s*	fakelag_type;
	cvar_s*	fakelag_choke_limit;
	//
	cvar_s* thirdperson;
	cvar_s* fov_value;
	//
	
};

//
extern CVars cvar;



struct CFunc
{
public:
	void Init();

	void SpeedEngine(float speed, float frametime);
	static bool forwardm;
	static bool moveleft;
	static bool moveright;
	static bool back;
	static bool duck;
	static bool jump;

	static void fps_on();
	static void fps_off();

	void p_for();
	void m_for();

	void p_ml();
	void m_ml();

	void p_mr();
	void m_mr();

	void p_bk();
	void m_bk();

	void p_dk();
	void m_dk();

	void p_jp();
	void m_jp();
};
extern CFunc cfunc;