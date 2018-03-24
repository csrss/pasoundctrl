/*
 * pulseaudioexception.h
 *
 *  Created on: Jun 14, 2015
 *      Author: cross
 */

#ifndef PULSEAUDIOEXCEPTION_H_
#define PULSEAUDIOEXCEPTION_H_

#include "headers.h"

class pulse_audio_exception : public std::exception
{
public:
	pulse_audio_exception(const std::string& message);
	virtual ~pulse_audio_exception();

	virtual const char * what();

private:
	std::string m_message;
};

#endif /* PULSEAUDIOEXCEPTION_H_ */
