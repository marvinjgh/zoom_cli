#include "main.h"

std::string mettingsFile = "/.zoom_meeting";

static void showMeetingList(std::list<Meeting>& meetings) {
	if (meetings.empty())
	{
		std::cout << "No rooms found" << std::endl;
	}
	else
	{
		for (const auto& meeting : meetings)
		{
			std::cout << meeting << std::endl;
		}
	}
}

static void readmMeetingsFile(std::list<Meeting>& meetings) {
	std::string home = getHomeDirectory();
	std::ifstream file(home + mettingsFile);

	if (file.is_open())
	{
		std::string line, name, conf, pwd;

		while (getline(file, line))
		{
			name = "";
			conf = "";
			pwd = "";
			std::stringstream ss(line);

			try
			{
				ss >> name >> conf >> pwd;
				Meeting metting(name, conf, pwd);
				meetings.push_back(metting);
			}
			catch (const std::exception& e)
			{
				std::cerr << "Error: " << e.what() << std::endl;
			}
		}
		file.close();
	}
}

static void addRoom(std::list<Meeting>& meetings, std::string name, std::string conf, std::string pwd)
{
	Meeting newMeeting(name, conf, pwd);

	// search if room exist in meetings, by name
	for (const auto& meeting : meetings)
	{
		if (meeting.getName().compare(newMeeting.getName()) == 0)
		{
			std::cout << "Room already exist" << std::endl;
			return;
		}
	}

	// add the room to file rooms
	std::string home = getHomeDirectory();
	std::ofstream file(home + mettingsFile, std::ios::app);
	file << newMeeting.getName() << " " << newMeeting.getConf();

	if (!newMeeting.getPwd().empty())
	{
		file << " " << newMeeting.getPwd();
	}

	file << std::endl;
	file.close();
}

static void executeZoomCL(std::string zoomCL) {
	system(strdup(zoomCL.c_str()));
}


int main(int argc, char** argv) {
	std::list<Meeting> meetings;

	try {
		if (argc > 5 || argc < 2) {
			printHelp();
			return 0;
		}

		// Help
		if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
			printHelp();
			return 0;
		}

		readmMeetingsFile(meetings);

		// List rooms registered
		if (strcmp(argv[1], "-l") == 0 || strcmp(argv[1], "--list") == 0) {
			showMeetingList(meetings);
			return 0;
		}

		// Add room
		if (strcmp(argv[1], "-a") == 0 || strcmp(argv[1], "--add") == 0) {
			if (argc == 5) {
				addRoom(meetings, argv[2], argv[3], argv[4]);
			}
			else if (argc == 4) {
				addRoom(meetings, argv[2], argv[3], "");
			}
			else {
				std::cout << "Usage: zoom -a <name> <conf> [<pwd>]" << std::endl;
			}
			return 0;
		}


		// List rooms registered
		for (const auto& room : meetings) {
			if (room.getName().compare(argv[1]) == 0) {
				// Use platform-specific execution (e.g., `system`, `shellexecute`, `open`)
				// Replace with appropriate function for your platform
				executeZoomCL(createZoomCL(room.getConf(), room.getPwd()));
				return 0;
			}
		}

		if (argc == 3) {
			// Use platform-specific execution (e.g., `system`, `shellexecute`, `open`)
			// Replace with appropriate function for your platform
			executeZoomCL(createZoomCL(argv[1], argv[2]));
			return 0;
		}

		if (strncmp(argv[1], "https", 5) == 0) {
			std::string conf = extractRoom(argv[1]);
			// Use platform-specific execution (e.g., `system`, `shellexecute`, `open`)
			// Replace with appropriate function for your platform
			executeZoomCL(createZoomCL(conf.substr(0, conf.find('|')), conf.substr(conf.find('|') + 1)));
			return 0;
		}

		//system(createZoomCL(argv[1], ""));
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << '\n';
	}

	return 0;
}