#include <iostream>
#include <vector>
#include <algorithm>

struct SpeedLog {
    std::string timeStamp;
    int speedKmph;
};

void displayLog(const SpeedLog& log) {
    std::cout << "Time: " << log.timeStamp << ", Speed: " << log.speedKmph << " km/h\n";
}

int main() {
    std::vector<SpeedLog> speedLogs = {
        {"08:00", 45},
        {"08:15", 60},
        {"08:30", 80},
        {"08:45", 50},
        {"09:00", 90}
    };

    std::cout << "=== Vehicle Speed Logs ===\n";
    std::for_each(speedLogs.begin(), speedLogs.end(), displayLog);

    auto it = std::find_if(speedLogs.begin(), speedLogs.end(), [](const SpeedLog& log) {
        return log.speedKmph == 50;
    });

    if (it != speedLogs.end()) {
        std::cout << "\n[std::find_if] Speed 50 kmph found at: " << it->timeStamp << "\n";
    } else {
        std::cout << "\n[std::find_if] Speed 50 kmph not found.\n";
    }

    auto overspeed = std::find_if(speedLogs.begin(), speedLogs.end(), [](const SpeedLog& log) {
        return log.speedKmph > 80;
    });

    if (overspeed != speedLogs.end()) {
        std::cout << "[std::find_if] Overspeeding detected at " << overspeed->timeStamp
                  << " with speed " << overspeed->speedKmph << " km/h\n";
    } else {
        std::cout << "[std::find_if] No overspeeding detected.\n";
    }

    return 0;
}