// widget_manager.cpp
#include "widget_manager.h"
#include <iostream>
// Global config variable definition
int widgetConfigValue = 42;
// --- Widget Base ---
Widget::~Widget() {
    std::cout << "Widget destroyed\n";
}
// --- Button ---
void Button::draw() const {
    std::cout << "Drawing Button\n";
}
Button::~Button() {
    std::cout << "Button destroyed\n";
}
// --- Label ---
void Label::draw() const {
    std::cout << "Drawing Label\n";
}
Label::~Label() {
    std::cout << "Label destroyed\n";
}
// --- Factory Function ---
std::unique_ptr<Widget> createWidget(WidgetType type) {
    // static local variable (used only once and remembered)
    static int creationCounter = 0;
    creationCounter++;
    std::cout << "Widgets created so far: " << creationCounter << "\n";
    switch (type) {
        case WidgetType::BUTTON:
            return std::make_unique<Button>();
        case WidgetType::LABEL:
            return std::make_unique<Label>();
        default:
            return nullptr;
    }
}
// --- HMI Widget Manager ---
void HMIWidgetManager::addWidget(std::unique_ptr<Widget> widget) {
    auto sharedWidget = std::shared_ptr<Widget>(std::move(widget));
    widgets.push_back(sharedWidget);
}
std::shared_ptr<Widget> HMIWidgetManager::getWidget(size_t index) {
    if (index < widgets.size()) {
        return widgets[index];
    }
    return nullptr;
}
void HMIWidgetManager::drawAllWidgets() const {
    for (const auto& widget : widgets) {
        widget->draw();
    }
}
