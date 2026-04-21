#include "SerialPrintChannels.h"

#include <stdio.h>

#include <zephyr/sys/printk.h>

SerialPrintChannels::SerialPrintChannels(uint8_t default_capacity)
	: channels(),
	  names(),
	  separator(),
	  channel_count(0U),
	  channel_capacity(0U)
{
	clearChannels();
	copySeparator(DEFAULT_SEPARATOR);
	setCapacity(default_capacity);
}

void SerialPrintChannels::setCapacity(uint8_t capacity)
{
	if (capacity == 0U) {
		channel_capacity = 1U;
	} else if (capacity > MAX_CHANNELS) {
		channel_capacity = MAX_CHANNELS;
	} else {
		channel_capacity = capacity;
	}

	if (channel_count > channel_capacity) {
		channel_count = channel_capacity;
	}
}

void SerialPrintChannels::setSeparator(const char *separator_string)
{
	copySeparator(separator_string);
}

void SerialPrintChannels::clearChannels()
{
	channel_count = 0U;
	for (uint8_t k = 0U; k < MAX_CHANNELS; ++k) {
		channels[k] = nullptr;
		names[k][0] = '\0';
	}
}

void SerialPrintChannels::connectChannel(float32_t &channel, const char name[])
{
	if (channel_count >= channel_capacity) {
		return;
	}

	channels[channel_count] = &channel;
	copyName(channel_count, name);
	channel_count++;
}

void SerialPrintChannels::printHeader() const
{
	char row_buffer[MAX_ROW_LENGTH] = {};
	size_t used_length = 0U;

	for (uint8_t k = 0U; k < channel_count; ++k) {
		if (k > 0U) {
			appendText(row_buffer, sizeof(row_buffer), used_length, separator);
		}
		appendText(row_buffer, sizeof(row_buffer), used_length, names[k]);
	}

	appendText(row_buffer, sizeof(row_buffer), used_length, "\n");
	printk("%s", row_buffer);
}

void SerialPrintChannels::printValues() const
{
	char row_buffer[MAX_ROW_LENGTH] = {};
	size_t used_length = 0U;

	for (uint8_t k = 0U; k < channel_count; ++k) {
		if (k > 0U) {
			appendText(row_buffer, sizeof(row_buffer), used_length, separator);
		}
		appendFormattedValue(row_buffer, sizeof(row_buffer), used_length, *channels[k]);
	}

	appendText(row_buffer, sizeof(row_buffer), used_length, "\n");
	printk("%s", row_buffer);
}

void SerialPrintChannels::copySeparator(const char *separator_string)
{
	if (separator_string == nullptr || separator_string[0] == '\0') {
		separator_string = DEFAULT_SEPARATOR;
	}

	strncpy(separator, separator_string, MAX_SEPARATOR_LENGTH - 1U);
	separator[MAX_SEPARATOR_LENGTH - 1U] = '\0';
}

void SerialPrintChannels::copyName(uint8_t index, const char *name)
{
	const char *resolved_name = name;

	if (resolved_name == nullptr || resolved_name[0] == '\0') {
		resolved_name = DEFAULT_NAME;
	}

	strncpy(names[index], resolved_name, MAX_NAME_LENGTH - 1U);
	names[index][MAX_NAME_LENGTH - 1U] = '\0';
}

void SerialPrintChannels::appendText(char *buffer,
									 size_t buffer_size,
									 size_t &used_length,
									 const char *text) const
{
	if (text == nullptr || used_length >= (buffer_size - 1U)) {
		return;
	}

	int written = snprintf(&buffer[used_length], buffer_size - used_length, "%s", text);
	if (written <= 0) {
		return;
	}

	size_t write_length = static_cast<size_t>(written);
	if (write_length >= (buffer_size - used_length)) {
		used_length = buffer_size - 1U;
	} else {
		used_length += write_length;
	}
}

void SerialPrintChannels::appendFormattedValue(char *buffer,
											   size_t buffer_size,
											   size_t &used_length,
											   float32_t value) const
{
	if (used_length >= (buffer_size - 1U)) {
		return;
	}

	int written = snprintf(&buffer[used_length], buffer_size - used_length, "%7.2f", (double)value);
	if (written <= 0) {
		return;
	}

	size_t write_length = static_cast<size_t>(written);
	if (write_length >= (buffer_size - used_length)) {
		used_length = buffer_size - 1U;
	} else {
		used_length += write_length;
	}
}
