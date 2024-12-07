#pragma once

#include "Main.hpp"

namespace Events
{
	class ModEventSink :
		public RE::BSTEventSink<RE::TESDeathEvent>
	{
		ModEventSink() = default;
		ModEventSink(const ModEventSink&) = delete;
		ModEventSink(ModEventSink&&) = delete;
		ModEventSink& operator=(const ModEventSink&) = delete;
		ModEventSink& operator=(ModEventSink&&) = delete;

	public:
		static ModEventSink* GetSingleton()
		{
			static ModEventSink singleton;
			return &singleton;
		}

		static void LoadEvents()
		{
			auto* eventSink = GetSingleton();
			auto* eventSourceHolder = RE::ScriptEventSourceHolder::GetSingleton();
			eventSourceHolder->AddEventSink<RE::TESDeathEvent>(eventSink);
		}

		RE::BSEventNotifyControl ProcessEvent(const RE::TESDeathEvent* event, RE::BSTEventSource<RE::TESDeathEvent>*);
	};
};
