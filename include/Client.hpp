#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>

class Client
{
	private:
		std::string	data;

	public:
		Client();
		Client( const char* str );
		Client( const Client& src );
		Client& operator=( const Client& src );
		~Client();

		void	get() const;
		void	set() const;
};

std::ostream&   operator<<( std::ostream& o, const Client& rhs );

#endif // CLIENT_HPP
