//
// Created by Nicholas Newdigate on 18/02/2024.
//

#ifndef MACHINEMODEL_H
#define MACHINEMODEL_H
#include <cstdint>
#include <vector>

namespace newdigate {

    namespace machine {

        enum machinedisplaytype : uint8_t {
            None = 0,
            ST7735 = 1,
            ST7789 = 2,
            U8G2 = 3
        };

        class machineled {
        public:
            float x = 0,y = 0, z = 0;
            uint16_t pwmValue;    // 12-bit PWM value
        };

        class machinekey {
        public:
            float x = 0,y = 0, z = 0;
            machineled* led = nullptr;
            float verticalDisplacement = 0;
        };

        class machinemodel {
        public:
            bool hasSoundCard = false;
            machinedisplaytype displaytype = None;
            unsigned int NumKeys = 0;
            unsigned int NumLeds = 0;
            std::vector<machinekey> Keys;
            std::vector<machineled> Leds;
            uint16_t framebuffer[240 * 240];
        };

        extern "C" newdigate::machine::machinemodel machine;

    }
}

#endif //MACHINEMODEL_H
