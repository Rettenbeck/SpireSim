#include <iostream>
#include <include.hpp>

SpireSim::UCombatTemplate createTestData1(SpireSim::PoolContainer &container) {
    auto t = std::make_unique<SpireSim::CombatTemplateFixed>(container);
    t->add({
        {SpireSim::CardId::BloodLetting, true},
        {SpireSim::CardId::Strike, true},
        {SpireSim::CardId::Strike, true},
        {SpireSim::CardId::Strike, true},
        {SpireSim::CardId::Strike, true},
        {SpireSim::CardId::Strike, true},
        {SpireSim::CardId::Defend, true},
        {SpireSim::CardId::Defend, true},
        {SpireSim::CardId::Defend, true},
        {SpireSim::CardId::FallingStar, true},
        {SpireSim::CardId::Venerate, true},
    })
    .set(SpireSim::PlayerId::IronClad)
    .set(SpireSim::EncounterId::Nibbits)
    .set({SpireSim::RelicId::BagOfPreparation, SpireSim::RelicId::CentennialPuzzle})
    .set({SpireSim::PotionId::FirePotion, SpireSim::PotionId::FlexPotion})
    ;
    return t;
}

SpireSim::UCombatTemplates createTestData(SpireSim::PoolContainer &container) {
    SpireSim::UCombatTemplates vec;
    vec.push_back(std::move(createTestData1(container)));
    return vec;
}

