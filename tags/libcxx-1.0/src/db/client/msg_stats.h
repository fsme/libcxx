//@(#) MRAS message statistic
// Author: Andrew Wingorodov <http://andr.mobi/>
// Licence: LGPLv3
// GCC 3.3; FreeBSD 4.10
//$Id: msg_stats.h 550 2008-03-07 16:08:47Z wingorodov $

#ifndef MSG_STATS_H
#define MSG_STATS_H

#include <sys/time.h>

#include <sstream>
#include <bitset>

using namespace std;

///\brief Bits of statistic
enum spam_flags {
	  known	= 0		///< Unclassified
	, spam		= 1		///< This is it
	, legal		= 2		///< Legal spam
	, analysis	= 3		///< Send to analyst
	, timeout	= 4		///< Timeout
};

#pragma pack(1)
///\brief Message status
struct msg_stats {
	uint16_t hits;	///< Amount of hits
	time_t ac_time;	///< Last access time
	time_t pr_time;	///< Time to send to the analysis
	std::bitset<5> flags;	///< Flags
};
#pragma pack(4)

///\class msgstats
///\brief Method of statistic
struct msgstats
	: public msg_stats
{
	///\brief Reset msg_stats
	void reset ()
	{
		hits = 0;
		ac_time = 0;
		pr_time = 0;
		flags.reset ();
	}

	///\brief Get info as string
	const std::string& str () const
	{
		static std::string _ret;
		std::stringstream _sstr;
		_sstr.str("");
		_sstr.clear();
		_sstr << dec << "hits " << hits << " last " << ac_time
			  << " analysis " << pr_time
			  << " flags " << flags << flush;
		;
		return _ret = _sstr.str();
	}

	///\brief Get current time in second
	long now () const
	{
		static struct timeval _now;
		::gettimeofday ( &_now, 0);
		return _now.tv_sec;
	}

	///\brief Set last access time
	void touch () { ac_time = now(); }

	///\brief Sent to analyst timestamp
	void set_analyst_flags () { pr_time = now(); flags.set (analysis); }

	///\brief It is legal spam
	bool is_legal () const { return flags.test(known) && flags.test(legal); }

	///\brief It is spam
	bool is_spam () const { return flags.test(spam); }

	///\brief It is legal spam
	bool is_analyst () const { return flags.test (analysis); }

	///\brief It is legal spam
	bool is_timeout () { return flags.test (timeout); }
};

#endif
