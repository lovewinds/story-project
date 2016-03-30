#include "util/LogHelper.hpp"
#include "resource/ESceneInfo.hpp"

ESceneInfo::ESceneInfo(std::string name)
{
	LOG_INFO("ESceneInfo[%s] created", name.c_str());
	this->name = name;
}

ESceneInfo::~ESceneInfo()
{
	LOG_INFO("ESceneInfo[%s] removed", name.c_str());
}

std::string ESceneInfo::getName()
{
	return name;
}

bool ESceneInfo::addImageResource(std::string& path)
{
	return true;
}

bool ESceneInfo::addSprite(std::string& path)
{
	return true;
}
