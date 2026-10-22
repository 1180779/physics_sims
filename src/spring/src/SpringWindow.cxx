#include "SpringWindow.hxx"

#include "SpringViewport.hxx"
#include "config/SectionConfig.hxx"

#include <QAction>
#include <QLabel>
#include <QMenuBar>
#include <QSignalBlocker>
#include <QWidget>

#include <cad_common/panels/PanelWidget.hxx>
#include <cad_common/panels/SectionHost.hxx>
#include <cad_common/style/Style.hxx>
#include <cad_common/window/TitleBar.hxx>

SpringWindow::SpringWindow(const PanelsConfigs& panelsConfigs, const SectionsConfigs& sectionsConfigs, QWidget* parent)
    : QWidget(parent) {
    setWindowFlag(Qt::FramelessWindowHint);
    setAutoFillBackground(true);

    const auto glViewport = new SpringViewport{};
    
    auto* viewport = new QFrame;
    viewport->setObjectName(QStringLiteral("centralViewport"));
    viewport->setFrameShape(QFrame::NoFrame);
    viewport->setAttribute(Qt::WA_StyledBackground, true);
    viewport->setMinimumSize(400, 300);
    auto* viewportLayout = new QVBoxLayout(viewport);
    viewportLayout->addWidget(glViewport);

    m_host = new SectionHost(viewport);
    m_host->setCornerOwner(CornerOwner::HorizontalSides);
    sectionsConfigs.apply(m_host);

    ConstructedPanelsConfigs configs(panelsConfigs);

    constexpr int resizeMargin = 6;
    auto* outerLayout = new QVBoxLayout(this);
    outerLayout->setContentsMargins(resizeMargin, resizeMargin, resizeMargin, resizeMargin);
    outerLayout->setSpacing(resizeMargin);

    auto* titleBar = new TitleBar(this);
    QMenuBar* menuBar = titleBar->menuBar();

    QMenu* panelsMenu = menuBar->addMenu(QStringLiteral("Panels"));
    configs.addAndWirePanelActions(panelsMenu, m_host);
    configs.openPanels(m_host);

    QMenu* viewMenu = menuBar->addMenu(QStringLiteral("View"));
    QAction* darkAction = viewMenu->addAction(QStringLiteral("Dark mode"));
    darkAction->setCheckable(true);
    darkAction->setChecked(palette().color(QPalette::Window).lightness() < 128);

    QMenu* fileMenu = menuBar->addMenu(QStringLiteral("File"));
    const QAction* quitAction = fileMenu->addAction(QStringLiteral("Quit"));

    outerLayout->addWidget(titleBar);
    outerLayout->addWidget(m_host, 1);
    enableFramelessResize(this, resizeMargin);

    connect(darkAction, &QAction::triggered, this, [](const bool dark) {
        cad_common::style::apply(dark ? cad_common::StyleMode::Dark : cad_common::StyleMode::Light);
    });
    connect(quitAction, &QAction::triggered, this, [this] { close(); });

    // swallow drops outside the section host (menu bar) so Qt does not warn
    setAcceptDrops(true);
}