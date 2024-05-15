#include "utils.h"
#include "hooks.h"
#include "dataHandler.h"

void hitEventHook::processHitTemplate(RE::Actor* target, RE::HitData& hitData)
{
	if (processHit(target, hitData) && !target->IsDead()) {
		hitData.totalDamage *= 10.0f;
	}

	_ProcessHit(target, hitData);
}

bool hitEventHook::processHit(RE::Actor* target, RE::HitData& hitData)
{
	using HITFLAG = RE::HitData::Flag;
	auto aggressor = hitData.aggressor.get();

	if (!target || !aggressor) return false;

	float targetHealth = target->AsActorValueOwner()->GetActorValue(RE::ActorValue::kHealth);
	bool isFatal = ((targetHealth - hitData.totalDamage < 0.0f) && hitData.flags.any(HITFLAG::kFatal));

	if (!isFatal && !target->IsDead()) return false;

	if (target->IsEssential() || (target->IsProtected() && !aggressor->IsPlayerRef())) return false;

	// Checking actor eligibility

	RE::TESRace* race = target->GetRace();
	if (!race) return false;

	RE::TESObjectARMO* decapitateArmor = race->decapitateArmors[target->GetActorBase()->GetSex()];
	if (!decapitateArmor) return false;

	RE::InventoryChanges* inventoryChanges = target->GetInventoryChanges();
	RE::BSSimpleList<RE::InventoryEntryData*> ::iterator itr, itrEnd;
	itrEnd = inventoryChanges->entryList->end();

	for (itr = inventoryChanges->entryList->begin(); itr != itrEnd; ++itr) {
		RE::InventoryEntryData* inventoryEntryData = *itr;
		RE::TESForm* item = inventoryEntryData->object;
		if (item->formID == decapitateArmor->formID) return false;
	}

	// if (race->data.flags.any(RE::RACE_DATA::Flag::kAllowRagdollCollision)) return false;

	// Checking worn helmet conditions
	RE::TESObjectARMO* currentHelmet = target->GetWornArmor(RE::BGSBipedObjectForm::BipedObjectSlot::kHair);
	if (currentHelmet) {
		if (Settings::bAlwaysExcludeHeavyHelmets && currentHelmet->IsHeavyArmor()) return false;
		if (currentHelmet == target->GetWornArmor(RE::BGSBipedObjectForm::BipedObjectSlot::kHead)) { // Full helmet
			if (!Settings::bAllowFullHelmets) return false;
		}
		else { // Neck visible helmet
			if (!Settings::bAllowNeckVisibleHelmets) return false;
		}
	}

	// Checking conditions
	if (Settings::bPlayerOnly && !aggressor->IsPlayerRef()) return false;
	if (Settings::bPlayerImmune && target->IsPlayerRef()) return false;
	if (!Settings::bOnKillmove && target->IsInKillMove()) return false;

	bool isPowerAttack = hitData.flags.any(HITFLAG::kPowerAttack);
	float randomChances = static_cast<float>(rand()) / RAND_MAX * 100.0f;
	if (!isPowerAttack && randomChances > Settings::fRegularAttackChances) return false;
	if (isPowerAttack && randomChances > Settings::fPowerAttackChances) return false;

	if (Settings::bOnKill && !isFatal) return false;
	if (!Settings::bIfDead && target->IsDead()) return false;
	if (!Settings::bIfBlocked && hitData.flags.any(HITFLAG::kBlockWithWeapon)) return false;

	RE::WEAPON_TYPE weaponType = RE::WEAPON_TYPE::kHandToHandMelee;
	if (hitData.weapon) {
		weaponType = hitData.weapon->GetWeaponType();
		if (weaponType == RE::WEAPON_TYPE::kOneHandSword) {
			if (!Settings::bOneHandedSword) return false;
		}
		else if (weaponType == RE::WEAPON_TYPE::kTwoHandSword) {
			if (!Settings::bTwoHandedSword) return false;
		}
		else if (weaponType == RE::WEAPON_TYPE::kOneHandAxe) {
			if (!Settings::bOneHandedAxe) return false;
		}
		else if (weaponType == RE::WEAPON_TYPE::kTwoHandAxe) {
			if (!Settings::bTwoHandedAxe) return false;
		}
		else if (weaponType == RE::WEAPON_TYPE::kOneHandDagger) {
			if (!Settings::bDagger) return false;
		}
		else if (weaponType == RE::WEAPON_TYPE::kOneHandMace) {
			if (!Settings::bMace) return false;
		}
		else if (weaponType == RE::WEAPON_TYPE::kBow || weaponType == RE::WEAPON_TYPE::kCrossbow) {
			if (!Settings::bRanged) return false;
		}
		else if (!Settings::bOther) return false;
	}
	else if (!Settings::bOther) return false;

	RE::NiAVObject* targetNode = target->GetNodeByName("NPC Neck [Neck]");
	if (!targetNode) targetNode = target->GetNodeByName("NPC Head [Head]");
	if (!targetNode) return false;

	RE::NiPoint3 hitPos = hitData.hitPosition;
	RE::NiPoint3 headPos = targetNode->world.translate;

	float distance = hitPos.GetDistance(headPos);
	if (distance > Settings::fHitPrecision) return false;

	if (Settings::bPlaySound) {
		switch (weaponType) {
		case RE::WEAPON_TYPE::kOneHandSword:
		case RE::WEAPON_TYPE::kTwoHandSword:
		case RE::WEAPON_TYPE::kOneHandAxe:
		case RE::WEAPON_TYPE::kTwoHandAxe:
		case RE::WEAPON_TYPE::kOneHandDagger:
			Utils::playSound(target, dataHandler::GetSingleton()->soundBeheadSlash);
		default:
			Utils::playSound(target, dataHandler::GetSingleton()->soundBeheadSmash);
		}
	}

	target->Decapitate();

	logger::info("{} decapitated {}!", aggressor->GetName(), target->GetName());

	return true;
}