#include <iostream>

#include "parser.h"

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

using std::get;


TEST_CASE("Simple Strings", "[simple string]") {
	REQUIRE(ParseRESP("+SimpleString\r\n") == "\"SimpleString\"");
	REQUIRE(ParseRESP("+\r\n") == "\"\"");
}

TEST_CASE("Simple Strings internal", "[simple string]") {
	SECTION("empty string") {
		auto result = ParseSimpleString("+\r\n");
		REQUIRE(get<1>(result) == 3);
	}

	SECTION("regular string") {
		auto result = ParseSimpleString("+regular string\r\n");
		REQUIRE(get<1>(result) == 17);
	}
}

TEST_CASE("Errors", "[error]") {
	REQUIRE(ParseRESP("-SomeError\r\n") == "Error:\"SomeError\"");
}

TEST_CASE("Errors internal", "[error]") {
	SECTION("emtpy error") {
		auto result = ParseError("-\r\n");
		REQUIRE(get<1>(result) == 3);
	}

	SECTION("regular error") {
		auto result = ParseError("-some error\r\n");
		REQUIRE(get<1>(result) == 13);
	}
}

TEST_CASE("Integers", "[integer]") {
	REQUIRE(ParseRESP(":123\r\n") == "123");
}

TEST_CASE("Integers internal", "[integer]") {
	auto result = ParseInteger(":123456\r\n");
	REQUIRE(get<1>(result) == 9);
}

TEST_CASE("Bulk String", "[bulk string]") {
	REQUIRE(ParseRESP("$12\r\nhello\r\nworld\r\n") == "\"hello\r\nworld\"");
}

TEST_CASE("Bulk String internal", "[bulk string]") {
	SECTION("Null string") {
		auto result = ParseBulkString("$-1\r\n");
		REQUIRE(get<1>(result) == 5);
	}

	SECTION("regular string with carriage return") {
		auto result = ParseBulkString("$12\r\nhello\r\nworld\r\n");
		REQUIRE(get<1>(result) == 19);
	}

}

TEST_CASE("Arrays" ,"[array]") {
	REQUIRE(ParseRESP("*2\r\n*3\r\n$12\r\nbulk\r\nstring\r\n:123\r\n+simple string\r\n-bad things\r\n") ==
		    "[[\"bulk\r\nstring\", 123, \"simple string\"], Error:\"bad things\"]");
	REQUIRE(ParseRESP("*-1\r\n") == "Null");
}

TEST_CASE("Arrays internal", "[array]") {
	SECTION("Null array") {
		auto result = ParseArray("*-1\r\n");
		REQUIRE(get<1>(result) == 5);
	}

	SECTION("regular array") {
		auto result = ParseArray("*2\r\n:123\r\n+string\r\n");
		REQUIRE(get<1>(result) == 19);
	}

	SECTION("nested array") {
		auto result = ParseArray("*3\r\n:123\r\n+string\r\n*2\r\n$11\r\nhello\nworld\r\n:123\r\n");
		REQUIRE(get<1>(result) == 47);
	}
}