#pragma once

#include "arm_math_types.h"

#include <stddef.h>
#include <stdint.h>
#include <string.h>

/**
 * @brief Reusable serial channel printer with ScopeMimicry-style registration.
 *
 * This class stores named float channels connected by reference and can print
 * either their header row or their current values in connection order.
 */
class SerialPrintChannels {
public:
	static constexpr uint8_t MAX_CHANNELS = 32U;
	static constexpr size_t MAX_NAME_LENGTH = 32U;
	static constexpr size_t MAX_SEPARATOR_LENGTH = 8U;
	static constexpr size_t MAX_FIELD_PRINT_LENGTH = 32U;
	static constexpr size_t MAX_ROW_LENGTH =
		(MAX_CHANNELS * MAX_FIELD_PRINT_LENGTH) +
		((MAX_CHANNELS - 1U) * MAX_SEPARATOR_LENGTH) + 2U;

	/**
	 * @brief Construct a serial printer with the default active capacity.
	 *
	 * @param default_capacity Active registration capacity after construction.
	 */
	explicit SerialPrintChannels(uint8_t default_capacity = 10U);

	/**
	 * @brief Set the active registration capacity.
	 *
	 * Reducing the capacity truncates existing registrations from the end.
	 *
	 * @param capacity Active maximum number of connected channels.
	 */
	void setCapacity(uint8_t capacity);

	/**
	 * @brief Set the field separator used for header and value rows.
	 *
	 * @param separator Null-terminated separator string.
	 */
	void setSeparator(const char *separator_string);

	/**
	 * @brief Remove all connected channels while keeping the configuration.
	 */
	void clearChannels();

	/**
	 * @brief Connect one float channel to the serial printer.
	 *
	 * The channel is stored internally by address, while the name is copied
	 * into internal storage.
	 *
	 * @param channel Static or persistent float variable to print.
	 * @param name User-visible channel name used in the header row.
	 */
	void connectChannel(float32_t &channel, const char name[]);

	/**
	 * @brief Print the connected channel names in connection order.
	 */
	void printHeader() const;

	/**
	 * @brief Print the connected channel values in connection order.
	 */
	void printValues() const;

private:
	static constexpr char DEFAULT_SEPARATOR[] = ":";
	static constexpr char DEFAULT_NAME[] = "unnamed";

	void copySeparator(const char *separator_string);

	void copyName(uint8_t index, const char *name);

	void appendText(char *buffer, size_t buffer_size, size_t &used_length, const char *text) const;

	void appendFormattedValue(char *buffer, size_t buffer_size, size_t &used_length, float32_t value) const;

	float32_t *channels[MAX_CHANNELS];
	char names[MAX_CHANNELS][MAX_NAME_LENGTH];
	char separator[MAX_SEPARATOR_LENGTH];
	uint8_t channel_count;
	uint8_t channel_capacity;
};
