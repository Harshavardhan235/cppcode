I can help you with that\! Here's the modified C++ code with `using namespace std;` and removed `std::` prefixes:

```cpp
#include <iostream>
#include <memory>
#include <vector>
#include <deque>
#include <list>
#include <string>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <random>
#include <chrono>
#include <algorithm>

using namespace std; // Added this line

// External variable simulation (would normally be in another file)
extern int fanLimit;
int fanLimit = 5;

// Forward declarations
class HVACControl;
class ClimateControlManager;

// Abstract base class for HVAC controls
class HVACControl {
protected:
    static int idCounter;  // Static storage class demonstration
    int controlId;
    string controlName;
    mutable int logCount;  // Mutable for logging in const methods

public:
    HVACControl(const string& name)
        : controlId(++idCounter), controlName(name), logCount(0) {}

    virtual ~HVACControl() = default;

    // Pure virtual functions for polymorphism
    virtual void render() const = 0;
    virtual void updateSettings() = 0;

    // Getter for control name
    const string& getName() const {
        ++logCount;  // Mutable allows modification in const method
        return controlName;
    }

    int getId() const { return controlId; }
};

// Static member definition
int HVACControl::idCounter = 0;

// Temperature Control Screen
class TemperatureControlScreen : public HVACControl {
private:
    int temperature;
    deque<int> temperatureHistory;  // STL container for history
    mutable mutex tempMutex;

    static const int MAX_HISTORY = 10;
    static const int MIN_TEMP = 16;
    static const int MAX_TEMP = 30;

public:
    TemperatureControlScreen() : HVACControl("TemperatureControl"), temperature(22) {}

    void render() const override {
        lock_guard<mutex> lock(tempMutex);
        cout << "[TemperatureControlScreen] Temp: " << temperature << "°C" << endl;
    }

    void updateSettings() override {
        lock_guard<mutex> lock(tempMutex);
        // Add to history
        temperatureHistory.push_back(temperature);
        if (temperatureHistory.size() > MAX_HISTORY) {
            temperatureHistory.pop_front();
        }
    }

    // Setters and getters with validation
    void setTemperature(int temp) {
        lock_guard<mutex> lock(tempMutex);
        if (temp >= MIN_TEMP && temp <= MAX_TEMP) {
            temperature = temp;
        }
    }

    int getTemperature() const {
        lock_guard<mutex> lock(tempMutex);
        return temperature;
    }

    mutex& getTempMutex() const { return tempMutex; }
};

// Fan Speed Control Screen
class FanSpeedControlScreen : public HVACControl {
private:
    int fanLevel;
    mutable mutex fanMutex;

public:
    FanSpeedControlScreen() : HVACControl("FanSpeedControl"), fanLevel(1) {}

    void render() const override {
        lock_guard<mutex> lock(fanMutex);
        cout << "[FanSpeedControlScreen] Fan: Level " << fanLevel << endl;
    }

    void updateSettings() override {
        lock_guard<mutex> lock(fanMutex);
        // Fan speed update logic here
    }

    void setFanLevel(int level) {
        lock_guard<mutex> lock(fanMutex);
        if (level >= 0 && level <= fanLimit) {  // Using extern variable
            fanLevel = level;
        }
    }

    int getFanLevel() const {
        lock_guard<mutex> lock(fanMutex);
        return fanLevel;
    }

    mutex& getFanMutex() const { return fanMutex; }
};

// Mode Control Screen
class ModeControlScreen : public HVACControl {
private:
    enum class Mode { AC, HEATER, AUTO };
    Mode currentMode;
    list<string> modeHistory;  // STL container for mode history
    mutable mutex modeMutex;

    string modeToString(Mode mode) const {
        switch (mode) {
            case Mode::AC: return "AC";
            case Mode::HEATER: return "HEATER";
            case Mode::AUTO: return "AUTO";
            default: return "UNKNOWN";
        }
    }

public:
    ModeControlScreen() : HVACControl("ModeControl"), currentMode(Mode::AC) {}

    void render() const override {
        lock_guard<mutex> lock(modeMutex);
        cout << "[ModeControlScreen] Mode: " << modeToString(currentMode) << endl;
    }

    void updateSettings() override {
        lock_guard<mutex> lock(modeMutex);
        modeHistory.push_back(modeToString(currentMode));
        if (modeHistory.size() > 10) {
            modeHistory.pop_front();
        }
    }

    void setMode(int mode) {
        lock_guard<mutex> lock(modeMutex);
        if (mode >= 0 && mode <= 2) {
            currentMode = static_cast<Mode>(mode);
        }
    }

    string getCurrentMode() const {
        lock_guard<mutex> lock(modeMutex);
        return modeToString(currentMode);
    }
};

// Climate Control Manager
class ClimateControlManager {
private:
    vector<shared_ptr<HVACControl>> controls;  // Smart pointers in STL container
    mutex managerMutex;
    condition_variable renderCondition;
    atomic<bool> keepRunning{true};

public:
    void addControl(shared_ptr<HVACControl> control) {
        lock_guard<mutex> lock(managerMutex);
        controls.push_back(control);
    }

    void removeControl(const string& controlName) {
        lock_guard<mutex> lock(managerMutex);
        // Using auto storage class for iteration
        auto it = remove_if(controls.begin(), controls.end(),
            [&controlName](const shared_ptr<HVACControl>& control) {
                return control->getName() == controlName;
            });
        controls.erase(it, controls.end());
    }

    void renderAll() {
        lock_guard<mutex> lock(managerMutex);
        register int counter = 0;  // Register storage class (deprecated but for demonstration)

        // Clear screen (simple console clear)
        cout << "\033[2J\033[1;1H";

        for (const auto& control : controls) {  // Auto storage class
            control->render();
            ++counter;
        }
        cout << endl;
    }

    void updateAllSettings() {
        lock_guard<mutex> lock(managerMutex);
        for (auto& control : controls) {
            control->updateSettings();
        }
    }

    void stop() {
        keepRunning = false;
        renderCondition.notify_all();
    }

    bool isRunning() const {
        return keepRunning;
    }

    void waitForUpdate() {
        unique_lock<mutex> lock(managerMutex);
        renderCondition.wait_for(lock, chrono::milliseconds(500));
    }

    void notifyUpdate() {
        renderCondition.notify_one();
    }

    // Get specific controls for thread operations
    shared_ptr<TemperatureControlScreen> getTemperatureControl() {
        lock_guard<mutex> lock(managerMutex);
        for (auto& control : controls) {
            auto tempControl = dynamic_pointer_cast<TemperatureControlScreen>(control);
            if (tempControl) {
                return tempControl;
            }
        }
        return nullptr;
    }

    shared_ptr<FanSpeedControlScreen> getFanControl() {
        lock_guard<mutex> lock(managerMutex);
        for (auto& control : controls) {
            auto fanControl = dynamic_pointer_cast<FanSpeedControlScreen>(control);
            if (fanControl) {
                return fanControl;
            }
        }
        return nullptr;
    }

    shared_ptr<ModeControlScreen> getModeControl() {
        lock_guard<mutex> lock(managerMutex);
        for (auto& control : controls) {
            auto modeControl = dynamic_pointer_cast<ModeControlScreen>(control);
            if (modeControl) {
                return modeControl;
            }
        }
        return nullptr;
    }
};

// Thread function for temperature updates
void temperatureUpdateThread(ClimateControlManager& manager) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> tempDist(18, 28);

    while (manager.isRunning()) {
        auto tempControl = manager.getTemperatureControl();
        if (tempControl) {
            int newTemp = tempDist(gen);
            tempControl->setTemperature(newTemp);
            tempControl->updateSettings();
            manager.notifyUpdate();
        }
        this_thread::sleep_for(chrono::milliseconds(1000));
    }
}

// Thread function for fan and mode updates
void fanModeUpdateThread(ClimateControlManager& manager) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> fanDist(1, fanLimit);
    uniform_int_distribution<> modeDist(0, 2);

    while (manager.isRunning()) {
        auto fanControl = manager.getFanControl();
        auto modeControl = manager.getModeControl();

        if (fanControl && modeControl) {
            // Demonstrate potential deadlock scenario and fix
            // Using scoped_lock to prevent deadlock by locking both mutexes atomically
            scoped_lock lock(fanControl->getFanMutex(),
                                 manager.getTemperatureControl()->getTempMutex());

            int newFanLevel = fanDist(gen);
            int newMode = modeDist(gen);

            fanControl->setFanLevel(newFanLevel);
            modeControl->setMode(newMode);

            fanControl->updateSettings();
            modeControl->updateSettings();

            manager.notifyUpdate();
        }
        this_thread::sleep_for(chrono::milliseconds(1500));
    }
}

// Main rendering thread
void renderThread(ClimateControlManager& manager) {
    while (manager.isRunning()) {
        manager.renderAll();
        manager.waitForUpdate();
    }
}

int main() {
    // Create Climate Control Manager
    ClimateControlManager manager;

    // Create control screens using smart pointers
    auto tempControl = make_shared<TemperatureControlScreen>();
    auto fanControl = make_shared<FanSpeedControlScreen>();
    auto modeControl = make_shared<ModeControlScreen>();

    // Add controls to manager
    manager.addControl(tempControl);
    manager.addControl(fanControl);
    manager.addControl(modeControl);

    // Set initial values
    tempControl->setTemperature(24);
    fanControl->setFanLevel(2);
    modeControl->setMode(0); // AC mode

    cout << "Starting Automotive Climate Control Simulation..." << endl;
    cout << "Press Ctrl+C to stop or wait 10 seconds for auto-stop" << endl;
    this_thread::sleep_for(chrono::milliseconds(2000));

    // Start threads
    thread tempThread(temperatureUpdateThread, ref(manager));
    thread fanModeThread(fanModeUpdateThread, ref(manager));
    thread displayThread(renderThread, ref(manager));

    // Run simulation for 10 seconds
    this_thread::sleep_for(chrono::seconds(10));

    // Graceful shutdown
    cout << "\nShutting down simulation..." << endl;
    manager.stop();

    // Join threads safely
    if (tempThread.joinable()) {
        tempThread.join();
    }
    if (fanModeThread.joinable()) {
        fanModeThread.join();
    }
    if (displayThread.joinable()) {
        displayThread.join();
    }

    cout << "Simulation completed successfully!" << endl;

    return 0;
}