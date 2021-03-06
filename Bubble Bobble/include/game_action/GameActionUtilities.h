#ifndef GAMEACTIONUTILITIES_H
#define GAMEACTIONUTILITIES_H

#define INIT_NEW_INSTANCE(anim_t, anim, animStateStr, animr_t, animr, sprite, spriteStateStr, _x, _y, gravAddict, goLeft)			\
	Sprite* sprite = new Sprite(_x,_y,gravAddict,AnimationFilmHolder::GetFilm(spriteStateStr), Terrain::GetActionLayer(), goLeft);	\
	anim_t* anim = (anim_t*) AnimationsParser::GetAnimation(animStateStr);															\
	animr_t* animr = new animr_t()

#define INIT_NEW_INSTANCE_WITH_SPRITE(anim_t, anim, animStateStr, animr_t, animr, Sprt )\
	Sprt->ClearListeners();																\
	anim_t *anim = (anim_t*) AnimationsParser::GetAnimation(animStateStr);				\
	animr_t* animr = new animr_t();																		

#define START_ANIMATOR( animr, sprite, anim, time )	\
	animr->Start( sprite, anim, time);				\
	AnimatorHolder::Register( animr );				\
	AnimatorHolder::MarkAsRunning( animr )

#define START_TIME_ANIMATOR( animr, time )			\
	animr->Start( time);							\
	AnimatorHolder::Register( animr );				\
	AnimatorHolder::MarkAsRunning( animr )

#define DESTROY_ANIMATOR( anim )				\
	anim->GetSprite()->Destroy();				\
	DESTROY_ANIMATOR_WITHOUT_SPRITE( anim )

#define DESTROY_ANIMATOR_WITHOUT_SPRITE( anim )	\
	anim->GetAnimation()->Destroy();			\
	anim->Destroy()

#define REMOVE_FROM_ACTION_ANIMATOR( anim )		\
	AnimatorHolder::MarkAsSuspended( anim );	\
	AnimatorHolder::Cancel( anim );				\
	CollisionChecker::Cancel( anim->GetSprite() )

#define REGISTER_COLLISION_PAIRS_WITH_TYPE(mysprite, subAnimator_t, animator_enum_t)		\
	std::vector<Animator*> v = AnimatorHolder::GetAnimators(animator_enum_t);				\
	for(unsigned int i=0; i<v.size(); ++i){													\
		CollisionChecker::Register(mysprite, ( (subAnimator_t*)v[i] )->GetSprite(), 0, 0);	\
	}

#define REFRESH_KILLED_ENEMIES_COUNTER();																						\
	if (AnimatorHolder::GetAnimators(zenChanDieAnimator_t, mightaDieFallingAnimator_t).empty() ){										\
		BubbleLogic::GetBubProfile()->IncrScore ( BubbleLogic::GetPointsOfPoning(BubbleLogic::GetBubProfile()->GetEnemiesKilledOnPon()) );	\
		BubbleLogic::GetBubProfile()->SetEnemiesKilledOnPon(0);																	\
	}

#define SCROLL_IF_NO_ENEMIES();																			\
	if (AnimatorHolder::GetAnimators(zenChanStandAnimator_t, mightaThrowFireBallAnimator_t).empty() &&	\
		AnimatorHolder::GetAnimators(BananaAnimator_t, WaterMelonAnimator_t).empty() &&					\
		AnimatorHolder::GetAnimators(BlueDiamondAnimator_t, PurpleSweetAnimator_t).empty() &&			\
		AnimatorHolder::GetAnimators(zenChanInBubble_t, mightaInBubbleHighAngry_t).empty()				\
		){													\
		BubbleLogic::IncrStageLevel();						\
		StageInitializer::ScrollToNextStage();				\
	}														\

#endif