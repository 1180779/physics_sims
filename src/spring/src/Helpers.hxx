/*
 * Created by Radosław Głasek on 22.10.2026
 * radoslaw.glasek@theworldlooksback.dev
 */

#ifndef PHYSICS_SIMS_HEADER_SRC_SPRING_SRC_HELPERS_HXX
#define PHYSICS_SIMS_HEADER_SRC_SPRING_SRC_HELPERS_HXX

#include "SectionSet.hxx"

#include <QAction>
#include <QMenu>

void CloseAllPanels(const SectionHost* host);

QAction* addPanelAction(QMenu* menu, const QString& text);

/// @brief Keep @p action checked exactly while @p id is open in any section
/// of @p host; triggering opens/closes the panel wherever it is registered
void wirePanelToggle(QAction* action, const SectionHost* host, const PanelId& id);


#endif // PHYSICS_SIMS_HEADER_SRC_SPRING_SRC_HELPERS_HXX