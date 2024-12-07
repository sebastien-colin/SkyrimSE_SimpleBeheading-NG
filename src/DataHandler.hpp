#pragma once

namespace ModData
{
	class DataHandler
	{
	public:
		static DataHandler* GetSingleton()
		{
			static DataHandler singleton;
			return &singleton;
		}
		
		static inline bool NextGenDecapitationsEnabled = false;

		RE::TESDataHandler*    TESdataHandler;

		// Properties storing game form references
		RE::BGSSoundDescriptorForm* soundBeheadSlash;
		RE::BGSSoundDescriptorForm* soundBeheadSmash;

		struct PluginForm
		{
			std::string_view name;
			void**           formPtr;
			uint32_t         formID;
			std::string_view pluginName;
		};
		const std::vector<PluginForm> pluginForms = {
			{ "soundBeheadSlash", reinterpret_cast<void**>(&soundBeheadSlash), 0xDAB81, "Skyrim.esm" },
			{ "soundBeheadSmash", reinterpret_cast<void**>(&soundBeheadSmash), 0xDCAF7, "Skyrim.esm" }
		};

		void LoadData();
	};

	inline void DataHandler::LoadData()
	{
        logger::info("Loading data...");
		TESdataHandler = RE::TESDataHandler::GetSingleton();

		for (const auto& formInfo : pluginForms) {
			*formInfo.formPtr = TESdataHandler->LookupForm(formInfo.formID, formInfo.pluginName.data());
			if (!*formInfo.formPtr) {
                std::string errorMessage = fmt::format("Error: Form \"{}\" not found in \"{}\".", formInfo.name, formInfo.pluginName);
                spdlog::critical(errorMessage);
				util::report_and_fail(errorMessage);
			}
		}

		logger::info("Loading data: DONE");
	}
}
