/*
 * refcount_ptr.h
 *
 *  Created on: Jun 11, 2015
 *      Author: cross
 */

#ifndef REFCOUNT_PTR_H_
#define REFCOUNT_PTR_H_


template <class T> class refcount_ptr
{
public:
	refcount_ptr()
	{
		p = nullptr;
	}

	refcount_ptr(T * lp)
	{
		if ((p = lp) != nullptr)
		{
			p->add_ref();
		}
	}

	refcount_ptr(const refcount_ptr<T>& lp)
	{
		if ((p = lp.p) != nullptr)
		{
			p->add_ref();
		}
	}

	virtual ~refcount_ptr(void)
	{
		if (p)
		{
			p->release();
		}
	}

//	void Release()
//	{
//		T * pTemp = p;
//		if (pTemp)
//		{
//			p = nullptr;
//			pTemp->release();
//		}
//	}

	operator T * () const { return p; }
	T & operator * () const { return *p; }
	T * operator->() const { return p; }
	T * operator = (T * lp) { return ref_count_ptr_assign(&p, lp); }
	T * operator = (const refcount_ptr<T> & lp) { return ref_count_ptr_assign(&p, lp.p); }
	bool operator!() const { return (p == nullptr); }
	bool operator == (T *lp) { return (p == lp); }
	bool operator < (T * lp) { return (p < lp); }

	void Attach(T * p2)
	{
		if(p)
		{
			p->release();
			p = p2;
		}
	}

	T * Detach()
	{
		T * pt = p;
		p = nullptr;
		return pt;
	}

	T * p;

protected:
	T * ref_count_ptr_assign(T ** pp, T * lp)
	{
		if (lp != nullptr)
		{
			lp->add_ref();
		}
		if (*pp)
		{
			(*pp)->release();
		}
		*pp = lp;
		return lp;
	}
};


#endif /* REFCOUNT_PTR_H_ */
