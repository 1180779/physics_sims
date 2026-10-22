/*
 * Created by Radosław Głasek on 22.10.2026
 * radoslaw.glasek@theworldlooksback.dev
 */

#ifndef PHYSICS_SIMS_HEADER_SRC_SPRING_SRC_PANEL_CONFIG_HXX
#define PHYSICS_SIMS_HEADER_SRC_SPRING_SRC_PANEL_CONFIG_HXX

#include <QString>

#include "cad_common/panels/PanelSection.hxx"
#include "cad_common/panels/SectionHost.hxx"

#include "../Helpers.hxx"
#include "../SpringPanel.hxx"

struct PanelConfig {
    const QString id;
    const QString displayName;
    SpringPanel* const panel;
    const SectionSide sectionSide;
    bool open = false;
};

using PanelsConfigs = std::vector<PanelConfig>;

/// @brief 
struct ConstructedPanelsConfigs {
    struct ConstructedPanelConfig {
        const QString stringId;
        const QString displayName;
        const PanelId id;
        SpringPanel* const panel;
        const SectionSide sectionSide;
        bool open;
    };

    explicit ConstructedPanelsConfigs(const PanelsConfigs& panelsConfigs);
    void addAndWirePanelActions(QMenu* panelsMenu, const SectionHost* host);
    void openPanels(const SectionHost* host) const;

    std::vector<ConstructedPanelConfig> panelsConfigs;
};


#endif // PHYSICS_SIMS_HEADER_SRC_SPRING_SRC_PANEL_CONFIG_HXX