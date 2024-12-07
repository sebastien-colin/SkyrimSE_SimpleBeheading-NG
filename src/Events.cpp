#include "Events.h"

namespace Events
{
	RE::BSEventNotifyControl ModEventSink::ProcessEvent(const RE::TESDeathEvent* event, RE::BSTEventSource<RE::TESDeathEvent>*)
	{
		RE::Actor* target = event->actorDying->As<RE::Actor>();
		if (!target) return RE::BSEventNotifyControl::kContinue;

		Events::MainEvent::ProcessDeferredHit(target);

		return RE::BSEventNotifyControl::kContinue;
	}
}
