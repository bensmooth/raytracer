/* 
 * RaytraceException.h
 * Ben Hart
 */

#pragma once

#include <string>

class RaytraceException : public std::exception
{
public:
	/**
	 * Initializes the exception with the given message.
     * @param what The message.
     */
	RaytraceException(std::string what = "exception!");

	RaytraceException(const RaytraceException& orig);
	~RaytraceException() throw();

	const char* what() const throw();
private:
	std::string m_what;
};
