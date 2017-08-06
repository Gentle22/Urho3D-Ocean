//
// Copyright (c) 2008-2016 the Urho3D project.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//

#include "../Precompiled.h"
#include "Urho3D/IO/Log.h"
#include "Urho3D/Core/Profiler.h"

#include "WaveSystem.h"


namespace Urho3D
{

WaveSystem::WaveSystem(Context* context) :
	Object(context)
{
}

WaveSystem::~WaveSystem()
{
}

void WaveSystem::Update(const float time)
{
	URHO3D_PROFILE(WaveSystem);

	// Handle waves that are currently fading in
	auto itWaveFadeIn = fadeInWaves_.begin();
	while (itWaveFadeIn != fadeInWaves_.end())
	{
		itWaveFadeIn->wave_->a_ += itWaveFadeIn->fadeAmplitude_ * time;
		itWaveFadeIn->wave_->q_ += itWaveFadeIn->fadeSteepness_ * time;
		if (itWaveFadeIn->wave_->a_ >= itWaveFadeIn->targetAmplitude_)
			itWaveFadeIn = fadeInWaves_.erase(itWaveFadeIn);
		else
			++itWaveFadeIn;
	}

	// Handle fade outs
	auto itWaveFadeOut = fadeOutWaves_.begin();
	while (itWaveFadeOut != fadeOutWaves_.end())
	{
		itWaveFadeOut->wave_->a_ -= itWaveFadeOut->fadeAmplitude_ * time;
		itWaveFadeOut->wave_->q_ -= itWaveFadeOut->fadeSteepness_ * time;
		if (itWaveFadeOut->wave_->a_ <= itWaveFadeOut->targetAmplitude_)
		{
			// Erase from active waves
			EraseActiveWave(itWaveFadeOut->wave_);
			itWaveFadeOut = fadeOutWaves_.erase(itWaveFadeOut);
		}
		else
		{
			++itWaveFadeOut;
		}
	}

	// Handle active waves
	for (auto& activeWave : activeWaves_)
	{
		activeWave.lifeTime_ -= time;
		if (activeWave.lifeTime_ <= 0.f && !activeWave.isFadingOut)
		{
			activeWave.isFadingOut = true;
			FadeOutWave(activeWave.wave_);
		}
	}

	for (size_t i = activeWaves_.size(); i < numWaves_; ++i)
	{
		FadeInWave();
	}
}

void WaveSystem::Reset()
{
	fadeInWaves_.clear();
	fadeOutWaves_.clear();
	activeWaves_.clear();
}

const PODVector<WaveSystem::Wave*> WaveSystem::GetWaves() const
{
	PODVector<WaveSystem::Wave*> waves;

	for (auto wave : activeWaves_)
	{
		waves.Push(wave.wave_.get());
	}

	return waves;
}

std::shared_ptr<WaveSystem::Wave> WaveSystem::CreateWave() const
{
	const float amplitudeLengthRatio = amplitude_ / length_;

	const float length = Random(0.5f * length_, 2.0f * length_);
	const float amplitude = length * amplitudeLengthRatio;
	const float angle = Random(-0.5f * angle_, 0.5f * angle_);
	Vector2 direction;
	direction.x_ = direction_.x_ * Cos(angle) - direction_.y_ * Sin(angle);
	direction.y_ = direction_.x_ * Sin(angle) + direction_.y_ * Cos(angle);

	float speed = speed_;
	if (speedVariationEnabled_)
		float speed = Random(0.7f * speed_, 3.0f * speed_);

	return std::make_shared<WaveSystem::Wave>(steepness_, speed, length, amplitude, direction);
}

void WaveSystem::FadeInWave()
{
	URHO3D_LOGINFO("Fade In!");

	auto newWave = CreateWave();
	activeWaves_.push_back(Active{ Random(lifetime_ * 0.5f, lifetime_ * 2.0f), false, newWave });

	if (fadingEnabled_)
	{
		fadeInWaves_.push_back(Fade{ newWave->a_, newWave->a_ / fadeTime_, newWave->q_, newWave->q_ / fadeTime_, newWave });
		newWave->a_ = 0.f;
		newWave->q_ = 0.f;
	}
}

void WaveSystem::FadeOutWave(std::shared_ptr<WaveSystem::Wave> wave)
{
	URHO3D_LOGINFO("Fade Out!");

	fadeOutWaves_.push_back(Fade{ 0.0f, wave->a_ / fadeTime_, 0.0f, wave->q_ / fadeTime_, wave });

	if (!fadingEnabled_)
	{
		wave->a_ = 0.f;
	}
}

void WaveSystem::EraseActiveWave(std::shared_ptr<Wave> wave)
{
	URHO3D_LOGINFO("Erase Wave!");

	for (auto itWave = activeWaves_.begin(); itWave != activeWaves_.end(); ++itWave)
	{
		if (itWave->wave_ == wave)
		{
			activeWaves_.erase(itWave);
			return;
		}
	}
}

}
