//@(#) Arguments of exec
//@(#) Licence: LGPLv3
//@(#) Author Andrew Wingorodov <http://andr.ru/>
//$Id: args.h,v 1.1.1.1 2007-12-20 15:53:44 wingorodov Exp $

#ifndef _SYS_ARGS_H
#define _SYS_ARGS_H 1

#include <vector>
#include <string>
#include <memory>

namespace sys {

/** @class args
	@brief Container of arguments of exec functions
  */
struct args
{
	std::string path;				///< Path or file name
	std::vector<std::string> argv;	///< List of arguments
	std::vector<std::string> envp;	///< Pair of environment ("variable=value")

	/// \brief Constructor by default
	args () {}

	/// \brief Constructor with argumens
	args (
	  const std::string& path_ /// \param path_ Path to the executed file
	, const std::vector<std::string>& argv_ /// \param argv_ List of arguments
	, const std::vector<std::string>& envp_/// \param envp_  Variable=Value
	)
	: path (path_)
	, argv (argv_)
	, envp (envp_)
	{}

	/// \class constchar
	///	\brief Get const char* [] from vector<string>
	class constchar
	{
	private:
		const char** array;

	public:
		/// \brief Constructor
		constchar (const std::vector<std::string>& arg_);

		/// \brief virtual Destructor
		~constchar () { delete [] array; }

		/// \brief Convert to const char**
		operator const char** () const { return array; }

	}; ///< class constchar

}; ///< class args

} //::sys

#endif //!_SYS_ARGS_H
