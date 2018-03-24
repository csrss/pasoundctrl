/*
 * pulseaudiodevice.cpp
 *
 *  Created on: Jun 11, 2015
 *      Author: cross
 */

#include "pulseaudiodevice.h"

pulse_audio_device_info::pulse_audio_device_info(const pa_sink_info * device_info)
	: m_name(device_info->name)
	, m_description(device_info->description)
	, m_index(device_info->index)
	, m_type(output)
	, m_volume_level(pa_cvolume_avg(&device_info->volume))
	, m_channels(device_info->channel_map.channels)
	, m_muted(device_info->mute ? true : false)
{
}

pulse_audio_device_info::pulse_audio_device_info(const pa_source_info * device_info)
	: m_name(device_info->name)
	, m_description(device_info->description)
	, m_index(device_info->index)
	, m_type(input)
	, m_volume_level(pa_cvolume_avg(&device_info->volume))
	, m_channels(device_info->channel_map.channels)
	, m_muted(device_info->mute ? true : false)
{
}

pulse_audio_device_info::~pulse_audio_device_info()
{
}

const std::string& pulse_audio_device_info::name()
{
	return m_name;
}

const std::string& pulse_audio_device_info::description()
{
	return m_description;
}

int pulse_audio_device_info::index()
{
	return m_index;
}

pulse_audio_device_info::device_type pulse_audio_device_info::type()
{
	return m_type;
}

double pulse_audio_device_info::volume_level()
{
	return (m_volume_level * 100) / PA_VOLUME_NORM;
}

void pulse_audio_device_info::volume_level(double value)
{
	// We donot set volume higher then normal volume, 100%
	if (m_volume_level >= PA_VOLUME_NORM)
	{
		m_volume_level = PA_VOLUME_NORM;
		return;
	}
	m_volume_level = (value * PA_VOLUME_NORM) / 100;
}

bool pulse_audio_device_info::muted()
{
	return m_muted;
}

pa_volume_t pulse_audio_device_info::raw_volume_level()
{
	return m_volume_level;
}

void pulse_audio_device_info::muted(bool value)
{
	m_muted = value;
}

uint8_t pulse_audio_device_info::channels()
{
	return m_channels;
}

bool pulse_audio_device_info::volume_decrement(double value)
{
	if (m_volume_level == PA_VOLUME_MUTED)
	{
		// Cannot decrement volume further, return.
		return false;
	}

	pa_volume_t dec_value = (value * PA_VOLUME_NORM) / 100;
	if (dec_value >= m_volume_level)
	{
		m_volume_level = PA_VOLUME_MUTED;
		return true;
	}

	m_volume_level -= dec_value;
	return true;
}

bool pulse_audio_device_info::volume_increment(double value)
{
	if (m_volume_level == PA_VOLUME_NORM)
	{
		// Cannot increase more
		return false;
	}

	pa_volume_t inc_value = (value * PA_VOLUME_NORM) / 100;
	if (inc_value >= PA_VOLUME_NORM - m_volume_level)
	{
		m_volume_level = PA_VOLUME_NORM;
		return true;
	}

	m_volume_level += inc_value;
	return true;
}

