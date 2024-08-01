#pragma once

class dataHandler
{
public:
	static dataHandler* GetSingleton()
	{
		static dataHandler singleton;
		return  std::addressof(singleton);
	}

	void loadData();

	void loadSoundDescriptors(RE::TESDataHandler* dataHandler);

	RE::BGSSoundDescriptorForm* soundBeheadSlash;
	RE::BGSSoundDescriptorForm* soundBeheadSmash;
};

struct Settings
{
	static void ReadSettings();

	// General
	static inline float fRegularAttackChances = 35.0f;
	static inline float fPowerAttackChances = 80.0f;
	static inline float fHitPrecision = 25.0f;
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
};