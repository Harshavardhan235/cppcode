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
#include <chrono>
#include <random>
#include <algorithm>

// External variable declaration (simulated)
extern int fanLimit = 5;

// Abstract base class for HVAC Control
class HVACControl {
protected:
    static int idCounter;
    int controlId;
    mutable int logCount = 0; // mutable for logging in const methods
    
public:
    HVACControl() : controlId(++idCounter) {}
    virtual ~HVACControl() = default;
    
    // Pure virtual functions
    virtual void render() const = 0;
    virtual void updateSettings() = 0;
    virtual std::string getName() const = 0;
    
    int getId() const { return controlId; }
    
    void log(const std::string& message) const {
        logCount++; // mutable allows modification in const method
        std::cout << "[LOG " << logCount << "] " << message << std::endl;
    }
};

// Static member definition
int HVACControl::idCounter = 0;

// Temperature Control Screen
class TemperatureControlScreen : public HVACControl {
private:
    mutable std::mutex tempMutex;
    double currentTemp;
    double targetTemp;
    std::deque<int> temperatureHistory; // Last N temperature readings
    static const size_t MAX_HISTORY = 10;
    
public:
    TemperatureControlScreen(double initial = 22.0) 
        : currentTemp(initial), targetTemp(initial) {}
    
    void render() const override {
        std::lock_guard<std::mutex> lock(tempMutex);
        log("Rendering Temperature Control");
        std::cout << "=== TEMPERATURE CONTROL ===" << std::endl;
        std::cout << "Current: " << currentTemp << "°C" << std::endl;
        std::cout << "Target:  " << targetTemp << "°C" << std::endl;
        std::cout << "History: ";
        for (auto temp : temperatureHistory) {
            std::cout << temp << "° ";
        }
        std::cout << std::endl << std::endl;
    }
    
    void updateSettings() override {
        std::lock_guard<std::mutex> lock(tempMutex);
        // Simulate temperature change towards target
        if (currentTemp < targetTemp) {
            currentTemp += 0.5;
        } else if (currentTemp > targetTemp) {
            currentTemp -= 0.5;
        }
        
        // Add to history
        temperatureHistory.push_back(static_cast<int>(currentTemp));
        if (temperatureHistory.size() > MAX_HISTORY) {
            temperatureHistory.pop_front();
        }
    }
    
    std::string getName() const override {
        return "TemperatureControl";
    }
    
    void setTargetTemp(double temp) {
        std::lock_guard<std::mutex> lock(tempMutex);
        targetTemp = temp;
    }
    
    double getCurrentTemp() const {
        std::lock_guard<std::mutex> lock(tempMutex);
        return currentTemp;
    }
    
    std::mutex& getMutex() { return tempMutex; }
};

// Fan Speed Control Screen
class FanSpeedControlScreen : public HVACControl {
private:
    mutable std::mutex fanMutex;
    int fanSpeed; // 0-5
    bool autoMode;
    
public:
    FanSpeedControlScreen(int initial = 2) 
        : fanSpeed(initial), autoMode(false) {}
    
    void render() const override {
        std::lock_guard<std::mutex> lock(fanMutex);
        log("Rendering Fan Speed Control");
        std::cout << "=== FAN SPEED CONTROL ===" << std::endl;
        std::cout << "Speed: " << fanSpeed << "/" << fanLimit << std::endl;
        std::cout << "Mode:  " << (autoMode ? "AUTO" : "MANUAL") << std::endl;
        std::cout << "Visual: ";
        for (int i = 0; i < fanSpeed; i++) {
            std::cout << "█";
        }
        for (int i = fanSpeed; i < fanLimit; i++) {
            std::cout << "░";
        }
        std::cout << std::endl << std::endl;
    }
    
    void updateSettings() override {
        std::lock_guard<std::mutex> lock(fanMutex);
        if (autoMode) {
            // Auto adjustment logic
            static std::random_device rd;
            static std::mt19937 gen(rd());
            std::uniform_int_distribution<> dis(1, fanLimit);
            fanSpeed = dis(gen);
        }
    }
    
    std::string getName() const override {
        return "FanSpeedControl";
    }
    
    void setFanSpeed(int speed) {
        std::lock_guard<std::mutex> lock(fanMutex);
        fanSpeed = std::max(0, std::min(speed, fanLimit));
    }
    
    void toggleAutoMode() {
        std::lock_guard<std::mutex> lock(fanMutex);
        autoMode = !autoMode;
    }
    
    std::mutex& getMutex() { return fanMutex; }
};

// Mode Control Screen
class ModeControlScreen : public HVACControl {
private:
    mutable std::mutex modeMutex;
    enum class ClimateMode { AC, HEATER, AUTO, OFF };
    ClimateMode currentMode;
    std::list<std::string> modeHistory; // Mode change history
    static const size_t MAX_MODE_HISTORY = 5;
    
    std::string modeToString(ClimateMode mode) const {
        switch (mode) {
            case ClimateMode::AC: return "AC";
            case ClimateMode::HEATER: return "HEATER";
            case ClimateMode::AUTO: return "AUTO";
            case ClimateMode::OFF: return "OFF";
            default: return "UNKNOWN";
        }
    }
    
public:
    ModeControlScreen(ClimateMode initial = ClimateMode::AUTO) 
        : currentMode(initial) {
        modeHistory.push_back(modeToString(initial));
    }
    
    void render() const override {
        std::lock_guard<std::mutex> lock(modeMutex);
        log("Rendering Mode Control");
        std::cout << "=== MODE CONTROL ===" << std::endl;
        std::cout << "Current Mode: " << modeToString(currentMode) << std::endl;
        std::cout << "History: ";
        for (const auto& mode : modeHistory) {
            std::cout << mode << " -> ";
        }
        std::cout << "CURRENT" << std::endl << std::endl;
    }
    
    void updateSettings() override {
        std::lock_guard<std::mutex> lock(modeMutex);
        // Randomly change mode for simulation
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, 3);
        
        ClimateMode newMode = static_cast<ClimateMode>(dis(gen));
        if (newMode != currentMode) {
            currentMode = newMode;
            modeHistory.push_back(modeToString(currentMode));
            if (modeHistory.size() > MAX_MODE_HISTORY) {
                modeHistory.pop_front();
            }
        }
    }
    
    std::string getName() const override {
        return "ModeControl";
    }
    
    void setMode(int mode) {
        std::lock_guard<std::mutex> lock(modeMutex);
        if (mode >= 0 && mode <= 3) {
            currentMode = static_cast<ClimateMode>(mode);
            modeHistory.push_back(modeToString(currentMode));
            if (modeHistory.size() > MAX_MODE_HISTORY) {
                modeHistory.pop_front();
            }
        }
    }
};

// Climate Control Manager
class ClimateControlManager {
private:
    std::vector<std::shared_ptr<HVACControl>> controls;
    mutable std::mutex managerMutex;
    std::condition_variable cv;
    std::atomic<bool> keepRunning{true};
    
    // For deadlock demonstration
    std::mutex mutex1, mutex2;
    
public:
    void addControl(std::shared_ptr<HVACControl> control) {
        std::lock_guard<std::mutex> lock(managerMutex);
        controls.push_back(control);
        std::cout << "Added control: " << control->getName() 
                  << " (ID: " << control->getId() << ")" << std::endl;
    }
    
    void removeControl(const std::string& controlName) {
        std::lock_guard<std::mutex> lock(managerMutex);
        auto it = std::remove_if(controls.begin(), controls.end(),
            [&controlName](const std::shared_ptr<HVACControl>& control) {
                return control->getName() == controlName;
            });
        
        if (it != controls.end()) {
            controls.erase(it, controls.end());
            std::cout << "Removed control: " << controlName << std::endl;
        }
    }
    
    void renderAll() {
        std::lock_guard<std::mutex> lock(managerMutex);
        system("clear"); // Linux/Mac - use "cls" for Windows
        std::cout << "╔══════════════════════════════════════╗" << std::endl;
        std::cout << "║     AUTOMOTIVE CLIMATE CONTROL      ║" << std::endl;
        std::cout << "╚══════════════════════════════════════╝" << std::endl;
        std::cout << std::endl;
        
        // Use auto for iteration (storage class demonstration)
        for (auto& control : controls) {
            control->render();
        }
        
        // Register variable demonstration (deprecated but requested)
        register int counter = 0; // Note: register keyword is deprecated in C++17
        for (const auto& control : controls) {
            counter++; // Simple counter using register variable
        }
        std::cout << "Total controls rendered: " << counter << std::endl;
        std::cout << "Press Ctrl+C to exit..." << std::endl;
    }
    
    void updateAllSettings() {
        std::lock_guard<std::mutex> lock(managerMutex);
        for (auto& control : controls) {
            control->updateSettings();
        }
        cv.notify_all();
    }
    
    void stop() {
        keepRunning = false;
        cv.notify_all();
    }
    
    bool isRunning() const {
        return keepRunning;
    }
    
    void waitForUpdate() {
        std::unique_lock<std::mutex> lock(managerMutex);
        cv.wait(lock);
    }
    
    // Deadlock demonstration functions
    void demonstrateDeadlock() {
        std::cout << "Demonstrating deadlock scenario..." << std::endl;
        
        std::thread t1([this]() {
            std::lock_guard<std::mutex> lock1(mutex1);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            std::lock_guard<std::mutex> lock2(mutex2);
            std::cout << "Thread 1 acquired both locks" << std::endl;
        });
        
        std::thread t2([this]() {
            std::lock_guard<std::mutex> lock2(mutex2);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            std::lock_guard<std::mutex> lock1(mutex1);
            std::cout << "Thread 2 acquired both locks" << std::endl;
        });
        
        // This would cause deadlock - commented out for safety
        // t1.join();
        // t2.join();
        
        t1.detach();
        t2.detach();
    }
    
    void demonstrateDeadlockFix() {
        std::cout << "Demonstrating deadlock fix with std::scoped_lock..." << std::endl;
        
        std::thread t1([this]() {
            std::scoped_lock lock(mutex1, mutex2); // Atomic locking
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            std::cout << "Thread 1 acquired both locks safely" << std::endl;
        });
        
        std::thread t2([this]() {
            std::scoped_lock lock(mutex1, mutex2); // Consistent ordering
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            std::cout << "Thread 2 acquired both locks safely" << std::endl;
        });
        
        t1.join();
        t2.join();
    }
};

// Thread functions for simulation
void temperatureUpdateThread(std::shared_ptr<TemperatureControlScreen> tempControl, 
                           ClimateControlManager& manager) {
    while (manager.isRunning()) {
        tempControl->updateSettings();
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }
}

void fanAndModeUpdateThread(std::shared_ptr<FanSpeedControlScreen> fanControl,
                          std::shared_ptr<ModeControlScreen> modeControl,
                          ClimateControlManager& manager) {
    while (manager.isRunning()) {
        fanControl->updateSettings();
        modeControl->updateSettings();
        std::this_thread::sleep_for(std::chrono::seconds(3));
    }
}

void renderThread(ClimateControlManager& manager) {
    while (manager.isRunning()) {
        manager.renderAll();
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

int main() {
    std::cout << "Starting Automotive Climate Control System..." << std::endl;
    
    // Create manager
    ClimateControlManager manager;
    
    // Create controls using smart pointers
    auto tempControl = std::make_unique<TemperatureControlScreen>(22.0);
    auto fanControl = std::make_unique<FanSpeedControlScreen>(2);
    auto modeControl = std::make_unique<ModeControlScreen>();
    
    // Convert to shared_ptr for manager
    auto sharedTempControl = std::shared_ptr<TemperatureControlScreen>(tempControl.release());
    auto sharedFanControl = std::shared_ptr<FanSpeedControlScreen>(fanControl.release());
    auto sharedModeControl = std::shared_ptr<ModeControlScreen>(modeControl.release());
    
    // Add controls to manager
    manager.addControl(sharedTempControl);
    manager.addControl(sharedFanControl);
    manager.addControl(sharedModeControl);
    
    // Demonstrate deadlock scenarios
    std::cout << "\n=== DEADLOCK DEMONSTRATION ===" << std::endl;
    manager.demonstrateDeadlockFix();
    
    std::cout << "\nStarting simulation threads..." << std::endl;
    
    // Create simulation threads
    std::thread tempThread(temperatureUpdateThread, sharedTempControl, std::ref(manager));
    std::thread fanModeThread(fanAndModeUpdateThread, sharedFanControl, sharedModeControl, std::ref(manager));
    std::thread displayThread(renderThread, std::ref(manager));
    
    // Run simulation for a limited time
    std::this_thread::sleep_for(std::chrono::seconds(15));
    
    // Graceful shutdown
    std::cout << "\nShutting down system..." << std::endl;
    manager.stop();
    
    // Safely join threads
    if (tempThread.joinable()) tempThread.join();
    if (fanModeThread.joinable()) fanModeThread.join();
    if (displayThread.joinable()) displayThread.join();
    
    std::cout << "System shutdown complete." << std::endl;
    
    return 0;
}