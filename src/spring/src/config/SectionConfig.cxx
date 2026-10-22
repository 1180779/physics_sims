/*
 * Created by Radosław Głasek on 22.10.2026
 * radoslaw.glasek@theworldlooksback.dev
 */

#include "SectionConfig.hxx"

#include "../SectionSet.hxx"

void SectionsConfigs::apply(const SectionHost* host) const {
    const auto sections = SectionSet::of(host);
    for (const auto [side, config] : configs) {
        const auto section = sections.fromSide(side);
        section->setMode(config.sectionMode);
        section->setMaximumOpenPanels(config.maximumOpenPanels);
    }
}