#include <catch2/catch_test_macros.hpp>
#include <sstream>

#include "meeting.h"
#include "utils.h"

// --- extractRoom ---

TEST_CASE("extractRoom parses URL with conf and pwd", "[extractRoom]") {
	std::string result = extractRoom("https://zoom.us/j/1234567890?pwd=mypassword");
	REQUIRE(result == "1234567890|mypassword");
}

TEST_CASE("extractRoom parses URL with conf only", "[extractRoom]") {
	std::string result = extractRoom("https://zoom.us/j/1234567890");
	REQUIRE(result == "1234567890|");
}

TEST_CASE("extractRoom returns empty for URL without /j/ segment", "[extractRoom]") {
	std::string result = extractRoom("https://zoom.us/invalid");
	REQUIRE(result == "|");
}

// --- createZoomCL ---

TEST_CASE("createZoomCL includes conf in command", "[createZoomCL]") {
	std::string cmd = createZoomCL("12345", "");
	REQUIRE(cmd.find("12345") != std::string::npos);
}

TEST_CASE("createZoomCL includes pwd when provided", "[createZoomCL]") {
	std::string cmd = createZoomCL("12345", "mypassword");
	REQUIRE(cmd.find("&pwd=mypassword") != std::string::npos);
}

TEST_CASE("createZoomCL omits pwd when empty", "[createZoomCL]") {
	std::string cmd = createZoomCL("12345", "");
	REQUIRE(cmd.find("&pwd=") == std::string::npos);
}

// --- Meeting ---

TEST_CASE("Meeting stores name, conf, and pwd", "[Meeting]") {
	Meeting m("work", "12345", "password");
	REQUIRE(m.getName() == "work");
	REQUIRE(m.getConf() == "12345");
	REQUIRE(m.getPwd() == "password");
}

TEST_CASE("Meeting stores empty pwd", "[Meeting]") {
	Meeting m("work", "12345", "");
	REQUIRE(m.getPwd() == "");
}

TEST_CASE("Meeting operator<< with pwd", "[Meeting]") {
	Meeting m("work", "12345", "pwd");
	std::ostringstream oss;
	oss << m;
	REQUIRE(oss.str() == "Meeting ( work, 12345, pwd )");
}

TEST_CASE("Meeting operator<< without pwd", "[Meeting]") {
	Meeting m("work", "12345", "");
	std::ostringstream oss;
	oss << m;
	REQUIRE(oss.str() == "Meeting ( work, 12345 )");
}
