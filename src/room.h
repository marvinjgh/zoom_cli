#include <string>
#include <ostream>

using std::string;

class Room
{
public:
  string name;
  string conf;
  string pwd;
  Room(string name, string conf, string pwd)
  {
    this->name = name;
    this->conf = conf;
    this->pwd = pwd;
  }
  ~Room() {}
};

std::ostream &operator<<(std::ostream &out, const Room &room)
{

  if (!room.pwd.empty())
  {
    return out << "Room ( " << room.name << ", " << room.conf << ", " << room.pwd << " )";
  }
  else
  {
    return out << "Room ( " << room.name << ", " << room.conf << " )";
  }
}