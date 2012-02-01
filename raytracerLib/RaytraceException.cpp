/* 
 * RaytraceException.cpp
 * Ben Hart
 */

#include "GraphicsException.h"

RaytraceException::RaytraceException(std::string what)
{
	m_what = what;
}


RaytraceException::RaytraceException(const RaytraceException& orig)
{
	m_what = orig.m_what;
}


RaytraceException::~RaytraceException() throw()
{
}


const char *RaytraceException::what() const throw()
{
	return (m_what.c_str());
}

