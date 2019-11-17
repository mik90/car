#include "sensorController.hpp"

namespace Car
{

bool SensorController::isLoggingDirReady(std::filesystem::path& logDir)
{
    if(!std::filesystem::exists(logDir))
    {
        // logDir doesn't exist, create all needed directories if they don't exist
        std::filesystem::create_directories(logDir);
    }
    else if(!std::filesystem::is_directory(logDir))
    {
        // logDir exists and isn't a directory, return an error
        std::cerr << "Error: log path \"" << logDir << "\" already exists and is not a directory.\n";
        return false;
    }
    else if(!std::filesystem::is_empty(logDir))
    {
        // logDir exists and isn't empty, clear it out
        logDir.clear();
    }

    return true;
}

// The Raspberry Pi camera does not have many APIs available outside of Python,
// there is a C++ one floating arond but it doesn't seem active
// Worst comes to worst, just use system("raspistill") to call the RaspiStill binary
// that captures a single image

// Periodically get pictures
[[noreturn]] void SensorController::cameraLoop(const std::string& cmdArgs,
                                               const std::filesystem::path& logDir)
{
    using namespace std::chrono_literals;

    const std::string firstHalfOfCmd {"raspistill -o" + logDir.string() + "carCamera-"};
    // Picture number is between the two
    const std::string secondHalfOfCmd{".jpg" + cmdArgs};


    auto createCmdString = [&firstHalfOfCmd, &secondHalfOfCmd] (unsigned int pictureNum) { 
        return std::string{firstHalfOfCmd + std::to_string(pictureNum) + secondHalfOfCmd};
    };

    std::cout << "Camera is ready, taking pictures every " 
              << cameraInterval.count() << " milliseconds\n";
    std::cout << "Pictures can be found at \"" << logDir << "\"" << std::endl; 

    unsigned int i = 0;
    while (true)
    {
        std::system(createCmdString(i++).c_str());
        std::this_thread::sleep_for(3s);
    }
}

}