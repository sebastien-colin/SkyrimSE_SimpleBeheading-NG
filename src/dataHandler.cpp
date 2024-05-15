#include "dataHandler.h"
#include "utils.h"

#include "SimpleIni.h"

void dataHandler::loadData() {

	logger::info("Loading data...");

	RE::TESDataHandler* dataHandler = RE::TESDataHandler::GetSingleton();

	loadSoundDescriptors(dataHandler);

	logger::info("Loading data: DONE");
}

void dataHandler::loadSoundDescriptors(RE::TESDataHandler* dataHandler) {
	logger::info("Loading Sound Descriptors...");
	soundBeheadSlash = dataHandler->LookupForm<RE::BGSSoundDescriptorForm>(0xDAB81, "Skyrim.esm");
	soundBeheadSmash = dataHandler->LookupForm<RE::BGSSoundDescriptorForm>(0xDCAF7, "Skyrim.esm");

	if (!soundBeheadSlash || !soundBeheadSmash) {
		logger::error("Error: Sound Descriptors not found in Skyrim.esm");
		RE::DebugMessageBox("Error: Sound Descriptors not found in Skyrim.esm");
	}
	logger::info("Loading Sound Descriptors: DONE");
}

void Settings::ReadSettings()
{
	logger::info("Reading settings...");
	constexpr auto path = L"Data/SKSE/Plugins/SimpleBeheading.ini";

	CSimpleIniA ini;
	ini.SetUnicode();
	ini.LoadFile(path);

	// General
	fRegularAttackChances = (float)ini.GetDoubleValue("General", "fRegularAttackChances", 35.0f);
	fPowerAttackChances = (float)ini.GetDoubleValue("General", "fPowerAttackChances", 80.0f);
	fHitPrecision = (float)ini.GetDoubleValue("General", "fHitPrecision", 25.0f);
	bPlaySound = ini.GetBoolValue("General", "bPlaySound", true);

	// Conditions
	bPlayerOnly = ini.GetBoolValue("Conditions", "bPlayerOnly", false);
	bPlayerImmune = ini.GetBoolValue("Conditions", "bPlayerImmune", true);
	bOnKill = ini.GetBoolValue("Conditions", "bOnKill", true);
	bIfDead = ini.GetBoolValue("Conditions", "bIfDead", true);
	bIfBlocked = ini.GetBoolValue("Conditions", "bIfBlocked", false);
	bOnKillmove = ini.GetBoolValue("Conditions", "bOnKillmove", false);

	// Helmet conditions
	bAllowNeckVisibleHelmets = ini.GetBoolValue("HelmetConditions", "bAllowNeckVisibleHelmets", true);
	bAllowFullHelmets = ini.GetBoolValue("HelmetConditions", "bAllowFullHelmets", false);
	bAlwaysExcludeHeavyHelmets = ini.GetBoolValue("HelmetConditions", "bAlwaysExcludeHeavyHelmets", false);

	// WeaponTypes
	bOneHandedSword = ini.GetBoolValue("WeaponTypes", "bOneHandedSword", true);
	bTwoHandedSword = ini.GetBoolValue("WeaponTypes", "bTwoHandedSword", true);
	bOneHandedAxe = ini.GetBoolValue("WeaponTypes", "bOneHandedAxe", true);
	bTwoHandedAxe = ini.GetBoolValue("WeaponTypes", "bTwoHandedAxe", true);
	bDagger = ini.GetBoolValue("WeaponTypes", "bDagger", false);
	bMace = ini.GetBoolValue("WeaponTypes", "bMace", false);
	bRanged = ini.GetBoolValue("WeaponTypes", "bRanged", false);
	bOther = ini.GetBoolValue("WeaponTypes", "bOther", false);

	logger::info("Reading settings: DONE");
}