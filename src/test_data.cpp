#pragma once

#include <iostream>
#include <include.hpp>


SpireSim::UCombatTemplate createTestData1(SpireSim::PoolContainer &container) {
    auto t = std::make_unique<SpireSim::CombatTemplateFixed>(container);
    t->add({
        {SpireSim::CardId::BloodLetting, true},
        {SpireSim::CardId::AstralPulse, true},
        {SpireSim::CardId::Strike, true},
        // {SpireSim::CardId::Strike, true},
        // {SpireSim::CardId::Strike, true},
        {SpireSim::CardId::Strike, true},
        {SpireSim::CardId::Defend, true},
        {SpireSim::CardId::Defend, true},
        {SpireSim::CardId::Defend, true},
        {SpireSim::CardId::FallingStar, true},
        {SpireSim::CardId::Venerate, true},
    })
    .set(SpireSim::PlayerId::Regent)
    .set(SpireSim::EncounterId::Nibbits)
    .set({SpireSim::RelicId::BagOfPreparation, SpireSim::RelicId::CentennialPuzzle})
    .set({SpireSim::PotionId::FirePotion, SpireSim::PotionId::FlexPotion})
    ;
    return t;
}

SpireSim::UCombatTemplate createTestData2(SpireSim::PoolContainer &container) {
    auto t = std::make_unique<SpireSim::CombatTemplateFixed>(container);
    t->add({
        {SpireSim::CardId::Hologram, true},
        {SpireSim::CardId::Claw, true},
        {SpireSim::CardId::Strike, true},
        {SpireSim::CardId::Strike, true},
        {SpireSim::CardId::Strike, true},
        {SpireSim::CardId::Strike, true},
        {SpireSim::CardId::Defend, true},
        {SpireSim::CardId::Defend, true},
        {SpireSim::CardId::Defend, true},
        {SpireSim::CardId::Bash, true},
        {SpireSim::CardId::Thunderclap, true},
    })
    .set(SpireSim::PlayerId::IronClad)
    .set(SpireSim::EncounterId::SF_Test)
    .set({SpireSim::RelicId::BagOfPreparation})
    .set({SpireSim::PotionId::FirePotion, SpireSim::PotionId::FlexPotion})
    ;
    return t;
}

SpireSim::UCombatTemplate createTestData3(SpireSim::PoolContainer &container) {
    auto t = std::make_unique<SpireSim::CombatTemplateRandom>(container);
    t->
     set({SpireSim::PlayerId::IronClad, SpireSim::PlayerId::Regent})
    .set({SpireSim::EncounterId::SF_Test, SpireSim::EncounterId::Nibbits})
    .set({SpireSim::RelicId::BagOfPreparation, SpireSim::RelicId::CentennialPuzzle})
    .set({SpireSim::PotionId::FirePotion, SpireSim::PotionId::FlexPotion})
    .add({
        {SpireSim::CardId::Hologram, true},
        {SpireSim::CardId::Strike, true},
        {SpireSim::CardId::Strike, true},
        {SpireSim::CardId::Strike, true},
        {SpireSim::CardId::Strike, true},
        {SpireSim::CardId::Defend, true},
        {SpireSim::CardId::Defend, true},
        {SpireSim::CardId::Defend, true},
        {SpireSim::CardId::Bash, true},
        {SpireSim::CardId::Thunderclap, true},
        {SpireSim::CardId::Venerate, true},
        {SpireSim::CardId::FallingStar, true},
    })
    ;
    return t;
}

SpireSim::UCombatTemplates createTestData(SpireSim::PoolContainer &container) {
    SpireSim::UCombatTemplates vec;
    vec.push_back(std::move(createTestData1(container)));
    // vec.push_back(std::move(createTestData2(container)));
    // vec.push_back(std::move(createTestData3(container)));
    return vec;
}

