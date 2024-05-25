
namespace papyrus
{
	inline unsigned int functions_counter = 0;

	static void ReadSettings(RE::StaticFunctionTag*)
	{
		Settings::ReadSettings();
	}

	/**
	 * \brief Register functions for Papyrus scripts.
	 * \param a_virtual_machine   - Papyrus virtual machine.
	 * \return                  - True, if everything went fine.
	 */
	inline bool RegisterFunctions(RE::BSScript::IVirtualMachine* a_virtual_machine)
	{
		a_virtual_machine->RegisterFunction("ReadSettings", Plugin::NAME, ReadSettings);
		functions_counter++;

		logger::info("Registered {} Papyrus function/s.", functions_counter);
		return true;
	}
}