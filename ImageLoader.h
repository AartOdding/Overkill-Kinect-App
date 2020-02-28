#pragma once

#include <thread>
#include <mutex>
#include <list>
#include <memory>
#include <atomic>
#include <utility>

#include "ofImage.h"
#include "Character.h"


class ImageLoader
{
public:

	ImageLoader() = default;

	ImageLoader(const ImageLoader&) = delete;

	ImageLoader(ImageLoader&&) = delete;


	~ImageLoader();

	void initialize();



	void load_background(const std::string& url);

	void load_character(int playerNumber, const std::string& characterName);


	bool has_background_ready();

	bool has_character_ready();


	ofImage* get_background();

	std::pair<int, Character*> get_character();

private:

	std::atomic<bool> stop_thread = false;

	std::unique_ptr<std::thread> worker_thread;

	std::mutex input_mutex;
	std::mutex output_mutex;

	std::list<ofImage*>                   background_output_queue;
	std::list<std::pair<int, Character*>>  character_output_queue;

	std::list<std::string>                 background_input_queue;
	std::list<std::pair<int, std::string>>  character_input_queue;


	void threaded_function();

};




