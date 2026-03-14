#include "main.h"

string mettingsFile = "/.zoom_meeting";

static void showMeetingList(list<Meeting>& meetings) {
	if (meetings.empty())
	{
		cout << "No rooms found" << endl;
	}
	else
	{
		for (const auto& meeting : meetings)
		{
			cout << meeting << endl;
		}
	}
}

static void readmMeetingsFile(list<Meeting>& meetings) {
	string home = getHomeDirectory();
	ifstream file(home + mettingsFile);

	if (file.is_open())
	{
		string line, name, conf, pwd;

		while (getline(file, line))
		{
			name = "";
			conf = "";
			pwd = "";
			stringstream ss(line);

			try
			{
				ss >> name >> conf >> pwd;
				Meeting metting(name, conf, pwd);
				meetings.push_back(metting);
			}
			catch (const exception& e)
			{
				cerr << "Error: " << e.what() << endl;
			}
		}
		file.close();
	}
}

static void addRoom(list<Meeting>& meetings, string name, string conf, string pwd)
{
	Meeting newMeeting(name, conf, pwd);

	// search if room exist in meetings, by name
	for (const auto& meeting : meetings)
	{
		if (meeting.getName().compare(newMeeting.getName()) == 0)
		{
			cout << "Room already exist" << endl;
			return;
		}
	}

	// add the room to file rooms
	string home = getHomeDirectory();
	ofstream file(home + mettingsFile, ios::app);
	file << newMeeting.getName() << " " << newMeeting.getConf();

	if (!newMeeting.getPwd().empty())
	{
		file << " " << newMeeting.getPwd();
	}

	file << endl;
	file.close();
}

static void deleteRoom(list<Meeting>& meetings, string name)
{
	list<Meeting> remaining;
	bool found = false;

	for (const auto& meeting : meetings)
	{
		if (meeting.getName().compare(name) == 0)
		{
			found = true;
		}
		else
		{
			remaining.push_back(meeting);
		}
	}

	if (!found)
	{
		cout << "Room not found" << endl;
		return;
	}

	string home = getHomeDirectory();
	ofstream file(home + mettingsFile);
	for (const auto& meeting : remaining)
	{
		file << meeting.getName() << " " << meeting.getConf();
		if (!meeting.getPwd().empty())
		{
			file << " " << meeting.getPwd();
		}
		file << endl;
	}
	file.close();

	cout << "Room \"" << name << "\" deleted" << endl;
}

static void executeZoomCL(string zoomCL)
{
	system(zoomCL.c_str());
}


int main(int argc, char** argv) {
	list<Meeting> meetings;

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

		// Delete room
		if (strcmp(argv[1], "-d") == 0 || strcmp(argv[1], "--delete") == 0) {
			if (argc == 3) {
				deleteRoom(meetings, argv[2]);
			} else {
				cout << "Usage: zoom -d <name>" << endl;
			}
			return 0;
		}

		// Add room
		if (strcmp(argv[1], "-a") == 0 || strcmp(argv[1], "--add") == 0) {
			if (argc == 5) {
				addRoom(meetings, argv[2], argv[3], argv[4]);
			}
			else if (argc == 4) {
				if (strncmp(argv[3], "https", 5) == 0) {
					string room = extractRoom(argv[3]);
					addRoom(meetings, argv[2], room.substr(0, room.find('|')), room.substr(room.find('|') + 1));
				}
				else {
					addRoom(meetings, argv[2], argv[3], "");
				}
			}
			else {
				cout << "Usage: zoom -a <name> <conf> [<pwd>] | zoom -a <name> <url>" << endl;
			}
			return 0;
		}


		// List rooms registered
		for (const auto& room : meetings) {
			if (room.getName().compare(argv[1]) == 0) {
				executeZoomCL(createZoomCL(room.getConf(), room.getPwd()));
				return 0;
			}
		}

		if (argc == 3) {
			executeZoomCL(createZoomCL(argv[1], argv[2]));
			return 0;
		}

		if (strncmp(argv[1], "https", 5) == 0) {
			string conf = extractRoom(argv[1]);
						
			executeZoomCL(createZoomCL(conf.substr(0, conf.find('|')), conf.substr(conf.find('|') + 1)));
			return 0;
		}
	}
	catch (const exception& e) {
		cerr << e.what() << '\n';
	}

	return 0;
}