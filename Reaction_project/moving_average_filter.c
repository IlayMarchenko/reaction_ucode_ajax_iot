#include <threads_header.h>

/**
 * @brief Function 'filter' is representation of moving average filter in c programming language
 * @param 'raw_value' - fresh raw value from sensor that will be filtered
 * @param 'buffer[]' - array with previous values from sensor
 * @param 'length_of_buffer' - length of previous parameter 'buffer[]'
 * @return float filtered value
 */
float filter(float raw_value, float buffer[], uint8_t length_of_buffer) {
    float current_value = 0;
    buffer[length_of_buffer - 1] = raw_value;
    uint8_t i = 0;
    for(; i < length_of_buffer; i++) {
        current_value += buffer[i];
    }
    current_value /= length_of_buffer;
    for(i = 0; i < length_of_buffer; i++) {
        buffer[i] = buffer[i+1];
    }
    return current_value;
}

