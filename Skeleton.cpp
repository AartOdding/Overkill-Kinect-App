#include "Skeleton.h"
#include "Character.h"
#include "main.h"


float dir(const glm::vec2& from, const glm::vec2& to)
{
	auto delta = to - from;
	return atan2(delta.y, delta.x);
}


Skeleton::Skeleton(IBody* body)
{
	body->get_TrackingId(&id);
	body->GetJoints(joints.size(), joints.data());
}


void Skeleton::draw(const Character& character)
{
	ofPushMatrix();

	ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2 + settings::height_offset);
	ofScale(settings::image_scale, settings::image_scale);

	character.draw(BodyPart::SHIN_RIGHT, avg(JointType_KneeLeft, JointType_AnkleLeft), HALF_PI + dir(avg(JointType_KneeLeft, JointType_AnkleLeft), pos(JointType_KneeLeft)));
	character.draw(BodyPart::SHIN_LEFT, avg(JointType_KneeRight, JointType_AnkleRight), HALF_PI + dir(avg(JointType_KneeRight, JointType_AnkleRight), pos(JointType_KneeRight)));

	character.draw(BodyPart::THIGH_RIGHT, avg(JointType_KneeLeft, JointType_HipLeft), HALF_PI + dir(avg(JointType_KneeLeft, JointType_HipLeft), pos(JointType_HipLeft)));
	character.draw(BodyPart::THIGH_LEFT, avg(JointType_KneeRight, JointType_HipRight), HALF_PI + dir(avg(JointType_KneeRight, JointType_HipRight), pos(JointType_HipRight)));

	character.draw(BodyPart::CHEST, pos(JointType_SpineMid), HALF_PI + dir(pos(JointType_SpineMid), pos(JointType_Neck)));

	character.draw(BodyPart::ARM_RIGHT, avg(JointType_ShoulderLeft, JointType_ElbowLeft), dir(avg(JointType_ShoulderLeft, JointType_ElbowLeft), pos(JointType_ShoulderLeft)));
	character.draw(BodyPart::ARM_LEFT, avg(JointType_ShoulderRight, JointType_ElbowRight), PI + dir(avg(JointType_ShoulderRight, JointType_ElbowRight), pos(JointType_ShoulderRight)));

	character.draw(BodyPart::FOREARM_RIGHT, avg(JointType_WristLeft, JointType_ElbowLeft), dir(avg(JointType_WristLeft, JointType_ElbowLeft), pos(JointType_ElbowLeft)));
	character.draw(BodyPart::FOREARM_LEFT, avg(JointType_WristRight, JointType_ElbowRight), PI + dir(avg(JointType_WristRight, JointType_ElbowRight), pos(JointType_ElbowRight)));

	character.draw(BodyPart::HAND_RIGHT, pos(JointType_HandLeft), 0);
	character.draw(BodyPart::HAND_LEFT, pos(JointType_HandRight), 0);

	character.draw(BodyPart::HEAD, pos(JointType_Head), 0);

	ofPopMatrix();
}

uint64_t Skeleton::getId() const
{
	return id;
}


void Skeleton::drawSkeleton()
{
	ofPushMatrix();

	ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2 + settings::height_offset);
	ofScale(settings::image_scale, settings::image_scale);

	ofSetColor(255, 0, 0);

	ofDrawCircle(pos(JointType_Head), 12);
	ofDrawCircle(pos(JointType_Neck), 12);
	ofDrawCircle(pos(JointType_SpineShoulder), 12);
	ofDrawCircle(pos(JointType_SpineMid), 12);
	ofDrawCircle(pos(JointType_SpineBase), 12);

	ofDrawCircle(pos(JointType_HipLeft), 12);
	ofDrawCircle(pos(JointType_KneeLeft), 12);
	ofDrawCircle(pos(JointType_AnkleLeft), 12);
	ofDrawCircle(pos(JointType_FootLeft), 8);

	ofDrawCircle(pos(JointType_HipRight), 12);
	ofDrawCircle(pos(JointType_KneeRight), 12);
	ofDrawCircle(pos(JointType_AnkleRight), 12);
	ofDrawCircle(pos(JointType_FootRight), 8);

	ofDrawCircle(pos(JointType_ShoulderLeft), 12);
	ofDrawCircle(pos(JointType_ElbowLeft), 12);
	ofDrawCircle(pos(JointType_WristLeft), 12);
	ofDrawCircle(pos(JointType_HandLeft), 8);
	ofDrawCircle(pos(JointType_HandTipLeft), 8);

	ofDrawCircle(pos(JointType_ShoulderRight), 12);
	ofDrawCircle(pos(JointType_ElbowRight), 12);
	ofDrawCircle(pos(JointType_WristRight), 12);
	ofDrawCircle(pos(JointType_HandRight), 8);
	ofDrawCircle(pos(JointType_HandTipRight), 8);

	ofPopMatrix();
}


bool flipXAxis = false;

glm::vec2 Skeleton::pos(JointType joint) const
{
	if (flipXAxis)
	{
		return glm::vec2(-joints[joint].Position.X * settings::body_scale, -joints[joint].Position.Y * settings::body_scale);
	}
	else
	{
		return glm::vec2(joints[joint].Position.X * settings::body_scale, -joints[joint].Position.Y * settings::body_scale);
	}
}

glm::vec2 Skeleton::avg(JointType joint_a, JointType joint_b) const
{
	return 0.5 * (pos(joint_a) + pos(joint_b));
}

/*
ofVec2f ofApp::pt(JointType joint) const
{
	return ofVec2f(body[joint].Position.X * body_scale, -body[joint].Position.Y * body_scale);
}*/

/*


*/

/*
void ofApp::draw_limb(BodyPartType limb, const ofVec2f& pt, float dir)
{
	ofPushMatrix();

	ofTranslate(pt);
	ofRotateRad(dir);
	//limbs[limb].draw(0, 0);

	ofPopMatrix();
}*/


//limbs[ARM_LEFT].draw(100, 100);

	//limbs[FOREARM_LEFT].draw(avg(JointType_WristLeft, JointType_ElbowLeft));
	//limbs[FOREARM_RIGHT].draw(avg(JointType_WristRight, JointType_ElbowRight));
	//limbs[SHIN_LEFT].draw(avg(JointType_KneeLeft, JointType_AnkleLeft));
	//limbs[SHIN_RIGHT].draw(avg(JointType_KneeRight, JointType_AnkleRight));

/*
if (has_body)
{
	ofPushMatrix();

	ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2);
	ofScale(img_scale, img_scale);

	draw_limb(CHEST, pt(JointType_SpineMid), HALF_PI + dir(pt(JointType_SpineMid), pt(JointType_Neck)));

	draw_limb(FOREARM_LEFT, avg(JointType_WristLeft, JointType_ElbowLeft), dir(avg(JointType_WristLeft, JointType_ElbowLeft), pt(JointType_ElbowLeft)));
	draw_limb(FOREARM_RIGHT, avg(JointType_WristRight, JointType_ElbowRight), dir(avg(JointType_WristRight, JointType_ElbowRight), pt(JointType_ElbowRight)));
	draw_limb(SHIN_LEFT, avg(JointType_KneeLeft, JointType_AnkleLeft), HALF_PI + dir(avg(JointType_KneeLeft, JointType_AnkleLeft), pt(JointType_KneeLeft)));
	draw_limb(SHIN_RIGHT, avg(JointType_KneeRight, JointType_AnkleRight), HALF_PI + dir(avg(JointType_KneeRight, JointType_AnkleRight), pt(JointType_KneeRight)));

	draw_limb(ARM_LEFT, avg(JointType_ShoulderLeft, JointType_ElbowLeft), dir(avg(JointType_ShoulderLeft, JointType_ElbowLeft), pt(JointType_ShoulderLeft)));
	draw_limb(ARM_RIGHT, avg(JointType_ShoulderRight, JointType_ElbowRight), dir(avg(JointType_ShoulderRight, JointType_ElbowRight), pt(JointType_ShoulderRight)));
	draw_limb(THIGH_LEFT, avg(JointType_KneeLeft, JointType_HipLeft), HALF_PI + dir(avg(JointType_KneeLeft, JointType_HipLeft), pt(JointType_HipLeft)));
	draw_limb(THIGH_RIGHT, avg(JointType_KneeRight, JointType_HipRight), HALF_PI + dir(avg(JointType_KneeRight, JointType_HipRight), pt(JointType_HipRight)));

	limbs[HEAD].draw(pt(JointType_Head));
	limbs[HAND_LEFT].draw(pt(JointType_HandLeft));
	limbs[HAND_RIGHT].draw(pt(JointType_HandRight));

	ofPopMatrix();
}*/