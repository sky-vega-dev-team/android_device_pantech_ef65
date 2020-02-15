/*
 * Copyright (C) 2018 The LineageOS Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#define LOG_TAG "android.hardware.light@2.0-service.ef65"
#include <android-base/logging.h>
#include <hidl/HidlTransportSupport.h>
#ifdef ARCH_ARM_32
#include <hwbinder/ProcessState.h>
#endif
#include <utils/Errors.h>
#include "Light.h"

// libhwbinder:
using android::hardware::configureRpcThreadpool;
using android::hardware::joinRpcThreadpool;

// Generated HIDL files
using android::hardware::light::V2_0::ILight;
using android::hardware::light::V2_0::implementation::Light;

const static std::string kLcdBacklightPath = "/sys/class/leds/lcd-backlight/brightness";
const static std::string kButtonLedPath = "/sys/class/leds/kpdbl_menu/brightness";
const static std::string kButtonLed1Path = "/sys/class/leds/kpdbl_back/brightness";
const static std::string kRedLedPath = "/sys/class/leds/led:rgb_red/brightness";
const static std::string kGreenLedPath = "/sys/class/leds/led:rgb_green/brightness";
const static std::string kBlueLedPath = "/sys/class/leds/led:rgb_blue/brightness";

int main() {
#ifdef ARCH_ARM_32
    android::hardware::ProcessState::initWithMmapSize((size_t)(32768));
#endif
    std::ofstream lcdBacklight(kLcdBacklightPath);
    if (!lcdBacklight) {
        LOG(ERROR) << "Failed to open " << kLcdBacklightPath << ", error=" << errno
                   << " (" << strerror(errno) << ")";
        return -errno;
    }
    std::ofstream buttonLed(kButtonLedPath);
    if (!buttonLed) {
        LOG(ERROR) << "Failed to open " << kButtonLedPath << ", error=" << errno
                   << " (" << strerror(errno) << ")";
        return -errno;
    }
    std::ofstream buttonLed1(kButtonLed1Path);
    if (!buttonLed1) {
        LOG(ERROR) << "Failed to open " << kButtonLed1Path << ", error=" << errno
                   << " (" << strerror(errno) << ")";
        return -errno;
    }
    std::ofstream redLed(kRedLedPath);
    if (!redLed) {
        LOG(ERROR) << "Failed to open " << kRedLedPath << ", error=" << errno
                   << " (" << strerror(errno) << ")";
        return -errno;
    }
    std::ofstream greenLed(kGreenLedPath);
    if (!greenLed) {
        LOG(ERROR) << "Failed to open " << kGreenLedPath << ", error=" << errno
                   << " (" << strerror(errno) << ")";
        return -errno;
    }
    std::ofstream blueLed(kBlueLedPath);
    if (!blueLed) {
        LOG(ERROR) << "Failed to open " << kBlueLedPath << ", error=" << errno
                   << " (" << strerror(errno) << ")";
        return -errno;
    }
    android::sp<ILight> service = new Light(
            std::move(lcdBacklight),
            { std::move(buttonLed), std::move(buttonLed1) },
            std::move(redLed), std::move(greenLed), std::move(blueLed));
    configureRpcThreadpool(1, true);
    android::status_t status = service->registerAsService();
    if (status != android::OK) {
        LOG(ERROR) << "Cannot register Light HAL service";
        return 1;
    }
    LOG(INFO) << "Light HAL Ready.";
    joinRpcThreadpool();
    // Under normal cases, execution will not reach this line.
    LOG(ERROR) << "Light HAL failed to join thread pool.";
    return 1;
}
