#pragma once

#include <string>

/* TODO: Inherit serializable class */
/*
 * This class stores only resource information for actual allocation.
 * All of these ~~Info classes are stored in ResourceManager.
 * When scene is changed, SceneManager allocates actual Scene instance
 * with this SceneInfo instance on ResourceManager.
 */
class ESceneInfo
{
public:
	ESceneInfo(std::string name);
	virtual ~ESceneInfo();

	/* Resource manager allocates image resource */
	bool addImageResource(std::string& path);
	bool addSprite(std::string& path);

	std::string getName();

protected:
	std::string name;
};
