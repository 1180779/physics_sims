/*
 * Created by Radosław Głasek on 22.10.2026
 * radoslaw.glasek@theworldlooksback.dev
 */

#ifndef PHYSICS_SIMS_HEADER_SRC_SPRING_SRC_SPRING_PANEL_HXX
#define PHYSICS_SIMS_HEADER_SRC_SPRING_SRC_SPRING_PANEL_HXX

#include "cad_common/panels/PanelWidget.hxx"

#include <QLabel>

/// @brief Placeholder panel content; replace with the real simulation widgets
class SpringPanel final : public PanelWidget {
public:
    SpringPanel(std::string title, const QString& body)
        : PanelWidget(std::move(title)) {
        auto* layout = createLayout();
        layout->addWidget(new QLabel(body));
        layout->addStretch();
    }
};

#endif // PHYSICS_SIMS_HEADER_SRC_SPRING_SRC_SPRING_PANEL_HXX
