#include "BubbleMain.h"

/* Initialization */
bool BubbleMain::InitAllegro(){

	if(!al_init()) {
		al_show_native_message_box(NULL, "Error", NULL, "failed to initialize allegro!\n", NULL, NULL);
		return false;
	}
	timer = al_create_timer(1.0 / FPS);
	if(!timer) {
		al_show_native_message_box(NULL, "Error", NULL, "failed to create timer!\n", NULL, NULL);
		return false;
	}
	display = al_create_display(VIEW_WINDOW_WIDTH, VIEW_WINDOW_HEIGHT);
	if(!display) {
		al_show_native_message_box(NULL, "Error", NULL, "failed to create display!\n", NULL, NULL);
		al_destroy_timer(timer);
		return false;
	}
	palette = al_create_bitmap(VIEW_WINDOW_WIDTH, VIEW_WINDOW_HEIGHT);
	if(!palette) {
		al_show_native_message_box(NULL, "Error", NULL, "failed to create bouncer bitmap!\n", NULL, NULL);
		al_destroy_display(display);
		al_destroy_timer(timer);
		return false;
	}
	al_set_window_position(display, 0, 0);
	al_install_keyboard();
	al_init_image_addon();
	
	event_queue = al_create_event_queue();
	if(!event_queue) {
		al_show_native_message_box(NULL, "Error", NULL, "failed to create event_queue!\n", NULL, NULL);
		al_destroy_bitmap(palette);
		al_destroy_display(display);
		al_destroy_timer(timer);
		return false;
	}
	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	
	al_clear_to_color(BB_BLACK);
	al_flip_display();

	return true;
}

void BubbleMain::InitGameEngine(){

	Terrain::SingeltonCreate();

	CollisionChecker::SingletonCreate();
	AnimationsParser::SingletonCreate((char*) BubblePathnames::GetAnimationXML().c_str() );
	AnimationFilmHolder::SingletonCreate((char*) BubblePathnames::GetSpritesXML().c_str() );
	
	INIT_NEW_INSTANCE(	MovingAnimation, bubStandanimation, "BubStand",
						BubStandAnimator, bubStandanimator, 
						bubSprite, "BubWalk", 350, 79, true, true);
	/*
	INIT_NEW_INSTANCE(	FrameRangeAnimation, zenChanAnimation2, "ZenChanStand",
						ZenChanStandAnimator, zenChanAnimator2, 
						zenChanSprite2, "ZenChanWalk", 400, 46, true, true);

	INIT_NEW_INSTANCE(	FrameRangeAnimation, zenChanAnimation, "ZenChanStand",
						ZenChanStandAnimator, zenChanAnimator, 
						zenChanSprite, "ZenChanWalk", 450, 79, true, true);
	
	INIT_NEW_INSTANCE(	FrameRangeAnimation, mightaAnimation, "ZenChanStand",
						ZenChanStandAnimator, mightaAnimator, 
						mightaSprite, "ZenChanWalk", 300, 112, true, false);

	INIT_NEW_INSTANCE(	FrameRangeAnimation, baronVonBlubaAnimation, "ZenChanStand",
						ZenChanStandAnimator, baronVonBlubaAnimator, 
						baronVonBlubaSprite, "ZenChanWalk", 250, 79, false, false);*/

	al_start_timer(timer);
	SetGameTime(GetCurrTime());
	
	START_ANIMATOR( bubStandanimator, bubSprite, bubStandanimation, GetGameTime() );/*
	START_ANIMATOR( zenChanAnimator2, zenChanSprite2, zenChanAnimation2, GetGameTime() );
	START_ANIMATOR( zenChanAnimator, zenChanSprite, zenChanAnimation, GetGameTime() );
	START_ANIMATOR( mightaAnimator, mightaSprite, mightaAnimation, GetGameTime() );
	START_ANIMATOR( baronVonBlubaAnimator, baronVonBlubaSprite, baronVonBlubaAnimation, GetGameTime() );*/

	redraw = true;
}


/* Game Loop */
void BubbleMain::ManageGameLoop(){

	using namespace BubbleMain;

	while (1){
		al_wait_for_event(event_queue, &ev);
		al_get_keyboard_state(&keyState);

		if(ev.type == ALLEGRO_EVENT_TIMER) {
			timestamp_t nowTime = GetCurrTime();
			Rendering();
			InputManagement();
			AnimationProgress(nowTime);
			ArtificialIntelligence();
			CollisionChecking();
			CommitDestructions();
			FPSCalculation();
			SystemLoopDispatching();

			SetGameTime(GetGameTime() + ( nowTime - GetGameTime()));
		}
		else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			break;
		}
	}
}

void BubbleMain::Rendering(){

	if(al_is_event_queue_empty(event_queue)) {
		al_set_target_bitmap(palette);
		al_clear_to_color(BB_BLACK);

		Terrain::DisplayTerrain(palette);
		AnimatorHolder::Display(palette);

		al_set_target_bitmap(al_get_backbuffer(display));
		al_draw_bitmap(palette, 0, 0, 0);
		al_flip_display();
	}
}

bool BubbleMain::InputManagement(){
	bool retVal = true;
	bool up=false,right=false,left=false;

	if(al_key_down(&keyState, ALLEGRO_KEY_UP)){
		retVal = InputManageHandling::OnKeyUp();

	}
	if(al_key_down(&keyState, ALLEGRO_KEY_DOWN)){
		std::cout << "pressing Down\n";
	}
	if(al_key_down(&keyState, ALLEGRO_KEY_RIGHT)){
		retVal = InputManageHandling::OnKeyRight();

		std::vector<Animator*> anim = AnimatorHolder::GetAnimators(bubJumpAnimator_t);
		if(anim.size()==1){
			BubJumpAnimator *bja=(BubJumpAnimator *)anim.front();
			std::vector<PathEntry> e=bja->GetAnimation()->GetPath();
			e[bja->GetCurrIndex()].x +=2; 
			bja->GetAnimation()->SetPath(e);
			bja->GetSprite()->SetGoesLeft(false);
		}
		anim = AnimatorHolder::GetAnimators(bubFallingAnimator_t);
		if(anim.size()==1){
			BubFallingAnimator *bja=(BubFallingAnimator *)anim.front();
			bja->GetSprite()->SetGoesLeft(false);
			bja->GetSprite()->Move(2,0);
		}
		
	}
	if(al_key_down(&keyState, ALLEGRO_KEY_LEFT)){
		retVal = InputManageHandling::OnKeyLeft();

		std::vector<Animator*> anim = AnimatorHolder::GetAnimators(bubJumpAnimator_t);
		if(anim.size()==1){
			BubJumpAnimator *bja=(BubJumpAnimator *)anim.front();
			std::vector<PathEntry> e=bja->GetAnimation()->GetPath();
			e[bja->GetCurrIndex()].x +=-2; 
			bja->GetAnimation()->SetPath(e);
			bja->GetSprite()->SetGoesLeft(true);
		}
		anim = AnimatorHolder::GetAnimators(bubFallingAnimator_t);
		if(anim.size()==1){
			BubFallingAnimator *bja=(BubFallingAnimator *)anim.front();
			bja->GetSprite()->SetGoesLeft(true);
			bja->GetSprite()->Move(-2,0);
		}
	}
	if(al_key_down(&keyState, ALLEGRO_KEY_SPACE)){
		retVal = InputManageHandling::OnKeySpace();
	}

	return retVal;
}

void BubbleMain::AnimationProgress(timestamp_t timeNow){
	AnimatorHolder::Progress(timeNow);
	
}

void BubbleMain::ArtificialIntelligence(){
}

void BubbleMain::CollisionChecking(){
	CollisionChecker::Check();
}

void BubbleMain::CommitDestructions(){
	DestructionManager::Commit();
}

void BubbleMain::FPSCalculation(){
}

void BubbleMain::SystemLoopDispatching(){
}

/* Game Termination */
void BubbleMain::GameOver(){

	Terrain::SingeltonCleanUp();

	CollisionChecker::SingletonCleanUp();
	AnimationsParser::SingletonDestroy();
	AnimationFilmHolder::SingletonDestroy();	

	al_destroy_bitmap(palette);
	al_destroy_timer(timer);
	al_destroy_display(display);
	al_destroy_event_queue(event_queue);
}


int main(int argc, char **argv){
	using namespace BubbleMain;

	if (InitAllegro() ){
		InitGameEngine();
		ManageGameLoop();
		GameOver();
	}
	system( "pause" );
}