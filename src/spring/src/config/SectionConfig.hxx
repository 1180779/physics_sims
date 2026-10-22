/*
 * Created by Radosław Głasek on 22.10.2026
 * radoslaw.glasek@theworldlooksback.dev
 */

#ifndef PHYSICS_SIMS_HEADER_SRC_SPRING_SRC_SECTION_CONFIG_HXX
#define PHYSICS_SIMS_HEADER_SRC_SPRING_SRC_SECTION_CONFIG_HXX

#include "cad_common/panels/PanelSection.hxx"
#include "cad_common/panels/SectionHost.hxx"

struct SectionConfig {
    SectionMode sectionMode = SectionMode::Exclusive;
    int maximumOpenPanels = 1;
};

struct SectionsConfigs {
    SectionsConfigs(std::map<SectionSide, SectionConfig> configs) : configs(std::move(configs)) {
    }

    SectionsConfigs(std::initializer_list<std::pair<const SectionSide, SectionConfig>>&& configs) : configs(
        std::map{configs}) {
    }


    std::map<SectionSide, SectionConfig> configs;

    void apply(const SectionHost* host) const;
};

#endif // PHYSICS_SIMS_HEADER_SRC_SPRING_SRC_SECTION_CONFIG_HXX