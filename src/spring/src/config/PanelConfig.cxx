/*
 * Created by Radosław Głasek on 22.10.2026
 * radoslaw.glasek@theworldlooksback.dev
 */

#include "PanelConfig.hxx"

#include "../SectionSet.hxx"

#include <ranges>

ConstructedPanelsConfigs::ConstructedPanelsConfigs(const PanelsConfigs& panelsConfigs) {
    for (const auto& [stringId, displayName, panel, sectionSide, open] : panelsConfigs) {
        const PanelId constructedId{stringId.toStdString()};
        this->panelsConfigs.emplace_back(ConstructedPanelConfig{
            .stringId = stringId,
            .displayName = displayName,
            .id = PanelId(stringId.toStdString()),
            .panel = panel,
            .sectionSide = sectionSide,
            .open = open,
        });
    };
}

void ConstructedPanelsConfigs::addAndWirePanelActions(QMenu* const panelsMenu, const SectionHost* const host) {
    const auto sections = SectionSet::of(host);
    for (const auto& [stringId, displayName, id, panel, sectionSide, open] : panelsConfigs) {
        sections.fromSide(sectionSide)->addPanel(id, panel);
        const auto action = addPanelAction(panelsMenu, displayName);
        wirePanelToggle(action, host, id);
    };
}

void ConstructedPanelsConfigs::openPanels(const SectionHost* host) const {
    // close all panels first to have consistent behaviour
    CloseAllPanels(host);
    const auto sections = SectionSet::of(host);
    auto openPanels = panelsConfigs | std::ranges::views::as_const | std::ranges::views::filter(
                          [](auto& config) {
                              return config.open;
                          });
    for (const auto& [stringId, displayName, id, panel, sectionSide, open] : openPanels) {
        const auto section = sections.fromSide(sectionSide);
        section->openPanel(id);
    }
}