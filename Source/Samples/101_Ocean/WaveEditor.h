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

#include "WaveSystem.h"


namespace Urho3D
{

/// Forward Declarations
class UIElement;
class Window;
class LineEdit;
class CheckBox;


class WaveEditor : public Object
{
	URHO3D_OBJECT(WaveEditor, Object);

public:

	WaveEditor(Context* context, WeakPtr<WaveSystem> waveManager);
    ~WaveEditor();

private:

	void CreateWindow();
	void CreateControls();

	void HandleSetPressed(StringHash eventType, VariantMap& eventData);
	void HandleResetPressed(StringHash eventType, VariantMap& eventData);

	/// Updates the Editor fields with data from Wave Manager
	void UpdateEditor();
	/// Updates the Wave Manager with the new values
	void UpdateWaveManager();

	/// The UI's root UIElement.
	SharedPtr<UIElement> uiRoot_;
	/// The Window.
	SharedPtr<Window> window_;
	/// Controls
	LineEdit* editSteepness_ = nullptr;
	LineEdit* editLength_ = nullptr;
	LineEdit* editAmplitude_ = nullptr;
	LineEdit* editSpeed_ = nullptr;
	LineEdit* editDirectionX_ = nullptr;
	LineEdit* editDirectionY_ = nullptr;
	LineEdit* editDirectionAngle_ = nullptr;
	CheckBox* cbFading_	= nullptr;
	CheckBox* cbSpeed_	= nullptr;

	/// The wave manager
	WeakPtr<WaveSystem> waveManagerWeak_;
};

}