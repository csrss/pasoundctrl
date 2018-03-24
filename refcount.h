/*
 * refcount.h
 *
 *  Created on: Jun 11, 2015
 *      Author: cross
 */

#ifndef REFCOUNT_H_
#define REFCOUNT_H_

class ref_count
{
public:
	ref_count();
	virtual ~ref_count();

	long add_ref(void);
	long release(void);

private:
	const ref_count& operator = (const ref_count&);

protected:
	volatile long m_counter;
};

#endif /* REFCOUNT_H_ */
