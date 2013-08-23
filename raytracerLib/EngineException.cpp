/* 
 * EngineException.cpp
 * Ben Hart
 */

#include "EngineException.h"


EngineException::EngineException(std::string what)
{
	m_what = what;
}


EngineException::EngineException(const EngineException& orig)
{
	m_what = orig.m_what;
}


EngineException::~EngineException() throw()
{
}


const char *EngineException::what() const throw()
{
	return (m_what.c_str());
}

