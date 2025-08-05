#ifndef AUTOMOTIVE_HMI_HPP
#define AUTOMOTIVE_HMI_HPP

#include <iostream>
#include <map>
#include <set>
#include <stack>
#include <queue>
#include <string>
#include <vector>

class AutomotiveHMI {
private:
    std::map<std::string, int> gaugeValues; // Stores current gauge values
    std::set<std::string> availableGauges = {"Speed", "RPM", "Fuel"}; // List of valid gauges
    std::multiset<std::string> updateHistory; // Tracks updates
    std::multimap<std::string, std::string> gaugeCategories = {
        {"Speed", "Performance"},
        {"RPM", "Performance"},
        {"Fuel", "Fuel System"}
    }; // Gauge mapping
    std::stack<std::pair<std::string, int>> undoStack; // Undo last update
    std::queue<std::pair<std::string, int>> updateQueue; // Buffered updates
    std::priority_queue<std::pair<int, std::string>> alerts; // Urgent alerts

public:
    void updateGauge(const std::string& name, int value);
    void undoLastUpdate();
    void processQueue();
    void displayUrgentAlerts();
    void showAllGauges();
    void logUpdate(const std::string& name);
    void addAlert(int priority, const std::string& message);
};

#endif // AUTOMOTIVE_HMI_HPP



#include "AutomotiveHMI.hpp"

void AutomotiveHMI::updateGauge(const std::string& name, int value) {
    if (availableGauges.find(name) == availableGauges.end()) {
        std::cout << "Invalid gauge name!\n";
        return;
    }
    
    if (gaugeValues.find(name) != gaugeValues.end()) {
        undoStack.push({name, gaugeValues[name]}); // Save previous value for undo
    }

    gaugeValues[name] = value;
    updateHistory.insert(name); // Track update history
    std::cout << name << " updated to " << value << "\n";
}

void AutomotiveHMI::undoLastUpdate() {
    if (!undoStack.empty()) {
        auto lastUpdate = undoStack.top();
        undoStack.pop();
        gaugeValues[lastUpdate.first] = lastUpdate.second;
        std::cout << "Undo last update: " << lastUpdate.first << " reverted to " << lastUpdate.second << "\n";
    } else {
        std::cout << "No updates to undo.\n";
    }
}

void AutomotiveHMI::processQueue() {
    while (!updateQueue.empty()) {
        auto update = updateQueue.front();
        updateQueue.pop();
        updateGauge(update.first, update.second);
    }
}

void AutomotiveHMI::displayUrgentAlerts() {
    while (!alerts.empty()) {
        auto alert = alerts.top();
        alerts.pop();
        std::cout << "Priority: " << alert.first << " - Alert: " << alert.second << "\n";
    }
}

void AutomotiveHMI::showAllGauges() {
    std::cout << "Current Gauge Readings:\n";
    for (const auto& gauge : gaugeValues) {
        std::cout << gauge.first << ": " << gauge.second << "\n";
    }
}

void AutomotiveHMI::logUpdate(const std::string& name) {
    updateHistory.insert(name);
    std::cout << "Logged update for: " << name << "\n";
}

void AutomotiveHMI::addAlert(int priority, const std::string& message) {
    alerts.push({priority, message});
    std::cout << "Alert added: " << message << " (Priority " << priority << ")\n";
}


#include "AutomotiveHMI.hpp"

int main() {
    AutomotiveHMI hmi;

    // Initial updates
    hmi.updateGauge("Speed", 80);
    hmi.updateGauge("RPM", 3000);
    hmi.updateGauge("Fuel", 45);
    
    hmi.showAllGauges();

    // Undo last update
    hmi.undoLastUpdate();
    hmi.showAllGauges();

    // Queue updates
    hmi.updateQueue.push({"Speed", 90});
    hmi.updateQueue.push({"Fuel", 40});
    hmi.processQueue();

    // Log updates
    hmi.logUpdate("Speed");
    hmi.logUpdate("Fuel");

    // Add alerts
    hmi.addAlert(1, "Engine Overheat");
    hmi.addAlert(3, "Fuel Level Critical");
    hmi.displayUrgentAlerts();

    return 0;
}