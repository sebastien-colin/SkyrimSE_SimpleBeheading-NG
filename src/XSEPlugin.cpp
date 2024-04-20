#include "hooks.h"
#include "dataHandler.h"

void MessageHandler(SKSE::MessagingInterface::Message* a_msg)
{
	switch (a_msg->type) {
	case SKSE::MessagingInterface::kDataLoaded:
		dataHandler::GetSingleton()->loadData();
		Settings::ReadSettings();
	}
}

void Load()
{
	SKSE::GetMessagingInterface()->RegisterListener("SKSE", MessageHandler);
	hitEventHook::InstallHook();
}