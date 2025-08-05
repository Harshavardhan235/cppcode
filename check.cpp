#include <iostream>
#include <vector>
#include <string>
#include <memory> // For smart pointers
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <random>
#include <chrono> // For std::chrono::milliseconds
#include <deque>  // For temperature history
#include <list>   // For mode history
#include <algorithm> // For std::find_if

// fanLimit is now directly defined here, no 'extern' needed for a single file.
int fanLimit = 5; 

// Mutexes for protecting shared resources
std::mutex g_temperatureMutex;
std::mutex g_fanSpeedMutex;
std::mutex g_modeMutex;
std::mutex g_consoleMutex; // For safe console output

// Condition variable to signal re-render
std::condition_variable g_renderCondition;

// Atomic flag for graceful shutdown
std::atomic<bool> g_keepRunning(true);

// Abstract Base Class
class HVACControl {
public:
    virtual void render() const = 0;
    virtual void updateSettings() = 0;
    virtual std::string getName() const = 0;

protected:
    // Static member for assigning unique IDs
    static int s_idCounter; // Declared, defined below
};

int HVACControl::s_idCounter = 0; // Definition of static member

// Derived Class: TemperatureControlScreen
class TemperatureControlScreen : public HVACControl {
private:
    int m_temperature;
    std::deque<int> m_temperatureHistory;
    mutable int m_logCount; // mutable to allow modification in const methods

public:
    TemperatureControlScreen(int initialTemp = 20)
        : m_temperature(initialTemp), m_logCount(0) {
        s_idCounter++; // Increment static ID counter
        m_temperatureHistory.push_back(initialTemp);
    }

    void setTemperature(int temp) {
        std::lock_guard<std::mutex> lock(g_temperatureMutex);
        if (temp >= 15 && temp <= 30) { // Validation
            m_temperature = temp;
            m_temperatureHistory.push_back(temp);
            if (m_temperatureHistory.size() > 10) { // Keep last 10 temperatures
                m_temperatureHistory.pop_front();
            }
        }
    }

    int getTemperature() const {
        std::lock_guard<std::mutex> lock(g_temperatureMutex);
        return m_temperature;
    }

    void render() const override {
        std::lock_guard<std::mutex> lock(g_consoleMutex);
        std::cout << "[TemperatureControlScreen] Temp: " << getTemperature() << "\u00B0C" << std::endl;
        m_logCount++; // Increment mutable log counter
    }

    void updateSettings() override {
        // This screen's updateSettings might be called by a dedicated thread
        // For manual update, it would prompt user for input.
    }

    std::string getName() const override {
        return "TemperatureControlScreen";
    }
};

// Derived Class: FanSpeedControlScreen
class FanSpeedControlScreen : public HVACControl {
private:
    int m_fanLevel;

public:
    FanSpeedControlScreen(int initialLevel = 1) : m_fanLevel(initialLevel) {
        s_idCounter++;
    }

    void setFanLevel(int level) {
        // Use std::scoped_lock for atomic locking with temperature for deadlock fix demonstration
        std::scoped_lock lock(g_fanSpeedMutex, g_temperatureMutex); // Consistent lock ordering

        if (level >= 0 && level <= fanLimit) { // Validation using global fanLimit
            m_fanLevel = level;
        }
    }

    int getFanLevel() const {
        std::lock_guard<std::mutex> lock(g_fanSpeedMutex);
        return m_fanLevel;
    }

    void render() const override {
        std::lock_guard<std::mutex> lock(g_consoleMutex);
        std::cout << "[FanSpeedControlScreen] Fan: Level " << getFanLevel() << std::endl;
    }

    void updateSettings() override {
        // As above, for manual update.
    }

    std::string getName() const override {
        return "FanSpeedControlScreen";
    }
};

// Derived Class: ModeControlScreen
class ModeControlScreen : public HVACControl {
public:
    enum Mode { AC, Heater, Auto };
private:
    Mode m_currentMode;
    std::list<std::string> m_modeHistory;

public:
    ModeControlScreen(Mode initialMode = AC) : m_currentMode(initialMode) {
        s_idCounter++;
        m_modeHistory.push_back(modeToString(initialMode));
    }

    void setMode(Mode mode) {
        std::lock_guard<std::mutex> lock(g_modeMutex);
        m_currentMode = mode;
        m_modeHistory.push_back(modeToString(mode));
        if (m_modeHistory.size() > 5) { // Keep last 5 mode changes
            m_modeHistory.pop_front();
        }
    }

    Mode getMode() const {
        std::lock_guard<std::mutex> lock(g_modeMutex);
        return m_currentMode;
    }

    std::string modeToString(Mode mode) const {
        switch (mode) {
            case AC: return "AC";
            case Heater: return "Heater";
            case Auto: return "Auto";
            default: return "Unknown";
        }
    }

    void render() const override {
        std::lock_guard<std::mutex> lock(g_consoleMutex);
        std::cout << "[ModeControlScreen] Mode: " << modeToString(getMode()) << std::endl;
    }

    void updateSettings() override {
        // As above, for manual update.
    }

    std::string getName() const override {
        return "ModeControlScreen";
    }
};

// HVAC Manager Module
class ClimateControlManager {
private:
    std::vector<std::shared_ptr<HVACControl>> m_controls;
    std::mutex m_managerMutex; // Mutex for managing the controls vector

public:
    // This member is made public temporarily for main to access shared_ptr,
    // in a real application, proper getter methods would be used.
    std::vector<std::shared_ptr<HVACControl>>& getControls() {
        return m_controls;
    }

    void addControl(std::shared_ptr<HVACControl> control) {
        std::lock_guard<std::mutex> lock(m_managerMutex);
        m_controls.push_back(control);
    }

    void removeControl(const std::string& controlName) {
        std::lock_guard<std::mutex> lock(m_managerMutex);
        m_controls.erase(
            std::remove_if(m_controls.begin(), m_controls.end(),
                           [&](const std::shared_ptr<HVACControl>& control) {
                               return control->getName() == controlName;
                           }),
            m_controls.end());
    }

    void renderAll() {
        std::unique_lock<std::mutex> lock(m_managerMutex);
        g_renderCondition.wait(lock, []{ return true; }); // Simply wait to ensure render is called after updates
        std::lock_guard<std::mutex> consoleLock(g_consoleMutex); // Protect console output for entire render
        std::cout << "\n--- Climate Control Status ---" << std::endl;
        for (auto const& control : m_controls) { // Using auto for iteration
            control->render();
        }
        std::cout << "----------------------------" << std::endl;
    }
};

// Thread functions
void temperatureUpdater(std::shared_ptr<TemperatureControlScreen> tempControl) {
    // Removed 'register' keyword to fix the warning
    int counter = 0; 
    while (g_keepRunning) {
        std::this_thread::sleep_for(std::chrono::seconds(2));
        if (!g_keepRunning) break;

        // Fix with std::scoped_lock for consistent ordering
        std::scoped_lock lock(g_temperatureMutex, g_fanSpeedMutex);

        int currentTemp = tempControl->getTemperature();
        if (currentTemp < 30) { // Cap temperature at 30
            tempControl->setTemperature(currentTemp + 1); // Increase temperature
        } else {
            tempControl->setTemperature(18); // Reset to a lower temp to show change
        }
        

        g_renderCondition.notify_one(); // Notify manager to re-render
        counter++;
    }
}

void fanModeUpdater(std::shared_ptr<FanSpeedControlScreen> fanControl,
                    std::shared_ptr<ModeControlScreen> modeControl) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> fanDist(0, fanLimit); // Using global fanLimit
    std::uniform_int_distribution<> modeDist(0, 2);

    while (g_keepRunning) {
        std::this_thread::sleep_for(std::chrono::seconds(3));
        if (!g_keepRunning) break;

        // Acquire fan and mode mutexes
        std::scoped_lock lock(g_fanSpeedMutex, g_modeMutex);

        fanControl->setFanLevel(fanDist(gen));
        modeControl->setMode(static_cast<ModeControlScreen::Mode>(modeDist(gen)));

        g_renderCondition.notify_one(); // Notify manager to re-render
    }
}

int main() {
    ClimateControlManager manager;

    // Use unique_ptr to manage individual screen instances initially
    std::unique_ptr<TemperatureControlScreen> tempScreen = std::make_unique<TemperatureControlScreen>(24);
    std::unique_ptr<FanSpeedControlScreen> fanScreen = std::make_unique<FanSpeedControlScreen>(2);
    std::unique_ptr<ModeControlScreen> modeScreen = std::make_unique<ModeControlScreen>(ModeControlScreen::AC);

    // Add controls to the manager using shared_ptr
    manager.addControl(std::move(tempScreen)); // tempScreen is moved and now owned by manager (shared_ptr)
    manager.addControl(std::move(fanScreen));
    manager.addControl(std::move(modeScreen));

    // Get shared_ptr references for threads by casting from the manager's vector
    std::shared_ptr<TemperatureControlScreen> sharedTemp = std::dynamic_pointer_cast<TemperatureControlScreen>(manager.getControls()[0]);
    std::shared_ptr<FanSpeedControlScreen> sharedFan = std::dynamic_pointer_cast<FanSpeedControlScreen>(manager.getControls()[1]);
    std::shared_ptr<ModeControlScreen> sharedMode = std::dynamic_pointer_cast<ModeControlScreen>(manager.getControls()[2]);


    // Create threads
    std::thread tempThread(temperatureUpdater, sharedTemp);
    std::thread fanModeThread(fanModeUpdater, sharedFan, sharedMode);

    // Main loop for rendering
    std::cout << "Starting Climate Control Simulation. Press Enter to exit." << std::endl;
    manager.renderAll(); // Initial render

    std::string line;
    std::getline(std::cin, line); // Wait for user input to exit

    g_keepRunning = false; // Signal threads to stop

    // Notify condition variable to unblock any waiting threads before joining
    g_renderCondition.notify_all();

    tempThread.join(); // Wait for threads to finish
    fanModeThread.join();

    std::lock_guard<std::mutex> consoleLock(g_consoleMutex);
    std::cout << "Simulation ended." << std::endl;

    return 0;
}