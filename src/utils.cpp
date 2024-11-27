#include "utils.h"

std::string getHomeDirectory() {
#if defined(_WIN32) || defined(_WIN64)
	return getenv("HOMEPATH");
#else
	return getenv("HOME");
#endif
}

std::string createZoomCL(const std::string& conf, const std::string& pwd) {
	std::stringstream ss;

#if defined(_WIN32) || defined(_WIN64)
	ss << "%APPDATA%\\Zoom\\bin\\Zoom.exe \"--url=zoommtg://zoom.us/join?confno=";
#elif defined(__APPLE__)
	ss << "open \"zoommtg://zoom.us/join?confno=";
#elif defined(__linux__)
	// TODO: Implement for Linux
	ss << "xdg-open \"zoommtg://zoom.us/join?confno="
#endif

	ss << conf;
	if (!pwd.empty()) {
		ss << "&pwd=" << pwd;
	}
	ss << "\"";
	return ss.str();
}

std::string extractRoom(const std::string& url) {
	// Extract the conf and pwd from the URL
	// Example: https://zoom.us/j/1234567890?pwd=password
	// conf=1234567890
	// pwd=password

	std::string conf, pwd;
	std::string url2 = url;
	size_t pos = url.find("/j/");
	if (pos != std::string::npos) {
		url2 = url2.substr(pos + 3);
		pos = url2.find("pwd=");
		if (pos != std::string::npos) {
			conf = url2.substr(0, pos);
			pwd = url2.substr(pos + 4);
		}
		else {
			conf = url2;
		}
	}
	return conf + '|' + pwd;
}

void printHelp()
{
	std::cout << "usage: zoom [-h | --help] [-l | --list] [(-a|--add) <name> <conf> [pwd]] [<conf> [pwd]] [name]" << std::endl
		<< std::endl;
	std::cout << "Options:" << std::endl;
	std::cout << "-h | --help\t\t show this description" << std::endl;
	std::cout << "-l | --list\t\t show the list of cenference room registers" << std::endl;
	std::cout << "-a | --add\t\t add a new conference room register" << std::endl;
	std::cout << "name\t\t the zoom conference room number, or name that is saved in the file ~/.zoom_meeting" << std::endl;
	std::cout << "conf\t\t the zoom conference room number" << std::endl;
	std::cout << "pwd\t\t password to enter in the conference room" << std::endl
		<< std::endl;
	std::cout << "url \"zoommtg://zoom.us/join?confno=<conf>&pwd=<pwd>\"" << std::endl;
}