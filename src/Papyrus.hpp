#pragma once

#include "Settings.hpp"

namespace Papyrus
{
	void ReadSettings(RE::StaticFunctionTag*)
	{
		Settings::ReadSettings();
	}

	bool BindPapyrusFunctions(RE::BSScript::IVirtualMachine* vm)
	{
		vm->RegisterFunction("ReadSettings", "SimpleBeheading", ReadSettings);
		return true;
	}
};
