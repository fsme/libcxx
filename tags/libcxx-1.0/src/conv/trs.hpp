//@(#) Translate char_type
// Author: Andrew Wingorodov <http://andr.mobi/>
// Licence: LGPLv3
//$Id: trs.hpp 247 2008-02-08 14:48:36Z wingorodov $

#ifndef _CXX_TRS_H
#define _CXX_TRS_H 1

#include <string>

using namespace std;

namespace cxx {

///\class trs
///\brief Translate char_type

template <typename C, typename T = std::char_traits<C> >
class trs
{
public:
typedef typename std::basic_string<C,T>	string_type;
typedef typename string_type::iterator	iter;

	///\brief Constructor with connect to single server
	explicit trs (
	  const string_type& from_///\param from_ String translate FROM 
	, const string_type& to_///\param to_ String translate TO
	)
		: _from (from_) 
		, _to (to_)
	{
		if (from_.size() != to_.size())
			throw std::invalid_argument ("size from isnt equal to");
	}

	///\brief Translate
	void
		translate (
			  iter ptr_ ///\param ptr_ Begin translate
			, iter end_ ///\param end_ End translate
		) {
			size_t pos;

			for (; ptr_ != end_; ++ptr_) {
				if ( (pos=_from.find (*ptr_)) != string_type::npos)
					*ptr_ = _to [pos];
			}
		}

private:
	string_type	_from;
	string_type	_to;
};

}//::cxx

#endif ///_CXX_TRS.H
