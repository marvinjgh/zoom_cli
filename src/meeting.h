#ifndef MEETING_H
#define MEETING_H

#include <ostream>
#include <string>

/*
	This class containg the minimal info of a Meeting
*/
class Meeting {
public:
	Meeting(const std::string& name, const std::string& conf, const std::string& pwd);

	const std::string& getName() const;
	const std::string& getConf() const;
	const std::string& getPwd() const;

private:
	std::string name;
	std::string conf;
	std::string pwd;
};

std::ostream& operator<<(std::ostream& out, Meeting Meeting);

#endif