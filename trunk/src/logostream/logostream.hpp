//@(#) Output logging stream (using ::syslog(3))
// Author: Andrew Wingorodov <http://andr.ru/>
// Licence: LGPLv3
// GCC 3.3; FreeBSD 4.10
// $Id: logostream.hpp 590 2008-03-12 14:59:33Z wingorodov $

#ifndef CXX_LOGOSTREAM_HPP
#define CXX_LOGOSTREAM_HPP 1

#include <syslog.h>
#include <stdarg.h>

#include <streambuf>
#include <iostream>
#include <string>

#ifndef STREAMBUF_SIZE	
#define STREAMBUF_SIZE	1024
#endif

using namespace std;

namespace cxx {

///\brief Priority of message
enum pri {
	  level_emerg   = LOG_EMERG ///< A panic condition
	, level_alert   = LOG_ALERT ///< A condition that should be corrected
	, level_critical= LOG_CRIT ///< Critical condition, e.g, hard device err
	, level_error   = LOG_ERR ///< Errors
	, level_warning = LOG_WARNING ///< Warning messages
	, level_notice  = LOG_NOTICE ///< Possibly be handled specially
	, level_info    = LOG_INFO ///< Informational
	, level_debug   = LOG_DEBUG ///< For debugging program
};

///\brief Facility of message
enum fac {
	  fac_auth    = LOG_AUTH	///< The authorization system
	, fac_privat  = LOG_AUTHPRIV ///< The same as auth
///	, fac_console = LOG_CONSOLE ///< Message to /dev/console LOG_CONS for linux
	, fac_cron    = LOG_CRON ///< The cron daemon
	, fac_daemon  = LOG_DAEMON ///< Systems daemons
	, fac_ftp     = LOG_FTP ///< The FTP daemon
	, fac_kern    = LOG_KERN ///< Messages generated by the kernel
	, fac_lpr     = LOG_LPR ///<  The line printer spooling
	, fac_mail    = LOG_MAIL ///< The mail system
	, fac_news    = LOG_NEWS ///< The network news
///	, fac_security= LOG_SECURITY ///< Security subsystems FREEBSD ONLY
	, fac_syslog  = LOG_SYSLOG ///< Messages generated internally
	, fac_user    = LOG_USER ///< By default
	, fac_uucp    = LOG_UUCP ///< The uucp system
	, fac_local0  = LOG_LOCAL0 ///< Local facility
	, fac_local1  = LOG_LOCAL1 ///< Local facility
	, fac_local2  = LOG_LOCAL2 ///< Local facility
	, fac_local3  = LOG_LOCAL3 ///< Local facility
	, fac_local4  = LOG_LOCAL4 ///< Local facility
	, fac_local5  = LOG_LOCAL5 ///< Local facility
	, fac_local6  = LOG_LOCAL6 ///< Local facility
	, fac_local7  = LOG_LOCAL7 ///< Local facility
};

///\brief Logging options
enum opt {
	  opt_cons    = LOG_CONS ///< Write message to /dev/console
	, opt_nodelay = LOG_NDELAY///< Open the connection to syslogd immediately
	, opt_perror  = LOG_PERROR ///< Write to standard error output
	, opt_pid     = LOG_PID ///< Log the proccess id with each message
};

///\class log_state 
///\brief Priority and level state of messages
struct log_state
{
	pri current_priority;
	pri priority;
	fac facility;
	opt logopt;

	///\brief Constructor by default
	log_state (
		  const opt opt_ = opt_pid ///\param opt_ Options default
		, const fac fac_ = fac_user ///\param fac_ Facility default
	)
		: priority (level_error)
		, current_priority (level_error)
		, logopt (opt_)
		, facility (fac_)
	{}

}; //.class log_state

///\class logostrmbuf
///\brief Log stream buffer

template <typename C, typename T = std::char_traits<C> >
class logostrmbuf
	: public std::basic_streambuf<C,T>
{
public:
typedef typename T::int_type	int_type;
typedef typename T::pos_type	pos_type;

	///\brief Constructor by default
	logostrmbuf ()
		: _is_open (false)
	{
		obuffer = new C [STREAMBUF_SIZE];
	}

	///\brief Constructor
	logostrmbuf (
		  const char* ident_ ///\param ident_ Identification of log
		, const opt logopt_   ///\param logopt_ Log options
		, const fac facility_ ///\param facility_ Log facility
	)
		: _is_open (false)
		, _state (logopt_, facility_)
	{
		obuffer = new C [STREAMBUF_SIZE] ;
		open (ident_, logopt_, facility_);
	}

	///\brief Destructor
	~logostrmbuf ()
	{
		close ();
		delete [] obuffer;
	}
//@}

	///\brief Allocation memory and initialization buffer.
	void open (
		  const char* ident_  ///\param ident_ Identification for openlog(3)
		, const opt logopt_   ///\param logopt_ Log options
		, const fac facility_ ///\param facility_ Log facility
	) {
		if (is_open()) close();

		_state.logopt = logopt_;
		_state.facility = facility_;

		::openlog (ident_, _state.logopt, _state.facility);
		level (_state.current_priority);
		this->setp (obuffer, obuffer+STREAMBUF_SIZE-1);

		_is_open = true ;
	}

	///\brief Close buffer
	void close ()
	{
		if ( !is_open() ) return;
		sync ();
		this->setp (0,0);
		::closelog ();
		_is_open = false;
	}

	///\brief Get state
	///\return Pointer to state
	log_state* state () { return &_state; }

	///\brief Is open
	///\return True if it is open
	bool is_open () const { return _is_open; }

	///\brief Set priority of the logs
	void level ( pri pri_ ///\param pri_ Set priority of logs
	) {
		state()->current_priority = pri_;
		::setlogmask (0);
		::setlogmask (LOG_UPTO (pri_));
	}

protected:
///\name Base IO
//@{
	///\brief Sync output
	///\return non-zero if error 
	virtual int_type sync ();

	///\brief Overflow output
	///\return Last character of EOF
	virtual int_type overflow (
		  int_type c ///\param c Last character
	);
//@}

private:
	logostrmbuf<C,T> (const logostrmbuf<C,T>&);
	logostrmbuf<C,T>& operator=(const logostrmbuf<C,T>&);

	C* obuffer;
	log_state _state;
	bool _is_open;

}; //class logostrmbuf

//-begin of logostrmbuf code 
//
template <typename C, typename T>
typename
logostrmbuf<C,T>::int_type
 logostrmbuf<C,T>::sync ()
{
	const std::streamsize count = this->pptr() - this->pbase();

	if (count > 0)
	{
		*(this->pptr()) = '\0';
		::syslog (_state.priority,  this->pbase());
		this->pbump (-count);
	}
	return 0;
}

//
template <typename C, typename T>
typename
logostrmbuf<C,T>::int_type
 logostrmbuf<C,T>::overflow (int_type c)
{
	if ( is_open ()) {
		if ( c != EOF ) {
			*(this->pptr()) = c;
			this->pbump(1) ;
		}
		sync ();
	}
	return c;
}

//-end of logostrmbuf code 

///\class logios 
///\brief Base IOS for log::ostream

template <typename C, typename T = std::char_traits<C> >
class logios
	: virtual public std::basic_ios<C,T>
{
protected:
	///\brief Constructor by default
	logios ()
		: std::basic_ios<C,T>()
		, obuf()
	{
		this->init ( std::cout.rdbuf ()); }

	///\brief Open to syslogd(3)
	explicit logios (
		  const char* ident_ ///\param ident_ Identification for ::openlog(3)
		, const opt logopt_ ///\param logopt_ Log options
		, const fac facility_ ///\param Log facility
	)
		: std::basic_ios<C,T> (0)
		, obuf (ident_, logopt_, facility_)
	{}

	///\brief Destructor
	virtual ~logios () { this->close(); }

	logostrmbuf<C,T> obuf; ///< Stream buffer

public:
	///\brief Open log::ostream buffer
	void open (
		  const char* ident_ ///\param ident_ Identification for openlog(3)
		, const opt logopt_ = opt_pid ///\param logopt_ Log options
		, const fac facility_ = fac_user ///\param Log facility
	) {
		obuf.open (ident_, logopt_, facility_);
		this->init (&obuf);
	}

	///\brief Close log::ostream buffer
	void close ()
	{
		obuf.close ();
		this->init ( std::cout.rdbuf());
	}

	///\brief Is open
	///\return True if is open

	bool is_open () const { return obuf.is_open (); }

	///\brief Get pointer to stream buffer
	///\return Pointer to the buffer

	logostrmbuf<C,T>* rdbuf () { return &obuf; }

}; //.class logios

///\class logostream 
///\brief Log output stream

template <typename C, typename T = std::char_traits<C> >
class logostream
	: public std::basic_ostream<C,T>
	, public logios<C,T>
{
private:

	typedef std::basic_ostream<C,T> ostream_type;
	typedef logios<C,T> base_ios;

	using base_ios::obuf;

public:
	///\brief Constructor by default
	logostream () : ostream_type(0), base_ios() {}

	///\brief Open to the syslogd
	explicit
	logostream (
		  const char* ident_ ///\param ident_ Identification to openlog(3)
		, const opt logopt_ = ///\param logopt_ Log options
						(opt)( opt_nodelay | opt_pid )
		, const fac facility_ = fac_user ///\param Log facility
	)
		: ostream_type(0)
		, base_ios (ident_, logopt_, facility_)
	{}

///\name Set level and options
//@{
	///\brief Set level of the logs
	logostream<C,T>& level ( pri pri_ ///\param pri_ Output level priority
	) {
		base_ios::rdbuf()->level(pri_);
		return *this;
	}

	///\brief Set level of state
	logostream<C,T>& logopt (
		opt opt_ ///\param opt_ syslog() option
	) {
		base_ios::rdbuf()->state()->logopt = opt_;
		return *this;
	}

	///\brief Check current level of state
	///\return True if priority is less or is equal current level
	bool is ( pri pri_ ///\param pri_ Check level priority
	) {
		if ( base_ios::rdbuf()->state()->current_priority < pri_)
			return false;
		else
			return true;
	}

	///\brief Check current priority
	///\return True if priority is highest then current_priority
	operator bool ()
	{
		if ( base_ios::rdbuf()->state()->current_priority >=\
			 base_ios::rdbuf()->state()->priority
		)
			return true;
		else
			return false;
	}
//@}

///\name Manipulators of priority
//@{
	///\brief Manipulator
	///\return Himself
	logostream<C,T>& operator<< (
		logostream<C,T>& (*function) (logostream<C,T>&) ///\param function, The
	) {
		return function (*this);
	}

	///\brief Level EMERG 
	logostream<C,T>& emerg (logostream<C,T>& out_) { return out_.emerg(); }

	///\brief Set priority to emerg
	///\return Himself
	logostream<C,T>& emerg ()
	{
		base_ios::rdbuf()->state()->priority = level_emerg;
		return *this;
	}//-----------------------------------

	///\brief Level ALERT
	logostream<C,T>& alert (logostream<C,T>& out_) { return out_.alert(); }

	///\brief Set priority to alert
	///\return Himself
	logostream<C,T>& alert ()
	{
		base_ios::rdbuf()->state()->priority = level_alert;
		return *this;
	}//-----------------------------------

	///\brief Level CRITICAL
	logostream<C,T>& critical (logostream& out_) { return out_.critical(); }

	///\brief Set priority to critical
	logostream<C,T>& critical ()
	{
		base_ios::rdbuf()->state()->priority = level_critical;
		return *this;
	}//------------------------------------

	///\brief Level ERROR
	logostream<C,T>& error (logostream<C,T>& out_) { return out_.error(); }

	///\brief Set priority to info
	///\return Himself
	logostream<C,T>& error ()
	{
		base_ios::rdbuf()->state()->priority = level_error;
		return *this;
	}//-----------------------------------

	///\brief Level WARNING
	logostream<C,T>& warning (logostream<C,T>& out_) { return out_.warning(); }

	///\brief Set priority to warning
	///\return Himself
	logostream<C,T>& warning ()
	{
		base_ios::rdbuf()->state()->priority = level_warning;
		return *this;
	}//-----------------------------------

	///\brief Level NOTICE
	logostream<C,T>& notice (logostream<C,T>& out_) { return out_.notice(); }

	///\brief Set priority to notice
	///\return Himself
	logostream<C,T>& notice ()
	{
		base_ios::rdbuf()->state()->priority = level_notice;
		return *this;
	}//-----------------------------------

	///\brief Level INFO
	logostream<C,T>& info (logostream<C,T>& out_) { return out_.info(); }

	///\brief Set priority to info
	///\return Himself
	logostream<C,T>& info ()
	{
		base_ios::rdbuf()->state()->priority = level_info;
		return *this;
	}//-----------------------------------

	///\brief Level DEBUG
	logostream<C,T>& debug (logostream<C,T>& out_) { return out_.debug(); }

	///\brief Set priority to debug
	///\return Himself
	logostream<C,T>& debug ()
	{
		base_ios::rdbuf()->state()->priority = level_debug;
		return *this;
	}//-----------------------------------

//@}

}; //.class logostream

///\name Outside level manipulators
//@{
//
template <typename C, typename T>
inline
 logostream<C,T>& emerg (logostream<C,T>& out_) { return out_.emerg(); }
//
template <typename C, typename T>
inline
 logostream<C,T>& alert (logostream<C,T>& out_) { return out_.alert(); }
//
template <typename C, typename T>
inline
 logostream<C,T>& critical (logostream<C,T>& out_) { return out_.critical(); }
//
template <typename C, typename T>
inline
 logostream<C,T>& error (logostream<C,T>& out_) { return out_.error(); }
//
template <typename C, typename T>
inline
 logostream<C,T>& warning (logostream<C,T>& out_) { return out_.warning(); }
//
template <typename C, typename T>
inline
 logostream<C,T>& notice (logostream<C,T>& out_) { return out_.notice(); }
//
template <typename C, typename T>
inline
 logostream<C,T>& info (logostream<C,T>& out_) { return out_.info(); }
//
template <typename C, typename T>
inline
 logostream<C,T>& debug (logostream<C,T>& out_) { return out_.debug(); }
//@}

///\name Manipulator with arguments implementation
//@{

///\class argmanip 
///\brief Manipulator with arguments

template <typename C, typename T, typename Type>
struct argmanip {

	///\brief Callback function
	///\param func_ This is it

	logostream<C,T>& (*func) (logostream<C,T>&, Type);
	Type arg; ///< Argument of manipulator

	///\brief Constructor
	argmanip (logostream<C,T>& (*f) (logostream<C,T>&, Type), Type arg_)
		: func(f)
		, arg (arg_)
	{}
}; //.class argmanip

/**************
///\brief Output manipulator with argument
template <class Type>
inline
logostream<C,T>& operator<< (
	  logostream<C,T>& outs_ ///\param outs_ Output stream
	, const argmanip<Type>& manip_ ///\param manip_ Manipulator with args
) {
	return manip_.func (outs_, manip_.arg);
}
//@}

///\name Manupulator of level
//@{
///\brief Prefix call to level
inline
logostream& set_level (
	  logostream& outs_ ///\param bios Base IO stream
	, pri arg ///\param arg Argument of manipulator
) {
	return outs_.level (arg);
}

///\brief Level object-function
inline argmanip<pri> level (pri arg) { return argmanip<pri> (set_level, arg); }
//@}

///\name Manupulator of log options
//@{
///\brief Prefix call to logopt
inline
logostream& set_logopt (
	  logostream& outs_ ///\param outs_ IO stream
	, opt arg ///\param arg Argument of manipulator
) {
	return outs_.logopt (arg);
}

///\brief Logopt object-function
inline argmanip<opt> logopt (opt arg) { return argmanip<opt>(set_logopt,arg); }
//@}
****************/

extern logostream<char> logs; ///< Global logging stream

} //::cxx

#endif ///!CXX_LOGOSTREAM_HPP
