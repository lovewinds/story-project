#include <iostream>

#include "json/json.h"
#include "util/LogHelper.hpp"
#include "util/JsonHelper.hpp"

std::string str;

void createJson()
{
        Json::Value root;
        root["id"] = "Luna";
        root["name"] = "Silver";
        root["age"] = 19;
        root["hasCar"] = false;

        Json::Value items;
        items.append("nootbook");
        items.append("ipadmini2");
        items.append("iphone5s");
        root["items"] = items;

        Json::Value friends;
        Json::Value tom;
        tom["name"] = "Tom";
        tom["age"] = 21;
        Json::Value jane;
        jane["name"] = "jane";
        jane["age"] = 23;
        friends.append(tom);
        friends.append(jane);
        root["friends"] = friends;

        Json::StyledWriter writer;
        str = writer.write(root);
        std::cout << str << std::endl << std::endl;
}

void readJson()
{
	Json::Reader reader;
	Json::Value root;
	bool parsingRet = reader.parse(str, root);

	if (!parsingRet)
	{
		std::cout << "Failed to parse Json : " << reader.getFormattedErrorMessages();
		return;
	}

	std::cout << root["hasCar"] << " : " << root["age"] << std::endl << std::endl;

	const Json::Value items = root["items"];
	for (int i = 0; i < items.size(); i++)
	{
		std::cout << items[i].asString() << std::endl;
	}
	std::cout << std::endl;

	auto member = root.getMemberNames();

	for (std::string s : member)
	{
		std::cout << s << std::endl;
	}
	std::cout << std::endl;

	for (std::string s : member)
	{
		if (root[s].isString())
		{
			std::cout << root[s] << std::endl;
		}
	}
	std::cout << std::endl;

	Json::Value friends = root["friends"];
	for (auto it = friends.begin(); it != friends.end(); it++)
	{
		if (it->isObject())
		{
			std::cout << (*it)["name"] << " : " << (*it)["age"] << std::endl;
		}
	}
	std::cout << std::endl;
}
