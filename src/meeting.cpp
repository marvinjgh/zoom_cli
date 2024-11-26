#include "meeting.h"


Meeting::Meeting(const std::string& name, const std::string& conf, const std::string& pwd)
	: name(name), conf(conf), pwd(pwd) {}

const std::string& Meeting::getName() const {
	return name;
}

const std::string& Meeting::getConf() const {
	return conf;
}

const std::string& Meeting::getPwd() const {
	return pwd;
}

std::ostream& operator<<(std::ostream& out, Meeting Meeting) {
	if (!Meeting.getPwd().empty())
	{
		return out << "Meeting ( " << Meeting.getName() << ", " << Meeting.getConf() << ", " << Meeting.getPwd() << " )";
	}
	else
	{
		return out << "Meeting ( " << Meeting.getName() << ", " << Meeting.getConf() << " )";
	}
}