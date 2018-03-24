/*
 * pulseaudiodevice.h
 *
 *  Created on: Jun 11, 2015
 *      Author: cross
 */

#ifndef PULSEAUDIODEVICE_H_
#define PULSEAUDIODEVICE_H_

#include "headers.h"

class pulse_audio_device_info : public ref_count
{
public:
	enum device_type
	{
		input, output
	};

	pulse_audio_device_info(const pa_sink_info * device_info);
	pulse_audio_device_info(const pa_source_info * device_info);
	virtual ~pulse_audio_device_info();

	// Name of the device.
	const std::string& name();

	// Description for this device.
	const std::string& description();

	// Device index.
	int index();

	// Deivce type: is it input or output device.
	device_type type();

	// Volume level is based on volume level of the first channel. We are assuming that every channel's volume level is the same.
	double volume_level();
	void volume_level(double value);
	pa_volume_t raw_volume_level();

	bool volume_decrement(double value);
	bool volume_increment(double value);

	// Gets if volume is muted
	bool muted();
	void muted(bool value);

	// Device channels
	uint8_t channels();

private:
	std::string m_name;
	std::string m_description;
	int			m_index;
	device_type	m_type;
	pa_volume_t	m_volume_level;
	uint8_t 	m_channels;
	bool		m_muted;
};

#endif /* PULSEAUDIODEVICE_H_ */
