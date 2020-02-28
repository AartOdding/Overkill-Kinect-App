#pragma once

#include <unordered_map>
#include <ofMain.h>

#include <glm/vec2.hpp>


enum class BodyPart
{
	ARM_LEFT = 0,
	ARM_RIGHT = 1,
	CHEST = 2,
	FOREARM_LEFT = 3,
	FOREARM_RIGHT = 4,
	HAND_LEFT = 5,
	HAND_RIGHT = 6,
	HEAD = 7,
	SHIN_LEFT = 8,
	SHIN_RIGHT = 9,
	THIGH_LEFT = 10,
	THIGH_RIGHT = 11,
};



inline BodyPart bodyPartFromName(const std::string& name)
{
	if (name == "arm_left")      return BodyPart::ARM_LEFT;
	if (name == "arm_right")     return BodyPart::ARM_RIGHT;
	if (name == "chest")         return BodyPart::CHEST;
	if (name == "forearm_left")  return BodyPart::FOREARM_LEFT;
	if (name == "forearm_right") return BodyPart::FOREARM_RIGHT;
	if (name == "hand_left")     return BodyPart::HAND_LEFT;
	if (name == "hand_right")    return BodyPart::HAND_RIGHT;
	if (name == "head")          return BodyPart::HEAD;
	if (name == "shin_left")     return BodyPart::SHIN_LEFT;
	if (name == "shin_right")    return BodyPart::SHIN_RIGHT;
	if (name == "thigh_left")    return BodyPart::THIGH_LEFT;
	if (name == "thigh_right")   return BodyPart::THIGH_RIGHT;
	return BodyPart::HEAD;
}



inline std::string bodyPartToName(BodyPart bodyPart)
{
	if (bodyPart == BodyPart::ARM_LEFT)      return "arm_left";
	if (bodyPart == BodyPart::ARM_RIGHT)     return "arm_right";
	if (bodyPart == BodyPart::CHEST)         return "chest";
	if (bodyPart == BodyPart::FOREARM_LEFT)  return "forearm_left";
	if (bodyPart == BodyPart::FOREARM_RIGHT) return "forearm_right";
	if (bodyPart == BodyPart::HAND_LEFT)     return "hand_left";
	if (bodyPart == BodyPart::HAND_RIGHT)    return "hand_right";
	if (bodyPart == BodyPart::HEAD)          return "head";
	if (bodyPart == BodyPart::SHIN_LEFT)     return "shin_left";
	if (bodyPart == BodyPart::SHIN_RIGHT)    return "shin_right";
	if (bodyPart == BodyPart::THIGH_LEFT)    return "thigh_left";
	if (bodyPart == BodyPart::THIGH_RIGHT)   return "thigh_right";
	return "head";
}



class Character
{
public:


	Character(const std::string& name);

	void uploadGpu(); 

	void draw(BodyPart body_part, glm::vec2 position, float rotation) const;

	const std::string name;

private:

	bool onGpu = false;

	std::unordered_map<BodyPart, ofImage> body_parts;

};
