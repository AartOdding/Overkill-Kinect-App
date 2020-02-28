#include "ImageLoader.h"
#include "ofApp.h"


ImageLoader::~ImageLoader()
{
	if (worker_thread)
	{
		stop_thread = true;
		worker_thread->join();
	}
}

void ImageLoader::initialize()
{
	worker_thread = std::make_unique<std::thread>(&ImageLoader::threaded_function, this);
}


void ImageLoader::threaded_function()
{
	using namespace std::chrono_literals;

	while (!stop_thread)
	{
		bool work = false;
		std::string name;
		int number;

/////////////////////////////////////////////////////////////////
		                                                       //
		input_mutex.lock();                                    //
		                                                       //
		if (!background_input_queue.empty())                   //
		{                                                      //
			work = true;                                       //    locked
			name = background_input_queue.front();             //
			background_input_queue.pop_front();                //
		}                                                      //
		                                                       //
		input_mutex.unlock();                                  //
		                                                       //
/////////////////////////////////////////////////////////////////

		if (work)
		{
			auto image = new ofImage();
			image->setUseTexture(false);
			image->load(name);
			
//////////////////////////////////////////////////////////////////////
			                                                        //
			output_mutex.lock();                                    //
																	//
			background_output_queue.emplace_back(image);			//   locked
			                                                        //
			output_mutex.unlock();                                  //
			                                                        //
////////////////////////////////////////////////////////////////////// 
		}

		work = false;

/////////////////////////////////////////////////////////////////
															   //
		input_mutex.lock();                                    //
															   //
		if (!character_input_queue.empty())                    //
		{                                                      //
			work = true;                                       //    locked
			name = character_input_queue.front().second;       //
			number = character_input_queue.front().first;      //
			character_input_queue.pop_front();                 //
		}                                                      //
															   //
		input_mutex.unlock();                                  //
															   //
/////////////////////////////////////////////////////////////////

		if (work)
		{

			Character* character = new Character(name);

//////////////////////////////////////////////////////////////////////
																	//
			output_mutex.lock();                                    //
																	//
			character_output_queue.emplace_back(number, character); //   locked
																	//
			output_mutex.unlock();                                  //
																	//
//////////////////////////////////////////////////////////////////////

		}

		std::this_thread::sleep_for(20ms);
	}
}


void ImageLoader::load_background(const std::string& url)
{
    std::lock_guard<std::mutex> lock{ input_mutex };
    background_input_queue.push_back(url);
}

void ImageLoader::load_character(int playerNumber, const std::string& name)
{
    std::lock_guard<std::mutex> lock{ input_mutex };
    character_input_queue.emplace_back(playerNumber, name);
}


bool ImageLoader::has_background_ready()
{
    std::lock_guard<std::mutex> lock{ output_mutex };
    return !background_output_queue.empty();
}

bool ImageLoader::has_character_ready()
{
    std::lock_guard<std::mutex> lock{ output_mutex };
    return !character_output_queue.empty();
}


ofImage* ImageLoader::get_background()
{
    std::lock_guard<std::mutex> lock{ output_mutex };

    if (!background_output_queue.empty())
    {
		ofImage* background = background_output_queue.front();
		background_output_queue.pop_front();
		return background;
    }
    else
    {
		return nullptr;
    }
}


std::pair<int, Character*> ImageLoader::get_character()
{
    std::lock_guard<std::mutex> lock{ output_mutex };
    
	if (!character_output_queue.empty())
    {
		int playerIndex = character_output_queue.front().first;
		Character* character = character_output_queue.front().second;
		character_output_queue.pop_front();
		return { playerIndex, character };
    }
    else
    {
		return { -1, nullptr };
    }
}

