/*
 * pulseaudioconnection.cpp
 *
 *  Created on: Jun 13, 2015
 *      Author: cross
 */

#include "pulseaudioconnection.h"

pulse_audio_connection::pulse_audio_connection() : m_state(state::unknown)
{
	m_pa_mainloop = pa_mainloop_new();
	if (m_pa_mainloop == nullptr)
	{
		return;
	}

	uuid_t guid = {0x00};
	uuid_generate_random(guid);

	char uuid[37] = {0x00};
	uuid_unparse(guid, uuid);

	m_pa_context = pa_context_new(pa_mainloop_get_api(m_pa_mainloop), uuid);
	if (m_pa_context == nullptr)
	{
		return;
	}

	pa_context_set_state_callback(m_pa_context, pulse_audio_get_connection_state, &m_state);
	pa_context_connect(m_pa_context, nullptr, pa_context_flags::PA_CONTEXT_NOFLAGS, nullptr);
}

pulse_audio_connection::~pulse_audio_connection()
{
	if (m_pa_mainloop != nullptr)
	{
		pa_mainloop_free(m_pa_mainloop);
	}
	if (m_pa_context != nullptr)
	{
		pa_context_disconnect(m_pa_context);
		pa_context_unref(m_pa_context);
	}
}

pa_context * pulse_audio_connection::get_context()
{
	return m_pa_context;
}

void pulse_audio_connection::iterate(bool block)
{
	pa_mainloop_iterate(m_pa_mainloop, block ? 1 : 0, nullptr);
}

pulse_audio_connection::state pulse_audio_connection::get_state()
{
	return m_state;
}

void pulse_audio_connection::pulse_audio_get_connection_state(pa_context* context, void* connection)
{
	pa_context_state_t state;
	pulse_audio_connection::state * pa_ready = (pulse_audio_connection::state *)connection;

	state = pa_context_get_state(context);
	switch  (state)
	{
		case PA_CONTEXT_UNCONNECTED:
		case PA_CONTEXT_CONNECTING:
		case PA_CONTEXT_AUTHORIZING:
		case PA_CONTEXT_SETTING_NAME:
		default:
			*pa_ready = pulse_audio_connection::state::unknown;
			break;
		case PA_CONTEXT_FAILED:
		case PA_CONTEXT_TERMINATED:
			*pa_ready = pulse_audio_connection::state::failed;
			break;
		case PA_CONTEXT_READY:
			*pa_ready = pulse_audio_connection::state::ready;
			break;
	}
}
