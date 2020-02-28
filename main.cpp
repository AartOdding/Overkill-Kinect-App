#include "ofMain.h"
#include "ofApp.h"
#include "cxxopts.hpp"



namespace
{
	std::string server_ip_value;

	std::string working_directory_value;
	std::string bodyparts_directory_value;
	std::string characters_directory_value;

	std::string font_value;
	int font_size_value;

	int window_x_value = 0;
	int window_y_value = 0;
	int window_width_value = 640;
	int window_height_value = 480;

	double image_scale_value = 0.5;
	double body_scale_value = 500;
}



namespace settings
{
	const std::string& server_ip = server_ip_value;

	const std::string& working_directory = working_directory_value;
	const std::string& bodyparts_directory = bodyparts_directory_value;
	const std::string& characters_directory = characters_directory_value;

	const std::string& font = font_value;
	const int& font_size = font_size_value;

	const int& window_x = window_x_value;
	const int& window_y = window_y_value;
	const int& window_width = window_width_value;
	const int& window_height = window_height_value;

	const double& body_scale = body_scale_value;
	const double& image_scale = image_scale_value;

	int height_offset = 100;
}



int main(int argc, char** argv)
{
	ofFileDialogResult directory = ofSystemLoadDialog("Select Working Directory", true);
	working_directory_value = directory.filePath;
	
	bodyparts_directory_value = settings::working_directory + "/body_parts";
	characters_directory_value = settings::working_directory + "/Characters";

	ofBuffer buffer = ofBufferFromFile(settings::working_directory + "/config/kinect_app.txt");

	for (auto line : buffer.getLines())
	{
		auto words = ofSplitString(line, " ", true, true);

		if (words.size() >= 2)
		{
			if (words[0] == "server_ip")
			{
				server_ip_value = words[1];
				std::cout << "server ip: " << settings::server_ip << std::endl;
			}
			else if (words[0] == "font")
			{
				font_value = words[1];
				std::cout << "font: " << settings::font << std::endl;
			}
			else if (words[0] == "font_size")
			{
				font_size_value = std::stoi(words[1]);
				std::cout << "font size: " << settings::font_size << std::endl;
			}
			else if (words[0] == "window_x")
			{
				window_x_value = std::stoi(words[1]);
				std::cout << "window x: " << settings::window_x << std::endl;
			}
			else if (words[0] == "window_y")
			{
				window_y_value = std::stoi(words[1]);
				std::cout << "window y: " << settings::window_y << std::endl;
			}
			else if (words[0] == "window_width")
			{
				window_width_value = std::stoi(words[1]);
				std::cout << "window width: " << settings::window_width << std::endl;
			}
			else if (words[0] == "window_height")
			{
				window_height_value = std::stoi(words[1]);
				std::cout << "window height: " << settings::window_height << std::endl;
			}
			else if (words[0] == "body_scale")
			{
				body_scale_value = std::stod(words[1]);
				std::cout << "body scale: " << settings::body_scale << std::endl;
			}
			else if (words[0] == "image_scale")
			{
				image_scale_value = std::stod(words[1]);
				std::cout << "image scale: " << settings::image_scale << std::endl;
			}
		}
	}

	ofGLWindowSettings windowSettings;
	windowSettings.setGLVersion(3, 2);
	//windowSettings.windowMode = OF_GAME_MODE;
	windowSettings.setPosition({ settings::window_x, settings::window_y });
	windowSettings.setSize(settings::window_width, settings::window_height);

	auto window = ofCreateWindow(windowSettings);

	//ofSetupOpenGL(1280,720,OF_WINDOW);
	ofRunApp(new ofApp());
}
