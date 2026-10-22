/*
 * Created by Radosław Głasek on 22.10.2026
 * radoslaw.glasek@theworldlooksback.dev
 */

#ifndef PHYSICS_SIMS_HEADER_SRC_SPRING_SRC_SECTION_SET_HXX
#define PHYSICS_SIMS_HEADER_SRC_SPRING_SRC_SECTION_SET_HXX

#include "cad_common/panels/PanelSection.hxx"
#include "cad_common/panels/SectionHost.hxx"

/// @brief The four sections of @p host, in a fixed order
struct SectionSet {
    PanelSection* left;
    PanelSection* right;
    PanelSection* top;
    PanelSection* bottom;
    
    [[nodiscard]] std::array<PanelSection*, 4> asArray() const;
    [[nodiscard]] PanelSection* fromSide(SectionSide side) const;
    
    static SectionSet of(const SectionHost* host);
};

#endif // PHYSICS_SIMS_HEADER_SRC_SPRING_SRC_SECTION_SET_HXX