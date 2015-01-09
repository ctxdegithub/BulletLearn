#include "PhysicsHelper3D.h"

std::vector<float> PhysicsHelper3D::loadRaw(const char* fileName)
{
	std::vector<float> data;
	if (loadRaw(fileName, data))
	{
		return data;
	}

	return std::vector<float>(0);
}

bool PhysicsHelper3D::loadRaw(const char* fileName, std::vector<float>& verts)
{
	char line[1024];
	
	float oneData;
	auto rawData = FileUtils::getInstance()->getStringFromFile(fileName);
	std::stringstream ss, ssLine;
	ss << rawData;
	while (ss.getline(line, 1024))
	{
		ssLine << line;
		for (int i = 0; i < 9; i++)
		{
			ssLine >> oneData;
			verts.push_back(oneData);
		}
	}
	return true;
}