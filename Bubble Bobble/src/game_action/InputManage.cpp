#include "InputManage.h"
#include "GameActionUtilities.h"
#include "vector"
#include "CollisionChecker.h"
#include "Animator.h"
#include "AnimatorHolder.h"
#include "BubAnimator.h"
#include "AnimationsParser.h"
#include "ZenChanAnimator.h"
#include "BubblesAnimator.h"

/////////////////////////static functions

static bool CheckDxDirectionWalking(const std::vector<Animator*>& bub, bool direction){
	DASSERT( bub.size()==1 );
	BubWalkingAnimator* bubWalk = (BubWalkingAnimator*) bub.front();
	offset_t offset = bubWalk->GetAnimation()->GetDx();
	DASSERT( offset!=0 );
	if( ( direction ? (offset > 0) : (offset < 0) ) ) {
		bubWalk->GetAnimation()->SetDx(-offset);
		bubWalk->GetSprite()->SetGoesLeft(direction);
	}

	return true;
}

static bool CheckDxDirectionStand(const std::vector<Animator*>& bub, bool direction){
	DASSERT( bub.size()==1 );
	BubStandAnimator* _this = (BubStandAnimator*) bub.front();

	REMOVE_FROM_ACTION_ANIMATOR( _this );

	DASSERT( _this->GetAnimation() && _this->GetSprite() );

	animid_t id = _this->GetAnimation()->GetId();
	Sprite* newSprite = _this->GetSprite();
	newSprite->ClearListeners();
	FrameRangeAnimation * ma = (FrameRangeAnimation*) AnimationsParser::GetAnimation( direction ? "BubWalkLeft" : "BubWalkRight" );
	newSprite->SetFrame(0);
	newSprite->SetGoesLeft(direction);

	BubWalkingAnimator* mar = new BubWalkingAnimator();

	newSprite->AddStartFallingListener(mar);
	mar->SetOnFinish(BubWalkingAnimator::OnFinishCallback, mar);

	std::vector<Animator*> enemy = AnimatorHolder::GetAnimators(zenChanStandAnimator_t);
	for(unsigned int i=0; i<enemy.size(); ++i){
		CollisionChecker::Register(newSprite, ( (ZenChanStandAnimator*)enemy[i] )->GetSprite(), (void *)mar, BubWalkingAnimator::OnCollisionWithEnemy);
	}

	START_ANIMATOR( mar, newSprite, ma, GetGameTime() );
	DESTROY_ANIMATOR_WITHOUT_SPRITE( _this );
	return true;
}

static bool CheckDyDirectionWalking(const std::vector<Animator*>& bub){
	DASSERT( bub.size()==1 );
	BubWalkingAnimator* _this = (BubWalkingAnimator*) bub.front();
	
	REMOVE_FROM_ACTION_ANIMATOR( _this );

	DASSERT( _this->GetAnimation() && _this->GetSprite() );
	animid_t id = _this->GetAnimation()->GetId();

	Sprite* newSprite = _this->GetSprite();
	newSprite->ClearListeners();
	MovingPathAnimation * ma;
	if(newSprite->GoesLeft())
		ma=(MovingPathAnimation*) AnimationsParser::GetAnimation( "BubJumpWalkingLeft" );
	else
		ma=(MovingPathAnimation*) AnimationsParser::GetAnimation( "BubJumpWalkingRight" );
	BubJumpAnimator* mar = new BubJumpAnimator();
	mar->SetOnFinish(BubJumpAnimator::OnFinishCallback, mar);

	std::vector<Animator*> enemy = AnimatorHolder::GetAnimators(zenChanStandAnimator_t);
	for(unsigned int i=0; i<enemy.size(); ++i){
		CollisionChecker::Register(_this->GetSprite(), ( (ZenChanStandAnimator*)enemy[i] )->GetSprite(), (void *)mar, BubJumpAnimator::OnCollisionWithEnemy);
	}
	std::vector<Animator*> bubbles = AnimatorHolder::GetAnimators(bubBubbleAnimator_t);
	for(unsigned int i=0; i<bubbles.size(); ++i){
		CollisionChecker::Register(( (BubBubbleAnimator*)bubbles[i] )->GetSprite(), newSprite, (void*)bubbles[i], BubBubbleAnimator::OnCollisionWithBubJump);
	}
	
	START_ANIMATOR( mar, newSprite, ma, GetGameTime() );
	DESTROY_ANIMATOR_WITHOUT_SPRITE( _this );
	return true;
}

static bool CheckDyDirectionStand(const std::vector<Animator*>& bub){
	DASSERT( bub.size()==1 );
	BubStandAnimator* _this = (BubStandAnimator*) bub.front();

	timestamp_t timestamp = GetGameTime();
	DASSERT( timestamp>0 );
	AnimatorHolder::MarkAsSuspended(_this);
	AnimatorHolder::Cancel(_this);
	CollisionChecker::Cancel(_this->GetSprite());

	DASSERT( _this->GetAnimation() );
	DASSERT( _this->GetSprite() );

	animid_t id = _this->GetAnimation()->GetId();

	Sprite* newSprite = _this->GetSprite();

	_this->GetAnimation()->Destroy();
	_this->Destroy(); 

	newSprite->RemoveAllStartFallingListeners();
	newSprite->RemoveAllStopFallingListeners();
	MovingPathAnimation * ma = (MovingPathAnimation*) AnimationsParser::GetAnimation( "BubJumpStand" );

	BubJumpAnimator* mar = new BubJumpAnimator();
	mar->SetOnFinish(BubJumpAnimator::OnFinishCallback, mar);
	mar->Start(newSprite, ma, timestamp);

	std::vector<Animator*> enemy = AnimatorHolder::GetAnimators(zenChanStandAnimator_t);
	for(unsigned int i=0; i<enemy.size(); ++i){
		CollisionChecker::Register(_this->GetSprite(), ( (ZenChanStandAnimator*)enemy[i] )->GetSprite(), (void *)mar, BubJumpAnimator::OnCollisionWithEnemy);
	}
	std::vector<Animator*> bubbles = AnimatorHolder::GetAnimators(bubBubbleAnimator_t);
	for(unsigned int i=0; i<bubbles.size(); ++i){
		CollisionChecker::Register(( (BubBubbleAnimator*)bubbles[i] )->GetSprite(), newSprite, (void*)bubbles[i], BubBubbleAnimator::OnCollisionWithBubJump);
	}

	AnimatorHolder::Register(mar);
	AnimatorHolder::MarkAsRunning(mar);

	return true;
}


/////////////////////////interface functions


bool InputManageHandling::OnKeyUp(void){
	bool retVal = true;
	std::vector<Animator*> bub;

	if(!(bub = AnimatorHolder::GetAnimators(bubWalkAnimator_t)).empty()){
		retVal = CheckDyDirectionWalking(bub);
	}else
	if(!(bub = AnimatorHolder::GetAnimators(bubStandAnimator_t)).empty()){
		retVal = CheckDyDirectionStand(bub);
	}

	return retVal;
}

bool InputManageHandling::OnKeyDown(void){

	return true;
}

bool InputManageHandling::OnKeyLeft(void){
	bool retVal = true;
	std::vector<Animator*> bub;

	if(!(bub = AnimatorHolder::GetAnimators(bubWalkAnimator_t)).empty()){
		retVal = CheckDxDirectionWalking(bub, true);
	}else
	if(!(bub = AnimatorHolder::GetAnimators(bubStandAnimator_t)).empty()){
		retVal = CheckDxDirectionStand(bub, true);
	}
	
	return retVal;
}

bool InputManageHandling::OnKeyRight(void){
	bool retVal = true;
	std::vector<Animator*> bub;
		
	if(!(bub = AnimatorHolder::GetAnimators(bubWalkAnimator_t)).empty()){
		retVal = CheckDxDirectionWalking(bub, false);
	}else
	if(!(bub = AnimatorHolder::GetAnimators(bubStandAnimator_t)).empty()){
		retVal = CheckDxDirectionStand(bub, false);
	}
		
	return retVal;
}

bool InputManageHandling::OnKeySpace(void){
	bool retVal = true;
	std::vector<Animator*> bub;

	if(!(bub = AnimatorHolder::GetAnimators(bubWalkAnimator_t)).empty()){
		DASSERT( bub.size()==1 );
		( (BubWalkingAnimator*) bub.front() )->OnOpenMouth();
	}else	
	if(!(bub = AnimatorHolder::GetAnimators(bubStandAnimator_t)).empty()){
		DASSERT( bub.size()==1 );
		( (BubStandAnimator*) bub.front() )->OnOpenMouth();
	}
		
	return retVal;
}
