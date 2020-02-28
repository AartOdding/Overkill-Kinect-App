#pragma once

#include <unordered_map>
#include <vector>
#include <string>
#include <memory>

#include "main.h"
#include "ofMain.h"

#include "Kinect.h"
#include "easywsclient.hpp"

#include "Skeleton.h"
#include "Character.h"
#include "ImageLoader.h"


using easywsclient::WebSocket;


class ofApp : public ofBaseApp
{

public:

	void setup();
	void exit();

	void update();
	void draw();

	void make_prompt(std::vector<std::string>& result, const std::vector<std::string>& words);

	void keyReleased(int key);

	void receive(const std::string& message);

private:

	ImageLoader image_loader;
	IKinectSensor* kinect = nullptr;
	IBodyFrameReader* skeleton_reader = nullptr;

	std::unique_ptr<ofImage> background;
	std::unique_ptr<WebSocket> web_socket;

	ofTrueTypeFont font;

	std::string promptText = "Overkill Festival 2019!";
	int promptWidth = 0;
	int promptHeight = 0;

	bool isRecording = false;
	bool isDebug = false;

	std::array<uint64_t, 6> players;
	std::unordered_map<uint64_t, std::unique_ptr<Skeleton>> skeletons;
	std::array<std::unique_ptr<Character>, 6> characters;
	
};
