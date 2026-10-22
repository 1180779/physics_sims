/*
 * Created by Radosław Głasek on 22.10.2026
 * radoslaw.glasek@theworldlooksback.dev
 */

#include "Helpers.hxx"

void CloseAllPanels(const SectionHost* const host) {
    for (const auto section : SectionSet::of(host).asArray()) {
        section->closeAllPanels();
    }
}

QAction* addPanelAction(QMenu* menu, const QString& text) {
    QAction* action = menu->addAction(text);
    action->setCheckable(true);
    return action;
}

void wirePanelToggle(QAction* action, const SectionHost* host, const PanelId& id) {
    const SectionSet sections = SectionSet::of(host);

    const auto isOpenAnywhere = [sections, id] {
        return std::ranges::any_of(sections.asArray(), [id](auto* section) {
            return section->isOpen(id);
        });
    };

    QObject::connect(action, &QAction::triggered, host, [sections, id](const bool checked) {
        if (checked) {
            for (PanelSection* section : sections.asArray()) {
                if (section->hasPanel(id)) {
                    section->openPanel(id);
                    return;
                }
            }
        } else {
            for (PanelSection* section : sections.asArray()) {
                if (section->isOpen(id)) {
                    section->closePanel(id);
                }
            }
        }
    });

    const auto refresh = [action, isOpenAnywhere] {
        const bool open = isOpenAnywhere();
        if (action->isChecked() == open) {
            return;
        }
        const QSignalBlocker blocker(action);
        action->setChecked(open);
    };

    for (const PanelSection* section : {sections.left, sections.right, sections.top, sections.bottom}) {
        QObject::connect(section, &PanelSection::panelOpened, action, [refresh](const PanelId&) { refresh(); });
        QObject::connect(section, &PanelSection::panelClosed, action, [refresh](const PanelId&) { refresh(); });
    }
    refresh();
}