#include "ofApp.h"




void split_prompt(std::vector<std::string>& result, const std::vector<std::string>& words, const ofTrueTypeFont& font, float width)
{
	std::string previous_line; // the line at the last iteration.
	
	for (auto it = words.begin(); it != words.end(); ++it)
	{
		vector<string> line{ words.begin(), it + 1 };
		auto line_string = ofJoinString(line, " ");
		auto line_width = font.stringWidth(line_string);

		if (line_width < width)
		{
			previous_line = line_string;
			continue; // continue the loop
		}
		else
		{
			if (line.size() == 1)
			{
				result.push_back(line_string);
				//std::cout << line_string << std::endl;
				split_prompt(result, vector<string>(it + 1, words.end()), font, width);
				break;
			}
			else
			{
				result.push_back(previous_line);
				//std::cout << previous_line << std::endl;
				split_prompt(result, vector<string>(it, words.end()), font, width);
				break;
			}
		}
	}
}


std::string split_prompt(std::string input, const ofTrueTypeFont& font, float width)
{
	auto words = ofSplitString(input, " ", true, true);
	std::vector<std::string> result;
	split_prompt(result, words, font, 200);
	return ofJoinString(result, "\n");
}


void ofApp::setup()
{
	if (FAILED(GetDefaultKinectSensor(&kinect))) throw std::runtime_error("No kinect device found, install drivers and plug in the kinect.");

	BOOLEAN open_already;
	kinect->get_IsOpen(&open_already);

	if (!open_already) if (FAILED(kinect->Open())) throw std::runtime_error("Kinect failed to open, restart and try again?");

	IBodyFrameSource* skeleton_source = nullptr;
	kinect->get_BodyFrameSource(&skeleton_source);

	if (skeleton_source)
	{
		skeleton_source->OpenReader(&skeleton_reader);
		skeleton_source->Release();
	}

	ofTrueTypeFontSettings fontSettings{ settings::working_directory + "/fonts/" + settings::font, settings::font_size };
	font.load(fontSettings);

	std::cout << split_prompt("A few simple principles", font, 200) << std::endl;

	// Need to load a test image, to connect to the network. If this is not done it will for some reason not
	// be possible to open the websocket connection.
	ofImage test{ "https://en.wikipedia.org/wiki/Battle_of_France#/media/File:Battle_of_France_collage.jpg" };

	background = std::make_unique<ofImage>(settings::working_directory + "/backgrounds/default.jpg");

	ofSetFrameRate(60);

	web_socket = std::unique_ptr<WebSocket>(WebSocket::from_url(settings::server_ip));

	image_loader.initialize();
}


void ofApp::exit()
{
	if (web_socket)
	{
		std::cout << "shutting down" << std::endl;
		web_socket->send("OK BYE!");
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
		web_socket->close();
	}
	kinect->Close();
	skeleton_reader->Release();
	kinect->Release();
}


void ofApp::receive(const std::string& message)
{
	std::cout << message << "\n";
	auto parts = ofSplitString(message, " ");

	if (!parts.empty())
	{
		if (parts[0] == "backgroundUrl")
		{
			image_loader.load_background(parts[1]);
		}
		else if (parts[0] == "promptText")
		{
			parts.erase(parts.begin());
			promptText = ofJoinString(parts, " ");
			promptWidth = font.stringWidth(promptText);
			promptHeight = font.stringHeight(promptText);
			std::vector<std::string> result;
			make_prompt(result, parts);
		}
		else if (parts[0] == "startRecording")
		{
			isRecording = true;
		}
		else if (parts[0] == "stopRecording")
		{
			isRecording = false;
		}
		else if (parts[0] == "character")
		{
			image_loader.load_character(std::stoi(parts[1]) - 1, parts[2]);
		}
		else if (parts[0] == "playerAmount")
		{

		}
	}
}

void ofApp::make_prompt(std::vector<std::string>& result, const std::vector<std::string>& words)
{
	std::string previous_line; // the line at the last iteration.

	for (auto it = words.begin(); it != words.end(); ++it)
	{
		vector<string> line{ words.begin(), it };
		auto line_string = ofJoinString(line, " ");
		auto line_width = font.stringWidth(line_string);
		
		if (line_width < ofGetWidth())
		{
			previous_line = line_string;
			continue; // continue the loop
		}
		else
		{
			if (line.size() <= 1)
			{

			}
			result.push_back(previous_line);
			make_prompt(result, vector<string>(it, words.end()));
		}
	}
}



void ofApp::update()
{
	IBodyFrame* frame = nullptr;
	auto status = skeleton_reader->AcquireLatestFrame(&frame);

	if (SUCCEEDED(status)) {

		skeletons.clear();

		std::array<IBody*, BODY_COUNT> new_body_data = { 0, 0, 0, 0, 0, 0 };

		status = frame->GetAndRefreshBodyData(BODY_COUNT, new_body_data.data());

		if (SUCCEEDED(status)) {

			for (auto body : new_body_data)
			{
				BOOLEAN is_tracked;
				body->get_IsTracked(&is_tracked);

				if (is_tracked)
				{
					Skeleton* newPlayer = new Skeleton(body);
					skeletons[newPlayer->getId()] = std::unique_ptr<Skeleton>(newPlayer);
				}
			}

			for (auto body : new_body_data)
			{
				body->Release();
			}
			frame->Release();
		}


		for (auto& player_id : players)
		{
			if (skeletons.find(player_id) != skeletons.end())
			{
				// do nothing, player should remain same character
				player_id = player_id;
			}
			else
			{
				// set all no longer present id's to 0
				player_id = 0;
			}
		}


		// if id not in playersId's find first possible spot if any and put it there.
		for (auto& skeleton : skeletons)
		{
			auto id = skeleton.first;
			bool succes = false;

			if (std::find(players.begin(), players.end(), id) == players.end())
			{
				for (auto& playerId : players)
				{
					if (playerId == 0)
					{
						playerId = id;
						succes = true;
						break;
					}
				}
			}

			if (succes)
			{
				break;
			}
		}
	}

	if (web_socket)
	{
		web_socket->poll(0);
		web_socket->dispatch([=](const std::string& msg) { this->receive(msg); });
	}

	if (image_loader.has_background_ready())
	{
		background.reset(image_loader.get_background());
		background->setUseTexture(true);
		background->update();
	}

	if (image_loader.has_character_ready())
	{
		auto character = image_loader.get_character();
		characters[character.first].reset(character.second);
		characters[character.first]->uploadGpu();
	}

}



void ofApp::draw()
{
	ofSetColor(255);

	background->draw(0, 0, ofGetWidth(), ofGetHeight());


	for (int i = 0; i < players.size(); ++i)
	{
		auto id = players[i];
	
		if (id != 0 && characters[i])
		{
			if (skeletons.find(id) != skeletons.end())
			{
				skeletons[id]->draw(*characters[i]);
			}
		}
	}

	if (isDebug)
	{
		for (auto& p: skeletons)
		{
			if (p.second)
			{
				p.second->drawSkeleton();
			}
		}
	}


	auto x = (ofGetWidth() - promptWidth) / 2;
	auto y = ofGetHeight() - promptHeight - 20;

	ofSetColor(0, 70);
	ofDrawRectangle(x - 30, y - promptHeight - 30, promptWidth + 60, promptHeight + 60);
	ofSetColor(255);

	font.drawString(promptText, x, y);

	if (isRecording)
	{
		ofSetColor(255, 0, 0);
		ofDrawCircle(ofGetWidth() - 50, 50, 30);
		ofSetColor(255, 255, 255);
	}

	if (isDebug)
	{
		ofSetColor(255, 0, 255);
		ofDrawBitmapString(ofToString("FPS: ") + ofToString(ofGetFrameRate()), 30, 50);
		ofDrawBitmapString("Y offset: " + ofToString(settings::height_offset), 180, 50);

		ofDrawBitmapString("Skeleton ID's:", 30, 80);
		ofDrawBitmapString("Player, ID:", 180, 80);
		ofDrawBitmapString("Player, Character:", 360, 80);

		int s = 0;

		for (auto& skeleton : skeletons)
		{
			ofDrawBitmapString(skeleton.first, 30, 100 + 15 * s);
			++s;
		}

		for (int i = 0; i < 6; ++i)
		{
			auto p = "P" + ofToString(i + 1) + ": ";
			ofDrawBitmapString(p + ofToString(players[i]), 180, 100 + 15 * i);

			if (characters[i])
			{
				ofDrawBitmapString(p + characters[i]->name, 360, 100 + 15 * i);
			}
			else
			{
				ofDrawBitmapString(p + ofToString(characters[i]), 360, 100 + 15 * i);
			}
		}
	}
}


void ofApp::keyReleased(int key)
{
	if (key == 'f')
	{
		ofToggleFullscreen();
	}
	else if (key == 'd')
	{
		isDebug = !isDebug;
	}
	else if (key >= '0' && key <= '9')
	{
		image_loader.load_background("https://picsum.photos/id/1" + std::to_string(key - '0')  + "/1920/1080");
	}
	else if (key == OF_KEY_UP)
	{
		settings::height_offset += 5;
	}
	else if (key == OF_KEY_DOWN)
	{
		settings::height_offset -= 5;
	}
}
