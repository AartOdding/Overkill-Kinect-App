#include "Character.h"
#include "ofApp.h"

#include <ofMain.h>


Character::Character(const std::string& n)
	: name(n)
{
	ofXml xml;
	xml.load(settings::characters_directory + "/" + name + ".xml");

	auto parts = xml.getChild("character").getChildren();

	for (auto& x : parts)
	{
		std::cout << x.getName() << "\t" << x.getValue() << "\n";

		BodyPart bp = bodyPartFromName(x.getName());
		auto dir = settings::working_directory + "/" + x.getValue();

		body_parts[bp] = ofImage();
		auto img = &body_parts[bp];
		img->setUseTexture(false);

		if (!img->load(dir))
		{
			body_parts.erase(bp);
		}

		std::cout << dir << std::endl;

	}
}



void Character::uploadGpu()
{
	for (auto& img : body_parts)
	{
		img.second.setUseTexture(true);
		img.second.update();
		img.second.setAnchorPoint(512, 512);
	}
	onGpu = true;
}



void Character::draw(BodyPart body_part, glm::vec2 position, float rotation) const
{
	if (onGpu && body_parts.find(body_part) != body_parts.end())
	{
		ofPushMatrix();

		ofTranslate(position);
		ofRotate(rotation * 57.2957795); // convert to radians
		body_parts.at(body_part).draw(0, 0);

		ofPopMatrix();
	}
}
