/*
 * Created by Radosław Głasek on 22.10.2026
 * radoslaw.glasek@theworldlooksback.dev
 */

#include "SectionSet.hxx"

std::array<PanelSection*, 4> SectionSet::asArray() const {
    return {left, right, top, bottom};
}

PanelSection* SectionSet::fromSide(const SectionSide side) const {
    switch (side) {
    case SectionSide::Left:
        return left;
    case SectionSide::Right:
        return right;
    case SectionSide::Top:
        return top;
    case SectionSide::Bottom:
        return bottom;
    }
    return top;
}

SectionSet SectionSet::of(const SectionHost* host) {
    return {
        .left = host->section(SectionSide::Left),
        .right = host->section(SectionSide::Right),
        .top = host->section(SectionSide::Top),
        .bottom = host->section(SectionSide::Bottom),
    };
}