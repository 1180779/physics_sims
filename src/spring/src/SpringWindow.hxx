#ifndef SPRING_WINDOW_HXX
#define SPRING_WINDOW_HXX

#include "cad_common/panels/SectionHost.hxx"
#include "config/PanelConfig.hxx"
#include "config/SectionConfig.hxx"

#include <QWidget>

/// @brief Main application window: a toolbar over a SectionHost that holds the
/// spring viewport in the centre and dockable parameter/plot sections around it
///
/// The panels are placeholders for now; the simulation widgets plug into the
/// sections without touching the window layout
class SpringWindow final : public QWidget {
    Q_OBJECT

public:
    explicit SpringWindow(const PanelsConfigs& panelsConfigs, const SectionsConfigs& sectionsConfigs, QWidget* parent = nullptr);

private:
    SectionHost* m_host = nullptr;
};

#endif // SPRING_WINDOW_HXX