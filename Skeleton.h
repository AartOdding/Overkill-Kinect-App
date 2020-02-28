#pragma once

#include <Kinect.h>
#include <glm/glm.hpp>
#include <ofMain.h>

#include <array>
#include <cstdint>

class Character;


class Skeleton
{

public:

	Skeleton(IBody* body);

	void draw(const Character& character);

	void drawSkeleton();

	uint64_t getId() const;

private:

	glm::vec2 pos(JointType joint) const;
	glm::vec2 avg(JointType joint_a, JointType joint_b) const;

	void draw_limb(const ofImage&) const;

	uint64_t id;

	std::array<Joint, JointType_Count> joints;

};

