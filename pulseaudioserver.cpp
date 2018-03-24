/*
 * pulseaudioserver.cpp
 *
 *  Created on: Jun 11, 2015
 *      Author: cross
 */

#include "pulseaudioserver.h"

pulse_audio_server::pulse_audio_server()
{
	m_connection = new pulse_audio_connection();
}

pulse_audio_server::~pulse_audio_server()
{
}

void pulse_audio_server::set_audio_device_info(refcount_ptr<pulse_audio_device_info> device)
{
	pa_operation * pa_op = nullptr;
	int state = 0;

	pa_cvolume pa_volume = {0x00};
	pa_cvolume_set(&pa_volume, device->channels(), device->raw_volume_level());

	for (;;)
	{
		if (m_connection->get_state() == pulse_audio_connection::state::unknown)
		{
			m_connection->iterate(true);
			continue;
		}

		if (m_connection->get_state() == pulse_audio_connection::state::failed)
		{
			throw pulse_audio_exception(pa_strerror(pa_context_errno(m_connection->get_context())));
		}

		switch (state)
		{
			case 0:
			{
				pa_op = pa_context_set_sink_volume_by_index(m_connection->get_context(), device->index(), &pa_volume, NULL, NULL);
				state++;
			}
			break;

			case 1: /* done settings volume value */
			{
				if (pa_operation_get_state(pa_op) == PA_OPERATION_DONE)
				{
					pa_operation_unref(pa_op);
					state++;

					pa_op = pa_context_set_sink_mute_by_index(m_connection->get_context(), device->index(), device->muted() ? 1 : 0, NULL, NULL);
				}
			}
			break;

			case 2: /* done setting mute flag */
			{
				if (pa_operation_get_state(pa_op) == PA_OPERATION_DONE)
				{
					pa_operation_unref(pa_op);
					return;
				}
			}
			break;
		}

		m_connection->iterate(true);
	}
}

void pulse_audio_server::get_audio_devices_info(std::vector<refcount_ptr<pulse_audio_device_info>> * devices)
{
	pa_operation * pa_op = nullptr;
	int state = 0;
	for (;;)
	{
		if (m_connection->get_state() == pulse_audio_connection::state::unknown)
		{
			m_connection->iterate(true);
			continue;
		}

		if (m_connection->get_state() == pulse_audio_connection::state::failed)
		{
			throw pulse_audio_exception(pa_strerror(pa_context_errno(m_connection->get_context())));
		}

		switch (state)
		{
			case 0:
			{
				pa_op = pa_context_get_sink_info_list(m_connection->get_context(), pulse_audio_get_output_device, devices);
				state++;
			}
			break;

			case 1:
			{
				if (pa_operation_get_state(pa_op) == PA_OPERATION_DONE)
				{
					pa_operation_unref(pa_op);

					pa_op = pa_context_get_source_info_list(m_connection->get_context(), pulse_audio_get_input_device, devices);
					state++;
				}
			}
			break;

			case 2:
			{
				if (pa_operation_get_state(pa_op) == PA_OPERATION_DONE)
				{
					pa_operation_unref(pa_op);
					return;
				}
			}
			break;

			default:
				return;
		}

		m_connection->iterate(true);
	}
}

refcount_ptr<pulse_audio_device_info> pulse_audio_server::get_audio_device_info(const std::string& name)
{
	std::vector<refcount_ptr<pulse_audio_device_info>> devices;
	this->get_audio_devices_info(&devices);

	for (std::vector<refcount_ptr<pulse_audio_device_info>>::const_iterator iter = devices.begin(); iter != devices.end(); ++iter)
	{
		if (name.compare((*iter)->name()) == 0)
		{
			return *iter;
		}
	}

	return nullptr;
}

void pulse_audio_server::pulse_audio_get_output_device(pa_context * context, const pa_sink_info * info, int eol, void * data)
{
	// If eol is set to a positive number, you're at the end of the list
	if (eol > 0)
	{
		return;
	}

	std::vector<refcount_ptr<pulse_audio_device_info>> * devices = (std::vector<refcount_ptr<pulse_audio_device_info>> *)data;

	// Collect device
	devices->push_back(new pulse_audio_device_info(info));
}

void pulse_audio_server::pulse_audio_get_input_device(pa_context* context, const pa_source_info * info, int eol, void * data)
{
	// If eol is set to a positive number, you're at the end of the list
	if (eol > 0)
	{
		return;
	}

	std::vector<refcount_ptr<pulse_audio_device_info>> * devices = (std::vector<refcount_ptr<pulse_audio_device_info>> *)data;

	// Collect device
	devices->push_back(new pulse_audio_device_info(info));
}



