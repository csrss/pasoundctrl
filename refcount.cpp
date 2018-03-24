/*
 * refcount.cpp
 *
 *  Created on: Jun 11, 2015
 *      Author: cross
 */

#include "refcount.h"

ref_count::ref_count() : m_counter(0)
{
}

ref_count::~ref_count()
{
}

long ref_count::add_ref(void)
{
	return __sync_fetch_and_add(&m_counter, 1);
}

long ref_count::release(void)
{
	long result = __sync_fetch_and_sub(&m_counter, 1);
	if (m_counter == 0)
	{
		delete this;
	}
	return result;
}

const ref_count& ref_count::operator = (const ref_count&)
{
	return *this;
}
