#include <iostream>
#include <fstream>
#include <sstream>
#include <list>
#include <unistd.h>
#include <pwd.h>
#include "room.h"

using std::cout;
using std::endl;
using std::ifstream;
using std::list;
using std::ofstream;
using std::string;
using std::stringstream;

struct passwd *pw = getpwuid(getuid());

/**
 * Print the help message
 */
void printHelp()
{
  cout << "usage: zoom [-h | --help] [-l | --list] [(-a|--add) <name> <conf> [pwd]] [<conf> [pwd]] [name]" << endl
     << endl;
  cout << "Options:" << endl;
  cout << "-h | --help\t\t show this description" << endl;
  cout << "-l | --list\t\t show the list of cenference room registers" << endl;
  cout << "-a | --add\t\t add a new conference room register" << endl;
  cout << "name\t\t the zoom conference room number, or name that is saved in the file ~/.zoom_rooms" << endl;
  cout << "conf\t\t the zoom conference room number" << endl;
  cout << "pwd\t\t password to enter in the conference room" << endl
     << endl;
  cout << "url open \"zoommtg://zoom.us/join?confno=<conf>&pwd=<pwd>\"" << endl;
}

char *createZoomCL(string conf, string pwd)
{
  stringstream ss;
  #if defined(_WIN32) || defined(_WIN64)
    // TODO
  #elif defined(__APPLE__)
    ss << "open \"zoommtg://zoom.us/join?confno=" << conf;
    if (!pwd.empty())
    {
      ss << "&pwd=" << pwd;
    }
    ss << "\"";
  #elif defined(__linux__)
    // TODO
  #else
    // TODO
  #endif

  return strdup(ss.str().c_str());
}

void readZoomRoomsFile(list<Room> &rooms)
{
  string home = pw->pw_dir;
  ifstream file(home + "/.zoom_rooms");

  if (file.is_open())
  {
    string line;
    string name = "";
    string conf = "";
    string pwd = "";
    while (getline(file, line))
    {
      name = "";
      conf = "";
      pwd = "";
      stringstream ss(line);
      
      try
      {
        ss >> name >> conf >> pwd;
        Room room(name, conf, pwd);
        rooms.push_back(room);
      }
      catch (const std::exception &e)
      {
        std::cerr << "Error: " << e.what() << endl;
      }
    }
    file.close();
  }
}

void showList(list<Room> &rooms)
{
  if (rooms.empty())
  {
    cout << "No rooms found" << endl;
  }
  else
  {
    for (const auto &room : rooms)
    {
      cout << room << endl;
    }
  }
}

void addRoom(list<Room> &rooms, string name, string conf, string pwd)
{
  Room room(name, conf, pwd);
  
  // search if room exist in rooms, by name
  for (const auto &r : rooms)
  {
    if (r.name.compare(room.name) == 0)
    {
      cout << "Room already exist" << endl;
      return;
    }
  }
  
  // add the room to file rooms
  string home = pw->pw_dir;
  ofstream file(home + "/.zoom_rooms", std::ios::app);
  file << room.name << " " << room.conf;
  
  if (!room.pwd.empty())
  {
    file << " " << room.pwd; 
  }
  
  file << endl;
  file.close();
}

string extractRoom(string url){
    // extract the conf and pwd from the url
    // example: https://zoom.us/j/1234567890?pwd=password
    // conf=1234567890
    // pwd=password
    
    string conf = "";
    string pwd = "";
    string url2 = url;
    size_t pos = url.find("/j/");
    if (pos != string::npos)
    {
        url2 = url2.substr(pos + 3);
        pos = url2.find("pwd=");
        if (pos != string::npos)
        {
            conf = url2.substr(0, pos);
            pwd = url2.substr(pos + 4);
        }
        else
        {
            conf = url2;
        }
    }
    return conf + '|'+ pwd;
}

int main(int argc, char **argv)
{
  list<Room> rooms;
  try
  {
    if (argc > 5 || argc < 2)
    {
      printHelp();
      return 0;
    }

    // Help
    if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0)
    {
      printHelp();
      return 0;
    }
    readZoomRoomsFile(rooms);

    // List rooms registed
    if (strcmp(argv[1], "-l") == 0 || strcmp(argv[1], "--list") == 0)
    {
      showList(rooms);
      return 0;
    }
    
    // Add room
    if (strcmp(argv[1], "-a") == 0 || strcmp(argv[1], "--add") == 0)
    {
      if (argc == 5)
      {
        addRoom(rooms, argv[2], argv[3], argv[4]);
      }else if (argc == 4){
        addRoom(rooms, argv[2], argv[3], "");
      } else{
        cout << "Usage: zoom -a <name> <conf> [<pwd>]" << endl;
      }
      return 0;
    }

    // List rooms registed
    for (const auto &room : rooms)
    {
      if (room.name.compare(argv[1]) == 0)
      {
        system(createZoomCL(room.conf, room.pwd));
        return 0;
      }
    }
    if (argc == 3)
    {
        system(createZoomCL(argv[1], argv[2]));
        return 0;
    }

    if (strncmp(argv[1], "https", 5)  == 0)
    {
        string conf = extractRoom(argv[1]);
        system(createZoomCL(conf.substr(0, conf.find('|')), conf.substr(conf.find('|')+1)));
        return 0;
    }

    system(createZoomCL(argv[1], ""));
  }
  catch(const std::exception& e)
  {
      std::cerr << e.what() << '\n';
  }

  return 0;
}
