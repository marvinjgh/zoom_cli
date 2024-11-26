#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <string>
#include <sstream>

/*
	Helper to get home directory
*/
std::string getHomeDirectory();

/*
	Helper to create the zoom meeting command line
*/
std::string createZoomCL(const std::string& conf, const std::string& pwd);

/*
	Helper to get the zoom meeting info, from a URL
*/
std::string extractRoom(const std::string& url);

/*
	Print the command line options
*/
void printHelp();

#endif