// main.cpp
#include "widget_manager.h"
#include <iostream>
void useSharedWidget(const std::shared_ptr<Widget>& widget) {
    std::cout << "Using shared widget externally\n";
    widget->draw();
}
int main() {
    HMIWidgetManager manager;
    std::cout << "Global config value: " << widgetConfigValue << "\n";
    // Create widgets using auto for temporary storage
    auto button = createWidget(WidgetType::BUTTON);
    auto label = createWidget(WidgetType::LABEL);
    // Add to manager
    manager.addWidget(std::move(button));
    manager.addWidget(std::move(label));
    // Get shared pointer and demonstrate shared usage
    auto sharedWidget = manager.getWidget(0);
    if (sharedWidget) {
        useSharedWidget(sharedWidget);
    }
    // Draw all widgets
    manager.drawAllWidgets();
    // Destructors will be called automatically as objects go out of scope
    return 0;
}











