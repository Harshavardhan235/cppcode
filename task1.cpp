#include <iostream>
#include <vector>
#include <deque>
#include <list>

class DashboardManager {
private:
    // Gauge & Telltale Preset List
    std::vector<std::string> presets = {
        "Tachometer: Sport",
        "Speedometer: Minimal",
        "Battery Warning: On",
        "Oil Pressure: Bar",
        "Brake Warning: Flash"
    };

    // User Input Event Queue
    std::deque<std::string> eventQueue;

    // Action History with Batch Replay
    std::list<std::string> actionHistory;

public:
    // Add a new preset
    void addPreset(const std::string& preset) {
        presets.push_back(preset);
    }

    // Display all presets
    void showPresets() const {
        std::cout << "Preset List:\n";
        for (const auto& preset : presets) {
            std::cout << "- " << preset << "\n";
        }
    }

    // Add an event (Priority events go to the front)
    void addEvent(const std::string& event, bool isPriority = false) {
        if (isPriority)
            eventQueue.push_front(event);
        else
            eventQueue.push_back(event);
    }

    // Process and remove events in FIFO order
    void processEvents() {
        std::cout << "Processing events:\n";
        while (!eventQueue.empty()) {
            std::cout << "- " << eventQueue.front() << "\n";
            eventQueue.pop_front();
        }
    }

    // Add a new action
    void addAction(const std::string& action) {
        actionHistory.push_back(action);
    }

    // Undo the last action
    void undoAction() {
        if (!actionHistory.empty()) {
            std::cout << "Undoing: " << actionHistory.back() << "\n";
            actionHistory.pop_back();
        } else {
            std::cout << "No actions to undo.\n";
        }
    }

    // Display all actions
    void showHistory() const {
        std::cout << "Action History:\n";
        for (const auto& action : actionHistory) {
            std::cout << "- " << action << "\n";
        }
    }

    // Replay last N actions
    void replayLast(int N) {
        std::cout << "Replaying last " << N << " actions:\n";
        auto it = actionHistory.rbegin();
        while (N-- && it != actionHistory.rend()) {
            std::cout << "- Replaying: " << *it << "\n";
            addEvent(*it); // Re-add actions to the event queue
            ++it;
        }
    }
};

int main() {
    DashboardManager manager;

    // Presets
    manager.addPreset("Fuel Gauge: Digital");
    manager.showPresets();

    // User Events
    manager.addEvent("CustomizeGauge");
    manager.addEvent("CriticalBatteryAlert", true); // Priority event
    manager.processEvents();

    // Action History
    manager.addAction("Customize Tachometer Sport");
    manager.addAction("Enable Battery Warning");
    manager.showHistory();
    manager.undoAction();
    manager.showHistory();

    // Replay last action
    manager.replayLast(1);

    return 0;
}