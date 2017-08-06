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
#include <Urho3D/Core/StringUtils.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/UI/UI.h>
#include <Urho3D/UI/UIElement.h>
#include <Urho3D/UI/Window.h>
#include <Urho3D/UI/Text.h>
#include <Urho3D/UI/LineEdit.h>
#include <Urho3D/UI/CheckBox.h>
#include <Urho3D/UI/Button.h>
#include <Urho3D/UI/UIEvents.h>

#include "WaveEditor.h"


namespace Urho3D
{

WaveEditor::WaveEditor(Context* context, WeakPtr<WaveSystem> waveManager) :
	Object(context),
	waveManagerWeak_(waveManager),
	uiRoot_(GetSubsystem<UI>()->GetRoot())
{
	CreateWindow();
	CreateControls();
	UpdateEditor();
}

WaveEditor::~WaveEditor()
{
}

void WaveEditor::CreateWindow()
{
	// Load XML file containing default UI style sheet
	ResourceCache* cache = GetSubsystem<ResourceCache>();
	XMLFile* style = cache->GetResource<XMLFile>("UI/DefaultStyle.xml");

	// Set the loaded style as default style
	uiRoot_->SetDefaultStyle(style);

	// Create the Window and add it to the UI's root node
	window_ = new Window(context_);
	uiRoot_->AddChild(window_);

	// Set Window size and layout settings
	window_->SetMinWidth(256);
	window_->SetMaxHeight(600);
	window_->SetLayout(LM_VERTICAL, 6, IntRect(6, 6, 6, 6));
	window_->SetAlignment(HA_LEFT, VA_TOP);
	window_->SetName("Window");

	// Create Window 'titlebar' container
	UIElement* titleBar = new UIElement(context_);
	titleBar->SetMinSize(0, 24);
	titleBar->SetVerticalAlignment(VA_TOP);
	titleBar->SetLayoutMode(LM_HORIZONTAL);

	// Create the Window title Text
	Text* windowTitle = new Text(context_);
	windowTitle->SetName("WindowTitle");
	windowTitle->SetText("Gerstner Wave Editor");

	// Add the controls to the title bar
	titleBar->AddChild(windowTitle);

	// Add the title bar to the Window
	window_->AddChild(titleBar);

	// Apply styles
	window_->SetStyleAuto();
	windowTitle->SetStyleAuto();
}

void WaveEditor::CreateControls()
{
	// Steepness
	Text* txtSteepness = new Text(context_);
	txtSteepness->SetName("txtSteepness");
	txtSteepness->SetText("Steepness (0 - 1)");
	txtSteepness->SetStyleAuto();
	window_->AddChild(txtSteepness);

	editSteepness_ = new LineEdit(context_);
	editSteepness_->SetName("LineEdit");
	editSteepness_->SetText("0");
	editSteepness_->SetMinHeight(16);
	editSteepness_->SetStyleAuto();
	window_->AddChild(editSteepness_);

	// Length
	Text* txtLength = new Text(context_);
	txtLength->SetName("txtLength");
	txtLength->SetText("Length");
	txtLength->SetStyleAuto();
	window_->AddChild(txtLength);

	editLength_ = new LineEdit(context_);
	editLength_->SetName("editLength");
	editLength_->SetMinHeight(16);
	editLength_->SetStyleAuto();
	window_->AddChild(editLength_);

	// Amplitude
	Text* txtAmplitude = new Text(context_);
	txtAmplitude->SetName("txtAmplitude");
	txtAmplitude->SetText("Amplitude");
	txtAmplitude->SetStyleAuto();
	window_->AddChild(txtAmplitude);

	editAmplitude_ = new LineEdit(context_);
	editAmplitude_->SetName("editAmplitude");
	editAmplitude_->SetMinHeight(16);
	editAmplitude_->SetStyleAuto();
	window_->AddChild(editAmplitude_);

	// Speed
	Text* txtSpeed = new Text(context_);
	txtSpeed->SetName("txtSpeed");
	txtSpeed->SetText("Speed");
	txtSpeed->SetStyleAuto();
	window_->AddChild(txtSpeed);

	editSpeed_ = new LineEdit(context_);
	editSpeed_->SetName("editSpeed");
	editSpeed_->SetMinHeight(16);
	editSpeed_->SetStyleAuto();
	window_->AddChild(editSpeed_);

	// Direction
	Text* txtDirection = new Text(context_);
	txtDirection->SetName("txtDirection");
	txtDirection->SetText("Direction (x, y, variation angle)");
	txtDirection->SetStyleAuto();
	window_->AddChild(txtDirection);

	editDirectionX_ = new LineEdit(context_);
	editDirectionX_->SetName("editDirectionX");
	editDirectionX_->SetMinHeight(16);
	editDirectionX_->SetStyleAuto();
	window_->AddChild(editDirectionX_);

	editDirectionY_ = new LineEdit(context_);
	editDirectionY_->SetName("editDirectionY");
	editDirectionY_->SetMinHeight(16);
	editDirectionY_->SetStyleAuto();
	window_->AddChild(editDirectionY_);

	editDirectionAngle_ = new LineEdit(context_);
	editDirectionAngle_->SetName("editDirectionAngle");
	editDirectionAngle_->SetMinHeight(16);
	editDirectionAngle_->SetStyleAuto();
	window_->AddChild(editDirectionAngle_);

	// Fading
	Text* txtFading = new Text(context_);
	txtFading->SetName("txtFading");
	txtFading->SetText("Fading Enabled");
	txtFading->SetStyleAuto();
	window_->AddChild(txtFading);

	cbFading_ = new CheckBox(context_);
	cbFading_->SetName("cbFading");
	cbFading_->SetStyleAuto();
	window_->AddChild(cbFading_);

	// Speed Variation
	Text* txtSpeedVariation = new Text(context_);
	txtSpeedVariation->SetName("txtSpeedVariation");
	txtSpeedVariation->SetText("Speed Variation Enabled");
	txtSpeedVariation->SetStyleAuto();
	window_->AddChild(txtSpeedVariation);

	cbSpeed_ = new CheckBox(context_);
	cbSpeed_->SetName("cbSpeed");
	cbSpeed_->SetStyleAuto();
	window_->AddChild(cbSpeed_);

	// Create buttons
	Text* txtBtnSet = new Text(context_);
	txtBtnSet->SetName("txtBtnSet");
	txtBtnSet->SetText("Set");
	txtBtnSet->SetStyleAuto();

	Button* buttonSet = new Button(context_);
	buttonSet->SetName("btnSet");
	buttonSet->SetMinHeight(24);
	buttonSet->SetStyleAuto();

	buttonSet->AddChild(txtBtnSet);
	window_->AddChild(buttonSet);

	Text* txtBtnReset = new Text(context_);
	txtBtnReset->SetName("txtBtnReset");
	txtBtnReset->SetText("Reset");
	txtBtnReset->SetStyleAuto();

	Button* buttonReset = new Button(context_);
	buttonReset->SetName("btnReset");
	buttonReset->SetMinHeight(24);
	buttonReset->SetStyleAuto();

	buttonReset->AddChild(txtBtnReset);
	window_->AddChild(buttonReset);

	// Subscribe to buttons
	SubscribeToEvent(buttonSet, E_RELEASED, URHO3D_HANDLER(WaveEditor, HandleSetPressed));
	SubscribeToEvent(buttonReset, E_RELEASED, URHO3D_HANDLER(WaveEditor, HandleResetPressed));
}

void WaveEditor::HandleSetPressed(StringHash eventType, VariantMap& eventData)
{
	UpdateWaveManager();
}

void WaveEditor::HandleResetPressed(StringHash eventType, VariantMap& eventData)
{
	if (SharedPtr<WaveSystem> waveManager = waveManagerWeak_.Lock())
	{
		UpdateWaveManager();
		waveManager->Reset();
	}
}

void WaveEditor::UpdateEditor()
{
	if (SharedPtr<WaveSystem> waveManager = waveManagerWeak_.Lock())
	{
		editSteepness_->SetText(String(waveManager->GetSteepness()));
		editLength_->SetText(String(waveManager->GetLength()));
		editAmplitude_->SetText(String(waveManager->GetAmplitude()));
		editSpeed_->SetText(String(waveManager->GetSpeed()));
		editDirectionX_->SetText(String(waveManager->GetDirection().x_));
		editDirectionY_->SetText(String(waveManager->GetDirection().y_));
		editDirectionAngle_->SetText(String(waveManager->GetAngle()));
		cbFading_->SetChecked(waveManager->FadingEnabled());
		cbSpeed_->SetChecked(waveManager->SpeedVariationEnabled());
	}
}

void WaveEditor::UpdateWaveManager()
{
	if (SharedPtr<WaveSystem> waveManager = waveManagerWeak_.Lock())
	{
		waveManager->SetSteepness(ToFloat(editSteepness_->GetText()));
		waveManager->SetLength(ToFloat(editLength_->GetText()));
		waveManager->SetAmplitude(ToFloat(editAmplitude_->GetText()));
		waveManager->SetSpeed(ToFloat(editSpeed_->GetText()));
		waveManager->SetDirection(Vector2(ToFloat(editDirectionX_->GetText()), ToFloat(editDirectionY_->GetText())));
		waveManager->SetAngle(ToFloat(editDirectionAngle_->GetText()));
		waveManager->EnableFading(cbFading_->IsChecked());
		waveManager->EnableSpeedVariation(cbSpeed_->IsChecked());
	}
}

}
