#pragma once

/*******************************************************************
* NEXT-GEN DECAPITATION - API
* Do not forget to include this source file to your project!
*******************************************************************/

/* How to create a hook to the API and use it:
SKSE::GetMessagingInterface()->RegisterListener([](MessagingInterface::Message* message) 
{
    switch (message->type) 
    {
        case MessagingInterface::kPostLoadGame:
        case MessagingInterface::kNewGame:
        {
            if (!NGDecapitationsAPI::LoadAPI()) {
				util::report_and_fail("Failed to bound to the Next-Gen Decapitations API");
			}
			NGDecapitationsAPI::g_API->GetVersion();
        }
        break;
    }
});
*/

// Define the API type key
#define NGD_API_TYPE_KEY static_cast<uint32_t>(0x00000108)

// Define the API version in a structured format
#define NGD_API_VERSION_MAJOR 1
#define NGD_API_VERSION_MINOR 0
#define NGD_API_VERSION_PATCH 0

// Combine the version numbers into a single value
#define NGD_API_VERSION ((NGD_API_VERSION_MAJOR << 16) | (NGD_API_VERSION_MINOR << 8) | NGD_API_VERSION_PATCH)

namespace NGDecapitationsAPI
{
	struct DecapitateParams
	{
		bool         customHitData = false;
		RE::NiPoint3 hitFromPosition;
		RE::NiPoint3 hitToPosition;
		float        hitPower;
		bool         motionless = false;
	};

	class NGDecapitationsAPI
	{
	public:
		// API functions
		virtual size_t GetVersion() const;

		virtual bool Decapitate(RE::Actor* target, DecapitateParams* params = nullptr) const;
	
		virtual bool IsDecapitated(RE::Actor* actor) const;

		virtual bool IsHead(RE::Actor* actor) const;
	};

	// Global API pointer
	inline extern NGDecapitationsAPI* g_API = nullptr;

	// Call this function only after the kDataLoaded event
	inline bool LoadAPI()
	{
		if (g_API != nullptr) return true;
		SKSE::GetMessagingInterface()->Dispatch(NGD_API_TYPE_KEY, (void*)&g_API, sizeof(void*), NULL);
		if (g_API) { // API successfully received!
			// Check if the API version matches
			return (g_API->GetVersion() == NGD_API_VERSION);
		}
		return false;
	}
}
