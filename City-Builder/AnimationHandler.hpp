#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

class Animation
{
public:
	unsigned int startFrame;
	unsigned int endFrame;

	float duration;

	Animation(unsigned int startFrame, unsigned int endFrame, float duration)
	{
		this->startFrame = startFrame;
		this->endFrame = endFrame;
		this->duration = duration;
	}

	unsigned int getLength()
	{
		return endFrame - startFrame + 1;
	}

};

class AnimationHandler
{
private:
	// array of animations
	std::vector<Animation> animations;

	// time since animation loop started
	float t;

	int currentAnim;

public:

	// add a new animation
	void addAnim(Animation& anim);

	/* 
	*  update current frame of animation. dt is time since last called 
	*  (aka time for one frame to execute)
	*/
	void update(const float dt);

	// change animation, resetting it to t in the process
	void changeAnim(unsigned int animNum);

	// current section of texture that should be displayed
	sf::IntRect bounds;

	// pixel dimensions of each frame
	sf::IntRect frameSize;

	// constructor
	AnimationHandler()
	{
		this->t = 0.0f;
		this->currentAnim = -1;
	}
	AnimationHandler(const sf::IntRect &frameSize)
	{
		this->frameSize = frameSize;

		this->t = 0.0f;
		this->currentAnim = -1;
	}

};