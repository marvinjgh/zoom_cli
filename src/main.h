#pragma once

#include <cstring>
#include <fstream>
#include <iostream>
#include <list>
#include <sstream>

#include "meeting.h"
#include "utils.h"

using namespace std;

static void showMeetingList(list<Meeting>& meetings);
static void readmMeetingsFile(list<Meeting>& meetings);
static void addRoom(list<Meeting>& meetings, string name, string conf, string pwd);
static void deleteRoom(list<Meeting>& meetings, string name);
static void executeZoomCL(string zoomCL);