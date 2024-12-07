#pragma once

class Settings
{
public:

	// General
	static inline int iVerboseMode = 1;

	static inline float fRegularAttackChances = 35.0f;
	static inline float fPowerAttackChances = 80.0f;
	static inline float fHitPrecision = 25.0f;
	static inline float fHeadImpulseFactor = 8.0f;
	static inline bool bPlaySound = true;

	// Conditions
	static inline bool bPlayerOnly = false;
	static inline bool bPlayerImmune = true;
	static inline bool bOnKill = true;
	static inline bool bIfDead = true;
	static inline bool bIfBlocked = false;
	static inline bool bOnKillmove = false;
	static inline bool bExcludeUniqueNPC = false;

	// Helmet conditions
	static inline bool bAllowNeckVisibleHelmets = true;
	static inline bool bAllowFullHelmets = false;
	static inline bool bAlwaysExcludeHeavyHelmets = false;

	// WeaponTypes
	static inline bool bOneHandedSword = true;
	static inline bool bTwoHandedSword = true;
	static inline bool bOneHandedAxe = true;
	static inline bool bTwoHandedAxe = true;
	static inline bool bTwoHandedMace = false;
	static inline bool bDagger = false;
	static inline bool bMace = false;
	static inline bool bRanged = false;
	static inline bool bOther = false;

	static bool ReadSettings()
	{
		constexpr auto path = L"Data/SKSE/Plugins/SimpleBeheading.ini";

		if (!std::filesystem::exists(path)) return false;

		CSimpleIniA ini;
		ini.SetUnicode();
		SI_Error rc = ini.LoadFile(path);

		if (rc < 0) return false;

		// General
		iVerboseMode = ini.GetLongValue("General", "iVerboseMode", 1);

		fRegularAttackChances = static_cast<float>(ini.GetDoubleValue("General", "fRegularAttackChances", 35.0f));
		fPowerAttackChances = static_cast<float>(ini.GetDoubleValue("General", "fPowerAttackChances", 80.0f));
		fHitPrecision = static_cast<float>(ini.GetDoubleValue("General", "fHitPrecision", 25.0f));
		fHeadImpulseFactor = static_cast<float>(ini.GetDoubleValue("General", "fHeadImpulseFactor", 8.0f));
		bPlaySound = ini.GetBoolValue("General", "bPlaySound", true);

		// Conditions
		bPlayerOnly = ini.GetBoolValue("Conditions", "bPlayerOnly", false);
		bPlayerImmune = ini.GetBoolValue("Conditions", "bPlayerImmune", true);
		bOnKill = ini.GetBoolValue("Conditions", "bOnKill", true);
		bIfDead = ini.GetBoolValue("Conditions", "bIfDead", true);
		bIfBlocked = ini.GetBoolValue("Conditions", "bIfBlocked", false);
		bOnKillmove = ini.GetBoolValue("Conditions", "bOnKillmove", false);
		bExcludeUniqueNPC = ini.GetBoolValue("Conditions", "bExcludeUniqueNPC", false);

		// Helmet conditions
		bAllowNeckVisibleHelmets = ini.GetBoolValue("HelmetConditions", "bAllowNeckVisibleHelmets", true);
		bAllowFullHelmets = ini.GetBoolValue("HelmetConditions", "bAllowFullHelmets", false);
		bAlwaysExcludeHeavyHelmets = ini.GetBoolValue("HelmetConditions", "bAlwaysExcludeHeavyHelmets", false);

		// WeaponTypes
		bOneHandedSword = ini.GetBoolValue("WeaponTypes", "bOneHandedSword", true);
		bTwoHandedSword = ini.GetBoolValue("WeaponTypes", "bTwoHandedSword", true);
		bOneHandedAxe = ini.GetBoolValue("WeaponTypes", "bOneHandedAxe", true);
		bTwoHandedAxe = ini.GetBoolValue("WeaponTypes", "bTwoHandedAxe", true);
		bTwoHandedMace = ini.GetBoolValue("WeaponTypes", "bTwoHandedMace", false);
		bDagger = ini.GetBoolValue("WeaponTypes", "bDagger", false);
		bMace = ini.GetBoolValue("WeaponTypes", "bMace", false);
		bRanged = ini.GetBoolValue("WeaponTypes", "bRanged", false);
		bOther = ini.GetBoolValue("WeaponTypes", "bOther", false);

		// Apply constraints
		fRegularAttackChances = std::clamp(fRegularAttackChances, 0.0f, 100.0f);
		fPowerAttackChances = std::clamp(fPowerAttackChances, 0.0f, 100.0f);

		fHitPrecision = std::clamp(fHitPrecision, 1.0f, 300.0f);
		fHeadImpulseFactor = std::clamp(fHeadImpulseFactor, 0.0f, 30.0f);

		return true;
	}
};
