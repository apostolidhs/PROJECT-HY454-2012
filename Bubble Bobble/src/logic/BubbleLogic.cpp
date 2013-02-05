#include "BubbleLogic.h"
#include "MemoryManage.h"
#include <math.h>
#include <ctime>

typedef std::pair<unsigned int, std::string> fruitPair;

	BubProfile* BubbleLogic::bub;
	BubProfile* BubbleLogic::bob;

	Points BubbleLogic::highScore;

	Lifes BubbleLogic::startingLifes;

	double BubbleLogic::baronSecToRaiseMS;
	double BubbleLogic::baronDecrDelay;

	Points BubbleLogic::emptyBubblePoints;
	Points BubbleLogic::zenChanBubblePoints;
	Points BubbleLogic::mightaBubblePoints;
	Points BubbleLogic::bananaPoints;
	Points BubbleLogic::orangePoints;
	Points BubbleLogic::peachPoints;
	Points BubbleLogic::waterMelonPoints;
	Points BubbleLogic::blueDiamondPoints;
	Points BubbleLogic::powerUpPoints;

	unsigned int BubbleLogic::enemiesForBanana;
	unsigned int BubbleLogic::enemiesForOrange;
	unsigned int BubbleLogic::enemiesForPeach;
	unsigned int BubbleLogic::enemiesForWaterMelon;

	unsigned int BubbleLogic::crossStageForRedShoes;
	unsigned int BubbleLogic::jumpsForYellowSwt;
	unsigned int BubbleLogic::ponedBubblesForBlueSwt;
	unsigned int BubbleLogic::blustedBubblesForPurpleSwt;

	std::vector<Coordinates> BubbleLogic::powerUpPosXY;

	///////////// INITIALIZE
	void BubbleLogic::SingletonCreate(void){

		highScore = 0;
		startingLifes = 5;
		emptyBubblePoints = 10;
		powerUpPoints = 100;
		zenChanBubblePoints = 800;
		mightaBubblePoints = 1000;
		bananaPoints = 500;
		orangePoints = 1000;
		peachPoints = 2000;
		waterMelonPoints = 3000;
		blueDiamondPoints = 5000;

		enemiesForBanana = 1;
		enemiesForOrange = 2;
		enemiesForPeach = 3;
		enemiesForWaterMelon = 4;

		crossStageForRedShoes = 2;
		jumpsForYellowSwt = 5;
		ponedBubblesForBlueSwt = 5;
		blustedBubblesForPurpleSwt = 5;

		baronSecToRaiseMS = 5;
		baronDecrDelay = 10;

		bub = DNEWCLASS(BubProfile, (startingLifes) );
		bob = DNEWCLASS(BubProfile, (startingLifes) );

		powerUpPosXY.push_back( std::make_pair(100,95) );
		powerUpPosXY.push_back( std::make_pair(210,175) );
		powerUpPosXY.push_back( std::make_pair(100,255) );
		powerUpPosXY.push_back( std::make_pair(210,415) );

		srand((unsigned)time(0));
	}

	///////////// BUBBLUN
	BubProfile* BubbleLogic::GetBubProfile(void) { return bub; }
	BubProfile* BubbleLogic::GetBobProfile(void) { return bob; }

	void BubbleLogic::SetHighScore(Points newHighScore) { highScore = newHighScore; }
	Points BubbleLogic::GetHighScore(void) { return highScore; }

	void BubbleLogic::SetBubScore(Points newScore) {
		bub->SetScore(newScore);
		if (bub->GetScore() > highScore)
			highScore = bub->GetScore();
	}
	void BubbleLogic::IncrBubScore(Points somePoints) {
		bub->IncrScore(somePoints);
		if (bub->GetScore() > highScore)
			highScore = bub->GetScore();
	}
	void BubbleLogic::BubRedShoesAcquired(void) {
		BubbleLogic::IncrBubScore(powerUpPoints);
		bub->SetRedShoes(true);
	}
	void BubbleLogic::BubYellowSwtAcquired(void) {
		BubbleLogic::IncrBubScore(powerUpPoints);
		bub->SetYellowSwt(true);
	}
	void BubbleLogic::BubBlueSwtAcquired(void) {
		BubbleLogic::IncrBubScore(powerUpPoints);
		bub->SetBlueSwt(true);
	}
	void BubbleLogic::BubPurpleSwtAcquired(void) {
		BubbleLogic::IncrBubScore(powerUpPoints);
		bub->SetPurpleSwt(true);
	}
	void BubbleLogic::BubBananaAcquired(void) { IncrBubScore(bananaPoints); }
	void BubbleLogic::BubOrangeAcquired(void) { IncrBubScore(orangePoints); }
	void BubbleLogic::BubPeachAcquired(void) { IncrBubScore(peachPoints); }
	void BubbleLogic::BubWaterMelonAcquired(void) { IncrBubScore(waterMelonPoints); }
	void BubbleLogic::BubBlueDiamondAcquired(void) { IncrBubScore(blueDiamondPoints); }

	void BubbleLogic::BubPonEmptyBubble(void) { IncrBubScore(emptyBubblePoints); }
	void BubbleLogic::BubPonZenChanBubble(void) { IncrBubScore(zenChanBubblePoints); }
	void BubbleLogic::BubPonMightaBubble(void) { IncrBubScore(mightaBubblePoints); }


	///////////// BOBBLUN
	void BubbleLogic::SetBobScore(Points newScore) {
		bob->SetScore(newScore);
		if (bob->GetScore() > highScore) highScore = bob->GetScore();
	}
	void BubbleLogic::IncrBobScore(Points somePoints) {
		bob->IncrScore(somePoints);
		if (bob->GetScore() > highScore)
			highScore = bob->GetScore();
	}

	void BubbleLogic::BobRedShoesAcquired(void) {
		BubbleLogic::IncrBobScore(powerUpPoints);
		bob->SetRedShoes(true);
	}
	void BubbleLogic::BobYellowSwtAcquired(void) {
		BubbleLogic::IncrBobScore(powerUpPoints);
		bob->SetYellowSwt(true);
	}
	void BubbleLogic::BobBlueSwtAcquired(void) {
		BubbleLogic::IncrBobScore(powerUpPoints);
		bob->SetBlueSwt(true);
	}
	void BubbleLogic::BobPurpleSwtAcquired(void) {
		BubbleLogic::IncrBobScore(powerUpPoints);
		bob->SetPurpleSwt(true);
	}
	void BubbleLogic::BobBananaAcquired(void) {	IncrBobScore(bananaPoints); }
	void BubbleLogic::BobOrangeAcquired(void) {	IncrBobScore(orangePoints); }
	void BubbleLogic::BobPeachAcquired(void) { IncrBobScore(peachPoints); }
	void BubbleLogic::BobWaterMelonAcquired(void) { IncrBobScore(waterMelonPoints); }
	void BubbleLogic::BobBlueDiamondAcquired(void) { IncrBobScore(blueDiamondPoints); }

	void BubbleLogic::BobPonEmptyBubble(void) { IncrBobScore(emptyBubblePoints); }
	void BubbleLogic::BobPonZenChanBubble(void) { IncrBobScore(zenChanBubblePoints); }
	void BubbleLogic::BobPonMightaBubble(void) {IncrBobScore(mightaBubblePoints);  }

	///////////// GENERAL
	unsigned int BubbleLogic::GetEnemiesForBanana(void) { return enemiesForBanana; }
	unsigned int BubbleLogic::GetEnemiesForOrange(void) { return enemiesForOrange; }
	unsigned int BubbleLogic::GetEnemiesForPeach(void) { return enemiesForPeach; }
	unsigned int BubbleLogic::GetEnemiesForWaterMelon(void) { return enemiesForWaterMelon; }
	unsigned int BubbleLogic::GetFruitType(unsigned int enemiesTerminated){

		if (enemiesTerminated <= enemiesForBanana)		return 1;
		else if (enemiesTerminated <= enemiesForOrange) return 2;
		else if (enemiesTerminated <= enemiesForPeach)	return 3;
		else											return 4;
	}

	unsigned int BubbleLogic::GetCrossStageForRedShoes(void) { return crossStageForRedShoes; }
	unsigned int BubbleLogic::GetJumpsForYellowSwt(void) { return jumpsForYellowSwt; }
	unsigned int BubbleLogic::GetPonedBubblesForBlueSwt(void) { return ponedBubblesForBlueSwt; }
	unsigned int BubbleLogic::GetBlustedBubblesForPurpleSwt(void) { return blustedBubblesForPurpleSwt; }

	Coordinates BubbleLogic::GetPowerUpPosXY(void){
		return powerUpPosXY[rand()%powerUpPosXY.size()];
	}

	Points GetPointsOfPoning(unsigned int enemiesPoned){
		return std::pow((double)2, (double)enemiesPoned-1 ) * 1000;
	}

	///////////// BARRON VON BLUBA
	double BubbleLogic::GetBaronSecToRaiseMS(void) { return baronSecToRaiseMS; }
	double BubbleLogic::GetBaronDecrDelay(void) { return baronDecrDelay; }

	///////////// CLEANUP
	void BubbleLogic::SingletonCleanUp(void){
		DDELETE( bub);
		DDELETE( bob);
	}