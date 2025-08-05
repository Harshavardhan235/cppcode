// widget_manager.h
#ifndef WIDGET_MANAGER_H
#define WIDGET_MANAGER_H
#include <memory>
#include <vector>
#include <string>
class Widget {
public:
    virtual void draw() const = 0;
    virtual ~Widget();
};

class Button : public Widget {
public:
    void draw() const override;
    ~Button();
};
class Label : public Widget {
public:
    void draw() const override;
    ~Label();
};
enum class WidgetType {
    BUTTON,
    LABEL
};
// Factory Function
std::unique_ptr<Widget> createWidget(WidgetType type);
// HMI Widget Manager
class HMIWidgetManager {
public:
    void addWidget(std::unique_ptr<Widget> widget);
    std::shared_ptr<Widget> getWidget(size_t index);
    void drawAllWidgets() const;
private:
    std::vector<std::shared_ptr<Widget>> widgets;
};
// Global configuration variable
extern int widgetConfigValue;
#endif // WIDGET_MANAGER_H
