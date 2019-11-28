#ifndef SENSOR_CONTROLLER_HPP_
#define SENSOR_CONTROLLER_HPP_

#include <cstdlib>
#include <memory>
#include <chrono>
#include <filesystem>
#include <thread>
#include <string_view>

#include "commonRpi.hpp"

namespace Car
{
    using centimeters = int; 
    using cm = centimeters;

    constexpr std::chrono::milliseconds ultrasonicInterval{300};

    constexpr std::chrono::milliseconds cameraInterval{3000};

    constexpr std::string_view cameraLogDirSv{"./rpiPics"};
    constexpr std::string_view cameraCmdArgsSv{"--quality 75 --encoding jpg --width 800 --height 600"};

    /** @brief All of the functions and data types relating to sensor usage **/
    class SensorController
    {
        private:
            cm m_ultrasonicDist{0};
            cm calcDistanceUltrasonic();
            void updateLoopUltrasonic(std::chrono::milliseconds loopInterval);

            bool isLoggingDirReady(std::filesystem::path& logDir);
            void cameraLoop(const std::string& cmdArgs,
                            const std::filesystem::path& logDir);

        public:
            cm getDistanceUltrasonic();
            SensorController();
            
    };




}

#endif