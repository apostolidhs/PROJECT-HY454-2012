#include "FruitsAnimator.h"
#include "GameActionUtilities.h"
#include "AnimatorHolder.h"
#include "CollisionChecker.h"
#include "BubbleLogic.h"
#include "AnimationsParser.h"
#include "Terrain.h"
#include "AnimationFilmHolder.h"


typedef void (*FruitSelector_t)(int x, int y);

static void CreateBananaAnimator(int x, int y){

	MovingAnimation* mva=(MovingAnimation*)AnimationsParser::GetAnimation("Banana");
	Sprite *n_sprite=new Sprite(x,y,true,AnimationFilmHolder::GetFilm("Banana"), Terrain::GetActionLayer(), true);
	
	BananaAnimator *ban=new BananaAnimator();
	ban->RegistCollitions(n_sprite);
	
	START_ANIMATOR( ban, n_sprite, mva, GetGameTime());
}

static void CreateOrangeAnimator(int x, int y){

	MovingAnimation* mva=(MovingAnimation*)AnimationsParser::GetAnimation("Orange");
	Sprite *n_sprite=new Sprite(x,y,true,AnimationFilmHolder::GetFilm("Orange"), Terrain::GetActionLayer(), true);
	
	OrangeAnimator *ban=new OrangeAnimator();
	ban->RegistCollitions(n_sprite);
	
	START_ANIMATOR( ban, n_sprite, mva, GetGameTime());
}

static void CreatePeachAnimator(int x, int y){

	MovingAnimation* mva=(MovingAnimation*)AnimationsParser::GetAnimation("Peach");
	Sprite *n_sprite=new Sprite(x,y,true,AnimationFilmHolder::GetFilm("Peach"), Terrain::GetActionLayer(), true);
	
	PeachAnimator *ban=new PeachAnimator();
	ban->RegistCollitions(n_sprite);
	
	START_ANIMATOR( ban, n_sprite, mva, GetGameTime());
}

static void CreateWaterMelonAnimator(int x, int y){

	MovingAnimation* mva=(MovingAnimation*)AnimationsParser::GetAnimation("WaterMelon");
	Sprite *n_sprite=new Sprite(x,y,true,AnimationFilmHolder::GetFilm("WaterMelon"), Terrain::GetActionLayer(), true);
	
	WaterMelonAnimator *ban=new WaterMelonAnimator();
	ban->RegistCollitions(n_sprite);
	
	START_ANIMATOR( ban, n_sprite, mva, GetGameTime());
}

static FruitSelector_t fruitSelector[]={
	0,
	CreateBananaAnimator,
	CreateOrangeAnimator,
	CreatePeachAnimator,
	CreateWaterMelonAnimator
};

void FruitsAnimators::StartFruitAnimator(int kind, int x, int y){
	fruitSelector[kind](x, y);
}

//////////////BananaAnimator
BananaAnimator::BananaAnimator(){
	this->SetOnFinish(OnFinishCallback,(void*)this);
}

void BananaAnimator::RegistCollitions(Sprite* sprite){
	CollisionChecker::Register(sprite,bubStandAnimator_t,bubJumpAnimator_t, this, BananaAnimator::OnCollisionWithBub);
}

void BananaAnimator::OnCollisionWithBub(Sprite *Bub, Sprite *Fruit, void *args){
	DASSERT( Bub && Fruit && args);
	BananaAnimator * _this = (BananaAnimator*)args;
	REMOVE_FROM_ACTION_ANIMATOR(_this);

	DESTROY_ANIMATOR( _this );
	BubbleLogic::BubBananaAcquired();
}

void BananaAnimator::OnFinishCallback(Animator* anim, void* args){
	DASSERT( anim && args);
	BananaAnimator * _this = (BananaAnimator*)args;
	DASSERT( _this->GetAnimation() && _this->GetSprite() && anim==_this );
	REMOVE_FROM_ACTION_ANIMATOR(_this);
	
	DESTROY_ANIMATOR( _this );
}


//////////////OrangeAnimator
OrangeAnimator::OrangeAnimator(){
	this->SetOnFinish(OnFinishCallback,(void*)this);
}

void OrangeAnimator::RegistCollitions(Sprite* sprite){
	CollisionChecker::Register(sprite,bubStandAnimator_t,bubJumpAnimator_t, this, OrangeAnimator::OnCollisionWithBub);
}

void OrangeAnimator::OnCollisionWithBub(Sprite *Bub, Sprite *Fruit, void *args){
	DASSERT( Bub && Fruit && args);
	OrangeAnimator * _this = (OrangeAnimator*)args;
	REMOVE_FROM_ACTION_ANIMATOR(_this);

	DESTROY_ANIMATOR( _this );
	BubbleLogic::BubOrangeAcquired();
}

void OrangeAnimator::OnFinishCallback(Animator* anim, void* args){
	DASSERT( anim && args);
	OrangeAnimator * _this = (OrangeAnimator*)args;
	DASSERT( _this->GetAnimation() && _this->GetSprite() && anim==_this );
	REMOVE_FROM_ACTION_ANIMATOR(_this);
	
	DESTROY_ANIMATOR( _this );
}

/////////////PeachAnimator
PeachAnimator::PeachAnimator(){
	this->SetOnFinish(OnFinishCallback,(void*)this);
}

void PeachAnimator::RegistCollitions(Sprite* sprite){
	CollisionChecker::Register(sprite,bubStandAnimator_t,bubJumpAnimator_t, this, PeachAnimator::OnCollisionWithBub);
}

void PeachAnimator::OnCollisionWithBub(Sprite *Bub, Sprite *Fruit, void *args){
	DASSERT( Bub && Fruit && args);
	PeachAnimator * _this = (PeachAnimator*)args;
	REMOVE_FROM_ACTION_ANIMATOR(_this);

	DESTROY_ANIMATOR( _this );
	BubbleLogic::BubPeachAcquired();
}

void PeachAnimator::OnFinishCallback(Animator* anim, void* args){
	DASSERT( anim && args);
	PeachAnimator * _this = (PeachAnimator*)args;
	DASSERT( _this->GetAnimation() && _this->GetSprite() && anim==_this );
	REMOVE_FROM_ACTION_ANIMATOR(_this);
	
	DESTROY_ANIMATOR( _this );
}

/////////////WaterMelonAnimator
WaterMelonAnimator::WaterMelonAnimator(){
	this->SetOnFinish(OnFinishCallback,(void*)this);
}

void WaterMelonAnimator::RegistCollitions(Sprite* sprite){
	CollisionChecker::Register(sprite,bubStandAnimator_t,bubJumpAnimator_t, this, WaterMelonAnimator::OnCollisionWithBub);
}

void WaterMelonAnimator::OnCollisionWithBub(Sprite *Bub, Sprite *Fruit, void *args){
	DASSERT( Bub && Fruit && args);
	WaterMelonAnimator * _this = (WaterMelonAnimator*)args;
	REMOVE_FROM_ACTION_ANIMATOR(_this);

	DESTROY_ANIMATOR( _this );
	BubbleLogic::BubWaterMelonAcquired();
}

void WaterMelonAnimator::OnFinishCallback(Animator* anim, void* args){
	DASSERT( anim && args);
	WaterMelonAnimator * _this = (WaterMelonAnimator*)args;
	DASSERT( _this->GetAnimation() && _this->GetSprite() && anim==_this );
	REMOVE_FROM_ACTION_ANIMATOR(_this);
	
	DESTROY_ANIMATOR( _this );
}

////////////BlueDiamondAnimator
BlueDiamondAnimator::BlueDiamondAnimator(){
	this->SetOnFinish(OnFinishCallback,(void*)this);
}

void BlueDiamondAnimator::RegistCollitions(Sprite* sprite){
	CollisionChecker::Register(sprite,bubStandAnimator_t,bubJumpAnimator_t, this, BlueDiamondAnimator::OnCollisionWithBub);
}

void BlueDiamondAnimator::OnCollisionWithBub(Sprite *Bub, Sprite *Fruit, void *args){
	DASSERT( Bub && Fruit && args);
	BlueDiamondAnimator * _this = (BlueDiamondAnimator*)args;
	REMOVE_FROM_ACTION_ANIMATOR(_this);

	DESTROY_ANIMATOR( _this );
	BubbleLogic::BubBlueDiamondAcquired();
}

void BlueDiamondAnimator::OnFinishCallback(Animator* anim, void* args){
	DASSERT( anim && args);
	BlueDiamondAnimator * _this = (BlueDiamondAnimator*)args;
	DASSERT( _this->GetAnimation() && _this->GetSprite() && anim==_this );
	REMOVE_FROM_ACTION_ANIMATOR(_this);
	
	DESTROY_ANIMATOR( _this );
}