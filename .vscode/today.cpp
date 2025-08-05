#include <iostream>
#include <vector>
#include <memory>
#include <thread>
#include <mutex>
#include <chrono>
#include <random>
#include <string>
#include <algorithm>
#include <atomic>

using namespace std;

// Abstract Base Class
class HVACControl {
protected:
    mutable mutex mtx;
    string controlName;

public:
    HVACControl(const string& name) : controlName(name) {}

    virtual ~HVACControl() = default;

    virtual void render() const = 0;
    virtual void updateSettings() = 0;

    string getName() const { 
        lock_guard<mutex> lock(mtx);
        return controlName; 
    }
};

// Temperature Control
class TemperatureControlScreen : public HVACControl {
private:
    double currentTemp{22.0};
    double targetTemp{22.0};

public:
    TemperatureControlScreen() : HVACControl("Temperature") {}

    void render() const override {
        lock_guard<mutex> lock(mtx);
        cout << "Temperature: " << currentTemp << "°C (Target: " << targetTemp << "°C)\n";
    }

    void updateSettings() override {
        lock_guard<mutex> lock(mtx);
        if (currentTemp < targetTemp) currentTemp += 0.1;
        else if (currentTemp > targetTemp) currentTemp -= 0.1;
    }

    void setTargetTemp(double temp) {
        lock_guard<mutex> lock(mtx);
        targetTemp = clamp(temp, 16.0, 30.0);
    }

    void setCurrentTemp(double temp) {
        lock_guard<mutex> lock(mtx);
        currentTemp = temp;
    }

    double getCurrentTemp() const {
        lock_guard<mutex> lock(mtx);
        return currentTemp;
    }
};

// Fan Speed Control
class FanSpeedControlScreen : public HVACControl {
private:
    int fanSpeed{1};
    const int fanLimit = 5;

public:
    FanSpeedControlScreen() : HVACControl("Fan") {}

    void render() const override {
        lock_guard<mutex> lock(mtx);
        cout << "Fan Speed Level: " << fanSpeed << " (1-" << fanLimit << ")\n";
    }

    void updateSettings() override {
        lock_guard<mutex> lock(mtx);
        fanSpeed = clamp(fanSpeed, 1, fanLimit);
    }

    void setFanSpeed(int speed) {
        lock_guard<mutex> lock(mtx);
        fanSpeed = clamp(speed, 1, fanLimit);
    }

    int getFanSpeed() const {
        lock_guard<mutex> lock(mtx);
        return fanSpeed;
    }
};

// Mode Control
class ModeControlScreen : public HVACControl {
public:
    enum class Mode { AC, HEATER, AUTO };

private:
    Mode currentMode{Mode::AUTO};

public:
    ModeControlScreen() : HVACControl("Mode") {}

    void render() const override {
        lock_guard<mutex> lock(mtx);
        cout << "Mode: " << getModeString() << "\n";
    }

    void updateSettings() override {
        lock_guard<mutex> lock(mtx);
    }

    string getModeString() const {
        lock_guard<mutex> lock(mtx);
        switch (currentMode) {
            case Mode::AC:
                return "AC";
            case Mode::HEATER:
                return "Heater";
            case Mode::AUTO:
                return "Auto";
            default:
                return "Unknown";
        }
    }

    void setMode(Mode mode) {
        lock_guard<mutex> lock(mtx);
        currentMode = mode;
    }

    Mode getCurrentMode() const {
        lock_guard<mutex> lock(mtx);
        return currentMode;
    }
};

// Climate Control Manager
class ClimateControlManager {
private:
    vector<shared_ptr<HVACControl>> controls;
    mutable mutex managerMtx;
    atomic<bool> running{true};

public:
    void addControl(shared_ptr<HVACControl> control) {
        lock_guard<mutex> lock(managerMtx);
        controls.push_back(control);
    }

    void removeControl(const string& controlName) {
        lock_guard<mutex> lock(managerMtx);
        controls.erase(
            remove_if(controls.begin(), controls.end(),
                      [&controlName](const shared_ptr<HVACControl>& control) {
                          return control->getName() == controlName;
                      }),
            controls.end());
    }

    void renderAll() const {
        lock_guard<mutex> lock(managerMtx);
        cout << "\033[2J\033[1;1H";  // Clear screen
        cout << "=== Climate Control System ===\n";
        for (const auto& control : controls) {
            control->render();
        }
        cout << "============================\n";
    }

    void startSimulation() {
        auto tempControl = make_shared<TemperatureControlScreen>();
        auto fanControl = make_shared<FanSpeedControlScreen>();
        auto modeControl = make_shared<ModeControlScreen>();

        addControl(tempControl);
        addControl(fanControl);
        addControl(modeControl);

        thread tempThread([this, tempControl]() {
            random_device rd;
            mt19937 gen(rd());
            uniform_real_distribution<double> tempDist(18.0, 28.0);

            while (running) {
                tempControl->setCurrentTemp(tempDist(gen));
                tempControl->updateSettings();
                this_thread::sleep_for(chrono::milliseconds(1000));
            }
        });

        thread fanThread([this, fanControl]() {
            random_device rd;
            mt19937 gen(rd());
            uniform_int_distribution<> fanDist(1, 5);

            while (running) {
                fanControl->setFanSpeed(fanDist(gen));
                fanControl->updateSettings();
                this_thread::sleep_for(chrono::seconds(2));
            }
        });

        thread modeThread([this, modeControl]() {
            random_device rd;
            mt19937 gen(rd());
            uniform_int_distribution<> modeDist(0, 2);

            while (running) {
                modeControl->setMode(static_cast<ModeControlScreen::Mode>(modeDist(gen)));
                modeControl->updateSettings();
                this_thread::sleep_for(chrono::seconds(3));
            }
        });

        thread renderThread([this]() {
            while (running) {
                renderAll();
                this_thread::sleep_for(chrono::seconds(1));
            }
        });

        // Run simulation for 15 seconds
        this_thread::sleep_for(chrono::seconds(15));

        running = false;

        tempThread.join();
        fanThread.join();
        modeThread.join();
        renderThread.join();
    }
};

int main() {
    ClimateControlManager manager;
    manager.startSimulation();
    return 0;
}