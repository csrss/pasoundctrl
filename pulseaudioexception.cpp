/*
 * pulseaudioexception.cpp
 *
 *  Created on: Jun 14, 2015
 *      Author: cross
 */

#include "pulseaudioexception.h"

pulse_audio_exception::pulse_audio_exception(const std::string& message)
	: m_message(message)
{
}

pulse_audio_exception::~pulse_audio_exception()
{
}

const char* pulse_audio_exception::what()
{
	return m_message.c_str();
}
