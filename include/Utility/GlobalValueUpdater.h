#pragma once

class GlobalUpdater
{

public:
    struct CacheBinding
    {
        void (*cache)(void* live, void* cached);
        void (*restore)(void* live, void* cached);
        void* live;
        void* cached;
    };

    struct CachedColdValues
    {
        float cachedColdStage1;
        float cachedColdStage2;
        float cachedColdStage3;
        float cachedColdStage4;
        float cachedColdStage5;
        float cachedColdMax;
        float cachedColdResistMax;
        bool  cachedColdShouldBeEnabled;
        bool  cachedColdAVPenDisabled;
        float cachedColdAfflictionChance;
        float cachedColdRate;
    };

    struct CachedExhaustionValues
    {
        float cachedExhaustionStage1;
        float cachedExhaustionStage2;
        float cachedExhaustionStage3;
        float cachedExhaustionStage4;
        float cachedExhaustionStage5;
        float cachedExhaustionMax;
        bool  cachedExhaustionShouldBeEnabled;
        bool  cachedExhaustionAVPenDisabled;
        float cachedExhaustionAfflictionChance;
        float cachedExhaustionRestorePerHour;
        float cachedExhaustionRate;
        float cachedVampireExhaustionRateMult;
        float cachedWerewolfExhaustionRateMult;
    };

    struct CachedHungerValues
    {
        float cachedHungerStage1;
        float cachedHungerStage2;
        float cachedHungerStage3;
        float cachedHungerStage4;
        float cachedHungerStage5;
        float cachedHungerMax;
        bool cachedHungerShouldBeEnabled;
        bool cachedHungerAVPenDisabled;
        float cachedHungerAfflictionChance;
        float cachedHungerRate;
        float cachedVampireHungerRateMult;
        float cachedWerewolfHungerRateMult;
        float cachedHungerRestoreVerySmall;
        float cachedHungerRestoreSmall;
        float cachedHungerRestoreMedium;
        float cachedHungerRestorelarge;
    };

    //Bindings must be loaded before calling cache or refresh
    static void LoadBindings();

    static void RefreshGlobals();
    static void CacheGlobals();


    template <typename T>
    static constexpr CacheBinding MakeBinding(T* live, T* cached)
    {
        return CacheBinding{
            [](void* l, void* c)
            {
                *static_cast<T*>(c) = *static_cast<T*>(l);
            },                      
            [](void* l, void* c)
            {
                *static_cast<T*>(l) = *static_cast<T*>(c);
            }, live, cached
        };
    }

    static constexpr CacheBinding MakeBoolBinding(float* live, bool* cached)
    {
        return CacheBinding
        {
            [](void* l, void* c)
            {
                *static_cast<bool*>(c)  = (*static_cast<float*>(l) != 0.0f);
            },                 
            [](void* l, void* c)
            {
                *static_cast<float*>(l) = *static_cast<bool*>(c) ? 1.0f : 0.0f;
            }, live, cached
        };
    }

    static void Cache(std::span<const CacheBinding> bindings);

    static void Restore(std::span<const CacheBinding> bindings);

    static CachedColdValues _coldCache;
    static CachedExhaustionValues _exhaustionCache;
    static CachedHungerValues     _hungerCache;

    static std::span<CacheBinding> _coldBindings;
    static std::span<CacheBinding> _exhaustionBindings;
    static std::span<CacheBinding> _hungerBindings;
};


