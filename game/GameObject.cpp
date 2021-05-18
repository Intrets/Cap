#include "GameObject.h"
//# start
void Everything::remove(SizeAlias i) {
	if (i == 0) return;
	if (hasgameposition(i)) {
		gamepositions[indirectionMap[i].gameposition_] = std::move(gamepositions[indirectionMap[gamepositionlast].gameposition_]);
	}
	if (hasgraphicstile(i)) {
		graphicstiles[indirectionMap[i].graphicstile_] = std::move(graphicstiles[indirectionMap[graphicstilelast].graphicstile_]);
	}
	if (hasbrain(i)) {
		brains[indirectionMap[i].brain_] = std::move(brains[indirectionMap[brainlast].brain_]);
	}
	if (hasnutrition(i)) {
		nutritions[indirectionMap[i].nutrition_] = std::move(nutritions[indirectionMap[nutritionlast].nutrition_]);
	}
	if (haslocomotion(i)) {
		locomotions[indirectionMap[i].locomotion_] = std::move(locomotions[indirectionMap[locomotionlast].locomotion_]);
	}
	if (haspossession(i)) {
		possessions[indirectionMap[i].possession_] = std::move(possessions[indirectionMap[possessionlast].possession_]);
	}
	if (hasvicinity(i)) {
		vicinitys[indirectionMap[i].vicinity_] = std::move(vicinitys[indirectionMap[vicinitylast].vicinity_]);
	}
};
UniqueGameObject::UniqueGameObject(UniqueGameObject&& other) {
	this->proxy->remove(this->index);
	this->index = other.index;
	this->proxy = other.proxy;
	other.index = 0;
	other.proxy = nullptr;
};
UniqueGameObject& UniqueGameObject::operator=(UniqueGameObject&& other) {
	this->index = other.index;
	this->proxy = other.proxy;
	other.index = 0;
	other.proxy = nullptr;
	return *this;
};
//# end
