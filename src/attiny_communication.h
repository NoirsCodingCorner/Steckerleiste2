// AttinyCommunication.hpp

#ifndef ATTINYCOMMUNICATION_HPP
#define ATTINYCOMMUNICATION_HPP

#include <Wire.h>
#include <vector>
#include <cstdint>

// I²C clock speed in Hz (override if needed)
inline int clockspeed = 10000;

class AttinyCommunication {
public:
    std::vector<uint8_t> addresses;
    std::vector<bool>    measurements;
    /**
     * @brief Construct a new AttinyCommunication object.
     * 
     * @param sdaPin custom SDA pin, or -1 for default
     * @param sclPin custom SCL pin, or -1 for default
     */
    AttinyCommunication(int sdaPin = -1, int sclPin = -1);

    /**
     * @brief Add an ATtiny's I²C address to the internal list.
     * 
     * @param addr 7-bit I²C address of the ATtiny
     */
    void addAddress(uint8_t addr);

    /**
     * @brief Request and return the latest "movement" reading from one ATtiny.
     * 
     * @param addr I²C address of the ATtiny
     * @return bool the reported movement flag, or false on error
     */
    bool getMovement(uint8_t addr);

    /**
     * @brief Poll all configured ATTinys and store their movement flags.
     */
    void pollAllMovements();

    /**
     * @brief Get the last-stored movement flags for all ATTinys.
     * 
     * @return const std::vector<bool>& flags in same order as getAddresses()
     */
    const std::vector<bool>& getMeasurements() const;

    /**
     * @brief Get the list of configured ATTiny addresses.
     * 
     * @return const std::vector<uint8_t>& the addresses in the order they were added
     */
    const std::vector<uint8_t>& getAddresses() const;

private:
};

#endif // ATTINYCOMMUNICATION_HPP
