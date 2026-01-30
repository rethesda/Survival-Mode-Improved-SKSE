#include "Utility/GlobalValueUpdater.h"
#include "Needs/NeedCold.h"
#include "Needs/NeedHunger.h"
#include "Needs/NeedExhaustion.h"

GlobalUpdater::CachedColdValues       GlobalUpdater::_coldCache{};
GlobalUpdater::CachedExhaustionValues GlobalUpdater::_exhaustionCache{};
GlobalUpdater::CachedHungerValues     GlobalUpdater::_hungerCache{};

std::span<GlobalUpdater::CacheBinding> GlobalUpdater::_coldBindings{};
std::span<GlobalUpdater::CacheBinding> GlobalUpdater::_exhaustionBindings{};
std::span<GlobalUpdater::CacheBinding> GlobalUpdater::_hungerBindings{};

void GlobalUpdater::LoadBindings()
{
    logger::info("Loading global bindings");

    auto util       = Utility::GetSingleton();
    auto cold       = NeedCold::GetSingleton();
    auto exhaustion = NeedExhaustion::GetSingleton();
    auto hunger     = NeedHunger::GetSingleton();

    //Caching like this is okay for global pointers. Never do this for pointers that are not consistant

    static CacheBinding coldBindings[] = {
        MakeBinding(&cold->NeedStage1->value, &_coldCache.cachedColdStage1),
        MakeBinding(&cold->NeedStage2->value, &_coldCache.cachedColdStage2),
        MakeBinding(&cold->NeedStage3->value, &_coldCache.cachedColdStage3),
        MakeBinding(&cold->NeedStage4->value, &_coldCache.cachedColdStage4),
        MakeBinding(&cold->NeedStage5->value, &_coldCache.cachedColdStage5),
        MakeBinding(&cold->NeedMaxValue->value, &_coldCache.cachedColdMax),
        MakeBoolBinding(&util->SMI_ColdShouldBeEnabled->value, &_coldCache.cachedColdShouldBeEnabled),
        MakeBoolBinding(&cold->NeedAvPenDisabled->value, &_coldCache.cachedColdAVPenDisabled),
        MakeBinding(&cold->Survival_ColdResistMaxValue->value, &_coldCache.cachedColdResistMax),
        MakeBinding(&cold->Survival_AfflictionColdChance->value, &_coldCache.cachedColdAfflictionChance),
        MakeBinding(&cold->SMI_ColdRate->value, &_coldCache.cachedColdRate),
    };

    _coldBindings = coldBindings;
    logger::info("Loaded cold bindings");


    static CacheBinding exhaustionBindings[] = {
        MakeBinding(&exhaustion->NeedStage1->value, &_exhaustionCache.cachedExhaustionStage1),
        MakeBinding(&exhaustion->NeedStage2->value, &_exhaustionCache.cachedExhaustionStage2),
        MakeBinding(&exhaustion->NeedStage3->value, &_exhaustionCache.cachedExhaustionStage3),
        MakeBinding(&exhaustion->NeedStage4->value, &_exhaustionCache.cachedExhaustionStage4),
        MakeBinding(&exhaustion->NeedStage5->value, &_exhaustionCache.cachedExhaustionStage5),
        MakeBinding(&exhaustion->NeedMaxValue->value, &_exhaustionCache.cachedExhaustionMax),
        MakeBoolBinding(&util->SMI_ExhaustionShouldBeEnabled->value, &_exhaustionCache.cachedExhaustionShouldBeEnabled),
        MakeBoolBinding(&exhaustion->NeedAvPenDisabled->value, &_exhaustionCache.cachedExhaustionAVPenDisabled),
        MakeBinding(&exhaustion->Survival_AfflictionExhaustionChance->value, &_exhaustionCache.cachedExhaustionAfflictionChance),
        MakeBinding(&exhaustion->Survival_ExhaustionRestorePerHour->value, &_exhaustionCache.cachedExhaustionRestorePerHour),
        MakeBinding(&exhaustion->NeedRate->value, &_exhaustionCache.cachedExhaustionRate),
        MakeBinding(&exhaustion->SMI_VampireExhaustionMult->value, &_exhaustionCache.cachedVampireExhaustionRateMult),
        MakeBinding(&exhaustion->SMI_WerewolfExhaustionMult->value, &_exhaustionCache.cachedWerewolfExhaustionRateMult),
    };

    _exhaustionBindings = exhaustionBindings;
    logger::info("Loaded exhaustion bindings");

    static CacheBinding hungerBindings[] = {
        MakeBinding(&hunger->NeedStage1->value, &_hungerCache.cachedHungerStage1),
        MakeBinding(&hunger->NeedStage2->value, &_hungerCache.cachedHungerStage2),
        MakeBinding(&hunger->NeedStage3->value, &_hungerCache.cachedHungerStage3),
        MakeBinding(&hunger->NeedStage4->value, &_hungerCache.cachedHungerStage4),
        MakeBinding(&hunger->NeedStage5->value, &_hungerCache.cachedHungerStage5),
        MakeBinding(&hunger->NeedStage5->value, &_hungerCache.cachedHungerStage5),
        MakeBinding(&hunger->NeedMaxValue->value, &_hungerCache.cachedHungerMax),
        MakeBoolBinding(&util->SMI_HungerShouldBeEnabled->value, &_hungerCache.cachedHungerShouldBeEnabled),
        MakeBoolBinding(&hunger->NeedAvPenDisabled->value, &_hungerCache.cachedHungerAVPenDisabled),
        MakeBinding(&hunger->Survival_AfflictionHungerChance->value, &_hungerCache.cachedHungerAfflictionChance),
        MakeBinding(&hunger->NeedRate->value, &_hungerCache.cachedHungerRate),
        MakeBinding(&hunger->SMI_VampireHungerMult->value, &_hungerCache.cachedVampireHungerRateMult),
        MakeBinding(&hunger->SMI_WerewolfHungerMult->value, &_hungerCache.cachedWerewolfHungerRateMult),
        MakeBinding(&hunger->Survival_HungerRestoreVerySmallAmount->value, &_hungerCache.cachedHungerRestoreVerySmall),
        MakeBinding(&hunger->Survival_HungerRestoreSmallAmount->value, &_hungerCache.cachedHungerRestoreSmall),
        MakeBinding(&hunger->Survival_HungerRestoreMediumAmount->value, &_hungerCache.cachedHungerRestoreMedium),
        MakeBinding(&hunger->Survival_HungerRestoreLargeAmount->value, &_hungerCache.cachedHungerRestorelarge),
    };

    _hungerBindings = hungerBindings;
    logger::info("Loaded hunger bindings");
}

void GlobalUpdater::CacheGlobals()
{
    Cache(_coldBindings);
    Cache(_exhaustionBindings);
    Cache(_hungerBindings);
}

void GlobalUpdater::RefreshGlobals()
{
    Restore(_coldBindings);
    Restore(_exhaustionBindings);
    Restore(_hungerBindings);
}

void GlobalUpdater::Cache(std::span<const CacheBinding> bindings)
{
    for (const auto& b : bindings)
        b.cache(b.live, b.cached);
}

void GlobalUpdater::Restore(std::span<const CacheBinding> bindings)
{
    for (const auto& b : bindings)
        b.restore(b.live, b.cached);
}
