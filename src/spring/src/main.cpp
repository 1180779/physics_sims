#include <QApplication>
#include <QTimer>

#include <cad_common/style/Font.hxx>
#include <cad_common/style/Style.hxx>

#include "SpringWindow.hxx"

int main(int argc, char* argv[]) {
    const QApplication app(argc, argv);
    const QStringList args = QApplication::arguments();
    const bool forceDark = args.contains(QStringLiteral("--dark"));
    cad_common::style::apply(forceDark ? cad_common::StyleMode::Dark : cad_common::StyleMode::System);
    cad_common::applyAppFont();

    const PanelsConfigs configs = {
        PanelConfig{
            .id = "parameters",
            .displayName = "Parameters",
            .panel = new SpringPanel("Parameters", "m, c, k"),
            .sectionSide = SectionSide::Left,
            .open = true
        },
        PanelConfig{
            .id = "controls",
            .displayName = "Initial conditions",
            .panel = new SpringPanel("Initial conditions", "x(0), v(0), integration step"),
            .sectionSide = SectionSide::Left,
            .open = true
        },
        PanelConfig{
            .id = "plots",
            .displayName = "Plots",
            .panel = new SpringPanel("Plots", "x(t), v(t), a(t)"),
            .sectionSide = SectionSide::Right,
            .open = true
        },
        PanelConfig{
            .id = "forces",
            .displayName = "Forces",
            .panel = new SpringPanel("Forces", "f(t), g(t), h(t)"),
            .sectionSide = SectionSide::Top,
            .open = false
        },
        PanelConfig{
            .id = "state",
            .displayName = "State",
            .panel = new SpringPanel("State", "x(t), v(t), a(t)"),
            .sectionSide = SectionSide::Bottom,
            .open = true
        }
    };
    const SectionsConfigs sectionsConfigs = {
        {SectionSide::Left, SectionConfig{
             .sectionMode = SectionMode::Split,
             .maximumOpenPanels = 2,
         }}
    };

    SpringWindow window{configs, sectionsConfigs};
    window.resize(1100, 700);
    window.show();
    return QApplication::exec();
}