#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <memory>
#include <utility>

// Forward declaration of the factory class to grant friend access
class InfotainmentNotifierFactory;

/**
 * @class InfotainmentNotifier
 * @brief Represents a notifier for a specific infotainment component.
 *
 * Provides thread-safe methods to send info, warning, and error notifications.
 * A static mutex is used to protect std::cout, ensuring that output from
 * different threads is not interleaved.
 */
class InfotainmentNotifier {
private:
    std::string componentName_;
    // Static mutex to ensure thread-safe console output across all instances.
    // 'inline' allows initialization within the class declaration (C++17+).
    inline static std::mutex cout_mutex_;

    /**
     * @brief Private constructor to enforce creation via the factory.
     * @param componentName The name of the component this notifier is tied to.
     */
    explicit InfotainmentNotifier(std::string componentName)
        : componentName_(std::move(componentName)) {}

    // The factory is a friend, so it can access the private constructor.
    friend class InfotainmentNotifierFactory;

public:
    // Disable copy and move operations to prevent unintended copies.
    InfotainmentNotifier(const InfotainmentNotifier&) = delete;
    InfotainmentNotifier& operator=(const InfotainmentNotifier&) = delete;
    InfotainmentNotifier(InfotainmentNotifier&&) = delete;
    InfotainmentNotifier& operator=(InfotainmentNotifier&&) = delete;

    /**
     * @brief Sends an informational message.
     * @param message The message content.
     */
    void sendInfo(const std::string& message) {
        std::lock_guard<std::mutex> lock(cout_mutex_);
        std::cout << "[" << componentName_ << "] INFO: " << message << std::endl;
    }

    /**
     * @brief Sends a warning message.
     * @param message The message content.
     */
    void sendWarning(const std::string& message) {
        std::lock_guard<std::mutex> lock(cout_mutex_);
        std::cout << "[" << componentName_ << "] WARNING: " << message << std::endl;
    }

    /**
     * @brief Sends an error message.
     * @param message The message content.
     */
    void sendError(const std::string& message) {
        std::lock_guard<std::mutex> lock(cout_mutex_);
        std::cout << "[" << componentName_ << "] ERROR: " << message << std::endl;
    }
};

/**
 * @class InfotainmentNotifierFactory
 * @brief Factory for creating InfotainmentNotifier instances.
 *
 * This class abstracts the construction of notifiers, ensuring they are
 * created correctly and consistently.
 */
class InfotainmentNotifierFactory {
public:
    /**
     * @brief Creates a notifier for a given component.
     * @param componentName The name of the component (e.g., "AudioSystem").
     * @return A shared_ptr to the newly created InfotainmentNotifier.
     */
    static std::shared_ptr<InfotainmentNotifier> createNotifier(const std::string& componentName) {
        // Use a helper struct to allow std::make_shared to access the private constructor.
        // This is a common pattern for creating shared_ptrs to objects with private constructors.
        struct NotifierMaker : public InfotainmentNotifier {
            explicit NotifierMaker(const std::string& name) : InfotainmentNotifier(name) {}
        };
        return std::make_shared<NotifierMaker>(componentName);
    }
};

// 1. Simulated Infotainment Module for Audio System
void audioSystemModule() {
    auto notifier = InfotainmentNotifierFactory::createNotifier("AudioSystem");
    notifier->sendInfo("Playback started.");
    notifier->sendWarning("Low volume detected.");
    notifier->sendError("Failed to load track.");
}

// 2. Simulated Infotainment Module for Navigation System
void navigationSystemModule() {
    auto notifier = InfotainmentNotifierFactory::createNotifier("NavigationSystem");
    notifier->sendInfo("Route calculated.");
    notifier->sendWarning("GPS signal weak.");
    notifier->sendError("Map data not found.");
}

// 3. Simulated Infotainment Module for Bluetooth Manager
void bluetoothManagerModule() {
    auto notifier = InfotainmentNotifierFactory::createNotifier("BluetoothManager");
    notifier->sendInfo("Device connected successfully.");
    notifier->sendWarning("Connection is unstable.");
    notifier->sendError("Pairing failed.");
}

// 4. Simulated Infotainment Module for Voice Assistant
void voiceAssistantModule() {
    auto notifier = InfotainmentNotifierFactory::createNotifier("VoiceAssistant");
    notifier->sendInfo("Listening for command.");
    notifier->sendWarning("Unrecognized speech pattern.");
    notifier->sendError("Voice service unavailable.");
}

int main() {
    std::cout << "Starting Infotainment HMI Notification System..." << std::endl;

    // Create a vector to hold all the threads
    std::vector<std::thread> moduleThreads;

    // Launch each infotainment module in a separate thread
    moduleThreads.emplace_back(audioSystemModule);
    moduleThreads.emplace_back(navigationSystemModule);
    moduleThreads.emplace_back(bluetoothManagerModule);
    moduleThreads.emplace_back(voiceAssistantModule);

    // Wait for all threads to complete their execution
    for (auto& t : moduleThreads) {
        if (t.joinable()) {
            t.join();
        }
    }

    std::cout << "All infotainment modules have completed their tasks." << std::endl;

    return 0;
}