/* 
 * RaytraceException.h
 * Ben Hart
 */

#pragma once

#include <string>

class EngineException : public std::exception
{
public:
	/**
	 * Initializes the exception with the given message.
     * @param what The message.
     */
	EngineException(std::string what = "exception!");

	EngineException(const RaytraceException& orig);
	~EngineException() throw();

	const char* what() const throw();
private:
	std::string m_what;
};
