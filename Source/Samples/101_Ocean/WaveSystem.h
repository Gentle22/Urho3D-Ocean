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

#pragma once

#include "Urho3D/Core/Object.h"
#include "Urho3D/Math/Vector2.h"
#include "Urho3D/Container/Ptr.h"

#include <tuple>
#include <memory>
#include <vector>

namespace Urho3D
{


class WaveSystem : public Object
{
	URHO3D_OBJECT(WaveSystem, Object);

public:

	struct Wave
	{
		Wave(const float q, const float s, const float l, const float a, const Vector2 d) :
			q_{ q }, s_{ s }, l_{ l }, a_{ a }, d_{ d }
		{}

		float q_;
		float s_;
		float l_;
		float a_;
		Vector2 d_;
	};

	WaveSystem(Context* context);
    ~WaveSystem();

	void SetSteepness(const float value) { steepness_ = value; }
	float GetSteepness() const { return steepness_; }

	void SetSpeed(const float value) { speed_ = value; }
	float GetSpeed() const { return speed_; }

	void SetLength(const float value) { length_ = value; }
	float GetLength() const { return length_; }

	void SetAmplitude(const float value) { amplitude_ = value; }
	float GetAmplitude() const { return amplitude_; }

	void SetDirection(const Vector2 value) { direction_ = value; }
	const Vector2 GetDirection() const { return direction_; }

	void SetAngle(const float value) { angle_ = value; }
	float GetAngle() const { return angle_; }

	void EnableFading(const bool value) { fadingEnabled_ = value; }
	bool FadingEnabled() const { return fadingEnabled_; }

	void EnableSpeedVariation(const bool value) { speedVariationEnabled_ = value; }
	bool SpeedVariationEnabled() const { return speedVariationEnabled_; }

	void Update(const float time);

	void Reset();

	const PODVector<Wave*> GetWaves() const;

private:

	struct Fade
	{
		float targetAmplitude_;
		float fadeAmplitude_;
		float targetSteepness_;
		float fadeSteepness_;
		std::shared_ptr<WaveSystem::Wave> wave_;
	};

	struct Active
	{
		float lifeTime_;
		bool isFadingOut;
		std::shared_ptr<WaveSystem::Wave> wave_;
	};

	std::shared_ptr<Wave> CreateWave() const;
	void FadeInWave();
	void FadeOutWave(std::shared_ptr<Wave> wave);
	void EraseActiveWave(std::shared_ptr<Wave> wave);
	
	size_t numWaves_ = 6;

	float lifetime_ = 30.0f;
	const float fadeTime_ = 5.0f;

	float steepness_ = 0.9f;
	float speed_ = 3.0f;
	float length_ = 35.0f;
	float amplitude_ = 1.0f;
	Vector2& direction_ = Vector2{1.0f, 0.0f};
	float angle_ = 180.0f;

	bool fadingEnabled_ = false;
	bool speedVariationEnabled_ = false;

	std::vector<Active> activeWaves_;
	std::vector<Fade> fadeInWaves_;
	std::vector<Fade> fadeOutWaves_;
};

}
