#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>

class utils
{
	private:
		std::string	data;

	public:
		utils();
		~utils();

		void	get() const;
		void	set() const;

		// std::string	preParsing( std::string rawStr );
};

std::ostream&   operator<<( std::ostream& o, const utils& rhs );

#endif // UTILS_HPP
