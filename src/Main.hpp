#pragma once

#include "DataHandler.hpp"
#include "MiscUtils.hpp"
#include "Settings.hpp"

#include "NGD-API.h"

namespace Events
{
	using namespace ModData;

	class MainEvent
	{
		public:

			using DecapitateParams = NGDecapitationsAPI::DecapitateParams;

			using HITFLAG = RE::HitData::Flag;

			static inline std::unordered_map<RE::Actor*, RE::HitData> deferredHitMap;
			static inline std::mutex deferredHitMapMutex;

			static void InstallHooks()
			{
				REL::Relocation<uintptr_t> hook{ REL::RelocationID(37673, 38627) };
				SKSE::AllocTrampoline(1 << 4);
				auto& trampoline = SKSE::GetTrampoline();
				_ProcessHit = trampoline.write_call<5>(hook.address() + REL::Relocate(0x3C0, 0x4A8), ProcessHitTemplate);
				logger::info("ProcessHit hooked at address: 0x{:X}", _ProcessHit.address());
			};
			
			static bool ProcessHit(RE::Actor* target, RE::HitData& hitData)
			{
				return ProcessDismemberment(target, hitData);
			}

			static void ProcessDeferredHit(RE::Actor* actor)
			{
				std::lock_guard<std::mutex> lock(deferredHitMapMutex);
				auto it = deferredHitMap.find(actor);
				if (it != deferredHitMap.end()) {
					const float actorHealth = actor->AsActorValueOwner()->GetActorValue(RE::ActorValue::kHealth);
					if (actor && (actorHealth <= 0.0f || actor->IsDead())) {
						ProcessHit(actor, it->second);
						deferredHitMap.erase(it);
					}
				}
			}

		private:

			static inline REL::Relocation<decltype(ProcessHit)> _ProcessHit;

			static void ClearDeferredHitMap(RE::Actor* target)
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(100));
				{
					std::lock_guard<std::mutex> lock(deferredHitMapMutex);
					auto it = deferredHitMap.find(target);
					if (it != deferredHitMap.end()) {
						deferredHitMap.erase(it);
					}
				}
			}

			static void ProcessHitTemplate(RE::Actor* target, RE::HitData& hitData)
			{
				if (target->IsDead() || target->IsInKillMove()) {
					ProcessHit(target, hitData);
				} else {
					deferredHitMap[target] = hitData;

					std::thread delayThread(ClearDeferredHitMap, target);
					delayThread.detach();
				}

				_ProcessHit(target, hitData);
			}

			static bool ProcessDismemberment(RE::Actor* target, RE::HitData& hitData)
			{
				RE::Actor* aggressor = (hitData.aggressor.get() ? hitData.aggressor.get()->As<RE::Actor>() : nullptr);

				if (!target || !aggressor) return false;

				float targetHealth = target->AsActorValueOwner()->GetActorValue(RE::ActorValue::kHealth);
				bool isFatal = ((targetHealth - hitData.totalDamage < 0.0f) && hitData.flags.any(HITFLAG::kFatal));
				if (!isFatal && !target->IsDead()) return false;
				if (target->IsEssential() || (target->IsProtected() && !aggressor->IsPlayerRef())) return false;

				// Checking actor eligibility
				RE::TESRace* race = target->GetRace();
				if (!race) return false;

				if (ModData::DataHandler::NextGenDecapitationsEnabled) {
					if (NGDecapitationsAPI::g_API->IsDecapitated(target)) return false;
				} else {
					RE::TESObjectARMO* decapitateArmor = race->decapitateArmors[target->GetActorBase()->GetSex()];
					if (!decapitateArmor) return false;

					RE::InventoryChanges* inventoryChanges = target->GetInventoryChanges();
					RE::BSSimpleList<RE::InventoryEntryData*>::iterator itr, itrEnd;
					itrEnd = inventoryChanges->entryList->end();

					for (itr = inventoryChanges->entryList->begin(); itr != itrEnd; ++itr) {
						RE::InventoryEntryData* inventoryEntryData = *itr;
						RE::TESForm* item = inventoryEntryData->object;
						if (item->formID == decapitateArmor->formID) return false;
					}
				}

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
				if (!Settings::bOnKillmove && (target->IsInKillMove() || aggressor->IsInKillMove())) return false;
				if (Settings::bExcludeUniqueNPC && target->GetActorBase()->IsUnique()) return false;

				bool isPowerAttack = hitData.flags.any(HITFLAG::kPowerAttack);
				float randomChances = static_cast<float>(rand()) / RAND_MAX * 100.0f;
				if (!isPowerAttack && randomChances > Settings::fRegularAttackChances) return false;
				if (isPowerAttack && randomChances > Settings::fPowerAttackChances) return false;

				if (!Settings::bOnKill && !target->IsDead()) return false;
				if (!Settings::bIfDead && target->IsDead()) return false;
				if (!Settings::bIfBlocked && hitData.flags.any(HITFLAG::kBlockWithWeapon)) return false;

				RE::WEAPON_TYPE weaponType = RE::WEAPON_TYPE::kHandToHandMelee;
				if (hitData.weapon) {
					weaponType = hitData.weapon->GetWeaponType();
					if (hitData.weapon->HasKeywordString("WeapTypeWarhammer")) {
						if (!Settings::bTwoHandedMace) return false;
					}
					else if (weaponType == RE::WEAPON_TYPE::kOneHandSword) {
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

				const auto& dataHandler = DataHandler::GetSingleton();
				if (Settings::bPlaySound) {
					switch (weaponType) {
					case RE::WEAPON_TYPE::kOneHandSword:
					case RE::WEAPON_TYPE::kTwoHandSword:
					case RE::WEAPON_TYPE::kOneHandAxe:
					case RE::WEAPON_TYPE::kTwoHandAxe:
					case RE::WEAPON_TYPE::kOneHandDagger:
						MiscUtils::PlaySound(target, dataHandler->soundBeheadSlash);
					default:
						MiscUtils::PlaySound(target, dataHandler->soundBeheadSmash);
					}
				}

				if (ModData::DataHandler::NextGenDecapitationsEnabled) {
					DecapitateParams decapitateParams;
					decapitateParams.customHitData = true;
					decapitateParams.hitFromPosition = hitData.hitPosition;
					decapitateParams.hitToPosition = (hitData.hitPosition + hitData.hitDirection);
					decapitateParams.hitPower = Settings::fHeadImpulseFactor;

					if (!NGDecapitationsAPI::g_API->Decapitate(target, &decapitateParams)) return false;
				} else target->Decapitate();
	
				logger::info("Decapitated actor : <\"{}\" [REF:{:08X}] [BASE:{:08X}]>", target->GetName(), target->formID, target->GetActorBase()->formID);

				return true;
			}
	};
};
