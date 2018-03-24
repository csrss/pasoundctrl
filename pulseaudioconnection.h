/*
 * pulseaudioconnection.h
 *
 *  Created on: Jun 13, 2015
 *      Author: cross
 */

#ifndef PULSEAUDIOCONNECTION_H_
#define PULSEAUDIOCONNECTION_H_

#include "headers.h"
#include "pulseaudioexception.h"

class pulse_audio_connection : public ref_count
{
public:
	enum state
	{
		ready, failed, unknown
	};

	pulse_audio_connection();
	virtual ~pulse_audio_connection();

	// Runs next iteration cycle
	void iterate(bool block);

	pa_context * get_context();
	state get_state();

private:
	static void pulse_audio_get_connection_state(pa_context * context, void * connection);

private:
	pa_mainloop * 	m_pa_mainloop;
	pa_context 	* 	m_pa_context;
	state			m_state;
};

#endif /* PULSEAUDIOCONNECTION_H_ */
