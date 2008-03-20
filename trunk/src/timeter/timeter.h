//@(#) Timers
// Licence: LGPLv3
// Author Andrew Wingorodov <http://andr.mobi/>
//$Id: timeter.h 592 2008-03-12 15:27:51Z wingorodov $

#ifndef _TI_METER_H
#define _TI_METER_H 1

#include <sys/time.h>

#include <sstream>

#include <logostream.hpp>

const int MILLION = 999999L;

using namespace std;
using namespace cxx;

namespace ti {

///\class meter
///\brief Time meter

class meter
{
public:
	struct timeval result;

	///\brief Constructor
	meter (
		const std::string& ident_ = "andr.mobi#"///\param ident_ Identifier
	)
		: works (false)
	{
		start (ident_);
	}

	///\brief Destructor
	~meter () { stop (); }

	///\brief Start timing
	void start (
		const std::string& ident_ ///\param ident_ Identifier string
	) {
		this->stop ();
		if ( !ident_.empty()) _ident = ident_;
		::gettimeofday (&_start, 0);
		works = true;
	}

	///\brief Stop timing
	void stop ()
	{
		if (!works) return; else works = false;

		::gettimeofday (&_stop, 0);

		if (_stop.tv_sec == _start.tv_sec)
		{
			result.tv_sec = 0L;
			result.tv_usec = _stop.tv_usec - _start.tv_usec;
		}
		else {
			result.tv_sec = _stop.tv_sec - _start.tv_sec - 1;
			result.tv_usec = MILLION-_start.tv_usec+_stop.tv_usec;

			if (result.tv_usec > MILLION)
			{
				++result.tv_sec;
				result.tv_usec -= MILLION;
			}
		}

		std::stringstream res;
		res << ident() << result.tv_sec << "."; 
		res.fill ('0');
		res.width (6);
		res << result.tv_usec;

		logs << info << res.str() << endl;
	}

	///\brief Get identifier string
	///\return identifier
	const std::string& ident () const { return _ident; }

private:
	struct timeval _start;
	struct timeval _stop;
	std::string _ident;
	bool works;

}; //class meter

///\class mer
///\brief Timer

class mer
{
public:
	struct timeval result;

	///\brief Constructor
	mer (
		const std::string& ident_ = "second: "///\param ident_ Identifier
	)
		: up (false)
	{
		start (ident_);
		_total = _start;
	}

	///\brief Destructor
	~mer ()
	{
		stop ();
		_ident += " Total time ";
		calc (_total, _stop); show ();
	}

	///\brief Start timing
	void start (
		const std::string& ident_ ///\param ident_ Identifier string
	) {
		if ( !ident_.empty()) _ident = ident_;
		this->stop ();
		::gettimeofday (&_start, 0);
		up = true;
	}

	///\brief Start timing
	void here (
		const std::string& ident_ ///\param ident_ Identifier string
	) {
		if (!ident_.empty()) _ident = ident_;
		if (!up) return;
		::gettimeofday (&_here, 0);

		calc (_start, _here); show ();
	}

	///\brief Stop timing
	void stop ()
	{
		if (!up) return;
		up = false;
		::gettimeofday (&_stop, 0);
		calc (_start, _stop); show ();
	}

	///\brief Get identifier string
	///\return identifier
	const std::string& ident () const { return _ident; }

private:
	///\brief Calculation the time
	void calc (
		  const struct timeval& start_ ///\param start_ Timeval of start
		, const struct timeval& stop_ ///\param stop_ Timeval of stop
	) { 
		if (stop_.tv_sec == start_.tv_sec) {
			result.tv_sec = 0L;
			result.tv_usec = stop_.tv_usec - start_.tv_usec;
		}
		else {
			result.tv_sec = stop_.tv_sec - start_.tv_sec - 1;
			result.tv_usec = MILLION - start_.tv_usec + stop_.tv_usec;

			if (result.tv_usec > MILLION) {
				++result.tv_sec;
				result.tv_usec -= MILLION;
			}
		}
	}

	///\brief Show the time
	void show () const
	{
		static std::stringstream res;
		res.str("");
		res.clear();
		res << ident() << result.tv_sec << "."; 
		res.fill ('0');
		res.width (6);
		res << result.tv_usec;

		if (logs << info) logs << res.str() << endl;
	}

	struct timeval _start;
	struct timeval _stop;
	struct timeval _total;
	struct timeval _here;
	struct timeval _average;
	u_int32_t _amount;
	std::string _ident;
	bool up;

}; //class mer

} //::ti

#endif ///_TI_METER_H
