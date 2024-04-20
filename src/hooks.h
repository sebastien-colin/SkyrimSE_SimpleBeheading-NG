#pragma once
class hitEventHook //props to Loki
{
    public:
        static void InstallHook() {
            REL::Relocation<uintptr_t> hook{ REL::RelocationID(37673, 38627) };
            SKSE::AllocTrampoline(1 << 4);
            auto& trampoline = SKSE::GetTrampoline();
            _ProcessHit = trampoline.write_call<5>(hook.address() + REL::Relocate(0x3C0, 0x4A8), processHitTemplate);
            logger::info("Hit event hook initialized.");
        };

    private:
        static void processHitTemplate(RE::Actor* target, RE::HitData& hitData);
        static bool processHit(RE::Actor* target, RE::HitData& hitData);
        static inline REL::Relocation<decltype(processHit)> _ProcessHit;
};