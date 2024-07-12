#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <iostream>

class Channel
{
	private:
		std::string	data;

	public:
		Channel();
		Channel( const char* str );
		Channel( const Channel& src );
		Channel& operator=( const Channel& src );
		~Channel();

		void	get() const;
		void	set() const;
};

std::ostream&   operator<<( std::ostream& o, const Channel& rhs );

#endif // CHANNEL_HPP
