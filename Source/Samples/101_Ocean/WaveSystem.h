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

#include <Urho3D/Core/Object.h>
#include <Urho3D/Math/Vector2.h>
#include <Urho3D/Container/Vector.h>

#include <tuple>
#include <memory>
#include <vector>

namespace Urho3D
{

/// The WaveSystem is responsible to create and manage waves used by the Ocean component.
class WaveSystem : public Object
{
    URHO3D_OBJECT(WaveSystem, Object);

public:

    /// Stores the values of a Wave
    struct Wave
    {
        Wave(const float q, const float s, const float l, const float a, const Vector2 d) :
            q_{ q }, s_{ s }, l_{ l }, a_{ a }, d_{ d }
        {}

        /// Steepness between 0.f and 1.f
        float q_;
        /// The speed of a Wave
        float s_;
        /// The length between the crests of the Wave
        float l_;
        /// The amplitude
        float a_;
        /// The movement direction of the Wave
        Vector2 d_;
    };

    WaveSystem(Context* context);
    ~WaveSystem();

    /// Setter and Getter
    void SetWaveCount(const int value) { numWaves_ = value; }
    int GetWaveCount() const { return numWaves_; }

    void SetLifetime(const float value) { lifetime_ = value; }
    float GetLifetime() const { return lifetime_; }

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

    /// Returns the active waves
    const PODVector<Wave*> GetWaves() const;

private:

    /// Stores data needed for fading in and out a Wave
    struct Fade
    {
        float targetAmplitude_;
        float fadeAmplitude_;
        float targetSteepness_;
        float fadeSteepness_;
        Wave* wave_;
    };

    /// Stores data for an active Wave
    struct Active
    {
        float lifeTime_;
        bool isFadingOut;
        std::shared_ptr<Wave> wave_;
    };

    /// Creates a new Wave based on the source values
    Wave* CreateWave();
    void FadeInWave();
    void FadeOutWave(Wave* wave);
    void EraseActiveWave(Wave* wave);
    
    /// Max number of active waves
    int numWaves_ = 6;

    /// The lifetime of the wave in seconds
    float lifetime_ = 15.0f;

    /// Source values from which a new wave is created
    float steepness_ = 0.1f;
    float speed_ = 0.7f;
    float length_ = 3.5f;
    float amplitude_ = 0.04f;
    Vector2& direction_ = Vector2{1.0f, 0.0f};
    float angle_ = 90.f;

    bool fadingEnabled_ = true;

    /// Duration of fading in seconds
    const float fadeDuration_ = 3.f;

    /// If enabled, the speed of waves varies
    bool speedVariationEnabled_ = false;

    std::vector<Active> activeWaves_;
    std::vector<Fade> fadeInWaves_;
    std::vector<Fade> fadeOutWaves_;
};

}
