/*
 * pulseaudioserver.h
 *
 *  Created on: Jun 11, 2015
 *      Author: cross
 */

#ifndef PULSEAUDIOSERVER_H_
#define PULSEAUDIOSERVER_H_

#include "headers.h"
#include "pulseaudiodevice.h"
#include "pulseaudioconnection.h"

class pulse_audio_server : public ref_count
{
public:
	pulse_audio_server();
	virtual ~pulse_audio_server();

	// Gets all information regarding audio devices state
	void get_audio_devices_info(std::vector<refcount_ptr<pulse_audio_device_info>> * devices);

	// Returns specific audio device information by its name
	refcount_ptr<pulse_audio_device_info> get_audio_device_info(const std::string& name);

	// Basically, sets only volume in this version for a specific device
	void set_audio_device_info(refcount_ptr<pulse_audio_device_info> device);

private:
	enum connection_state
	{
		ready, failed, unknown
	};

private:
	//static void pulse_audio_get_state(pa_context * context, void * connection);
	static void pulse_audio_get_output_device(pa_context * context, const pa_sink_info * info, int eol, void * data);
	static void pulse_audio_get_input_device(pa_context * context, const pa_source_info * info, int eol, void * data);

private:
	refcount_ptr<pulse_audio_connection> m_connection;
};

#endif /* PULSEAUDIOSERVER_H_ */
