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

void WaveEditor::SetVisible(bool visible)
{
    window_->SetVisible(visible);
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
    //window_->SetMinWidth(256);
    //window_->SetMaxHeight(600);
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
    // Create Controls Pane
    UIElement* contentPane = new UIElement(context_);
    contentPane->SetLayout(LM_HORIZONTAL, 6);

    // Create Left and Right columns
    UIElement* leftColumn = new UIElement(context_);
    leftColumn->SetLayout(LM_VERTICAL, 6);

    UIElement* rightColumn = new UIElement(context_);
    rightColumn->SetLayout(LM_VERTICAL, 6);

    contentPane->AddChild(leftColumn);
    contentPane->AddChild(rightColumn);
    window_->AddChild(contentPane);

    // Number of created Waves
    Text* txtWaveCount = new Text(context_);
    txtWaveCount->SetName("txtWaveCount");
    txtWaveCount->SetText("Waves");
    txtWaveCount->SetStyleAuto();
    leftColumn->AddChild(txtWaveCount);

    editWaveCount_ = new LineEdit(context_);
    editWaveCount_->SetName("editWaveCount");
    editWaveCount_->SetStyleAuto();
    rightColumn->AddChild(editWaveCount_);

    // Lifetime of a wave
    Text* txtLifetime = new Text(context_);
    txtLifetime->SetName("txtLifetime");
    txtLifetime->SetText("Lifetime");
    txtLifetime->SetStyleAuto();
    leftColumn->AddChild(txtLifetime);

    editLifetime_ = new LineEdit(context_);
    editLifetime_->SetName("editLifetime_");
    editLifetime_->SetStyleAuto();
    rightColumn->AddChild(editLifetime_);

    // Steepness
    Text* txtSteepness = new Text(context_);
    txtSteepness->SetName("txtSteepness");
    txtSteepness->SetText("Steepness");
    txtSteepness->SetStyleAuto();
    leftColumn->AddChild(txtSteepness);

    editSteepness_ = new LineEdit(context_);
    editSteepness_->SetName("editSteepness_");
    editSteepness_->SetStyleAuto();
    rightColumn->AddChild(editSteepness_);

    // Length
    Text* txtLength = new Text(context_);
    txtLength->SetName("txtLength");
    txtLength->SetText("Length");
    txtLength->SetStyleAuto();
    leftColumn->AddChild(txtLength);

    editLength_ = new LineEdit(context_);
    editLength_->SetName("editLength");
    editLength_->SetStyleAuto();
    rightColumn->AddChild(editLength_);

    // Amplitude
    Text* txtAmplitude = new Text(context_);
    txtAmplitude->SetName("txtAmplitude");
    txtAmplitude->SetText("Amplitude");
    txtAmplitude->SetStyleAuto();
    leftColumn->AddChild(txtAmplitude);

    editAmplitude_ = new LineEdit(context_);
    editAmplitude_->SetName("editAmplitude");
    editAmplitude_->SetStyleAuto();
    rightColumn->AddChild(editAmplitude_);

    // Speed
    Text* txtSpeed = new Text(context_);
    txtSpeed->SetName("txtSpeed");
    txtSpeed->SetText("Speed");
    txtSpeed->SetStyleAuto();
    leftColumn->AddChild(txtSpeed);

    editSpeed_ = new LineEdit(context_);
    editSpeed_->SetName("editSpeed");
    editSpeed_->SetStyleAuto();
    rightColumn->AddChild(editSpeed_);

    // Direction
    Text* txtDirection = new Text(context_);
    txtDirection->SetName("txtDirection");
    txtDirection->SetText("Direction");
    txtDirection->SetStyleAuto();
    leftColumn->AddChild(txtDirection);

    UIElement* directionPane = new UIElement(context_);
    directionPane->SetLayout(LM_HORIZONTAL);
    rightColumn->AddChild(directionPane);

    editDirectionX_ = new LineEdit(context_);
    editDirectionX_->SetName("editDirectionX");
    editDirectionX_->SetStyleAuto();
    directionPane->AddChild(editDirectionX_);

    editDirectionY_ = new LineEdit(context_);
    editDirectionY_->SetName("editDirectionY");
    editDirectionY_->SetStyleAuto();
    directionPane->AddChild(editDirectionY_);

    editDirectionAngle_ = new LineEdit(context_);
    editDirectionAngle_->SetName("editDirectionAngle");
    editDirectionAngle_->SetStyleAuto();
    directionPane->AddChild(editDirectionAngle_);

    // Fading
    UIElement* fadingGroup = new UIElement(context_);
    fadingGroup->SetLayout(LM_HORIZONTAL, 6);

    leftColumn->AddChild(fadingGroup);

    cbFading_ = new CheckBox(context_);
    cbFading_->SetName("cbFading");
    cbFading_->SetStyleAuto();
    fadingGroup->AddChild(cbFading_);

    Text* txtFading = new Text(context_);
    txtFading->SetName("txtFading");
    txtFading->SetText("Fading");
    txtFading->SetStyleAuto();
    fadingGroup->AddChild(txtFading);

    // Speed Variation
    UIElement* speedGroup = new UIElement(context_);
    speedGroup->SetLayout(LM_HORIZONTAL, 6);

    rightColumn->AddChild(speedGroup);

    cbSpeed_ = new CheckBox(context_);
    cbSpeed_->SetName("cbSpeed");
    cbSpeed_->SetStyleAuto();
    speedGroup->AddChild(cbSpeed_);

    Text* txtSpeedVariation = new Text(context_);
    txtSpeedVariation->SetName("txtSpeedVariation");
    txtSpeedVariation->SetText("Speed Variation");
    txtSpeedVariation->SetStyleAuto();
    speedGroup->AddChild(txtSpeedVariation);

    // Create buttons
    Text* txtBtnSet = new Text(context_);
    txtBtnSet->SetName("txtBtnSet");
    txtBtnSet->SetText("Set");
    txtBtnSet->SetAlignment(HA_CENTER, VA_CENTER);
    txtBtnSet->SetStyleAuto();

    Button* buttonSet = new Button(context_);
    buttonSet->SetName("btnSet");
    buttonSet->SetMinHeight(24);
    buttonSet->SetStyleAuto();

    buttonSet->AddChild(txtBtnSet);
    leftColumn->AddChild(buttonSet);

    Text* txtBtnReset = new Text(context_);
    txtBtnReset->SetName("txtBtnReset");
    txtBtnReset->SetText("Reset");
    txtBtnReset->SetAlignment(HA_CENTER, VA_CENTER);
    txtBtnReset->SetStyleAuto();

    Button* buttonReset = new Button(context_);
    buttonReset->SetName("btnReset");
    buttonReset->SetMinHeight(24);
    buttonReset->SetStyleAuto();

    buttonReset->AddChild(txtBtnReset);
    rightColumn->AddChild(buttonReset);


    // Subscribe to buttons
    SubscribeToEvent(buttonSet, E_RELEASED, URHO3D_HANDLER(WaveEditor, HandleSetPressed));
    SubscribeToEvent(buttonReset, E_RELEASED, URHO3D_HANDLER(WaveEditor, HandleResetPressed));
}

void WaveEditor::HandleSetPressed(StringHash eventType, VariantMap& eventData)
{
    UpdateWaveSystem();
}

void WaveEditor::HandleResetPressed(StringHash eventType, VariantMap& eventData)
{
    if (SharedPtr<WaveSystem> waveManager = waveManagerWeak_.Lock())
    {
        UpdateWaveSystem();
        waveManager->Reset();
    }
}

void WaveEditor::UpdateEditor()
{
    if (SharedPtr<WaveSystem> waveSystem = waveManagerWeak_.Lock())
    {
        editWaveCount_->SetText(String(waveSystem->GetWaveCount()));
        editLifetime_->SetText(String(waveSystem->GetLifetime()));
        editSteepness_->SetText(String(waveSystem->GetSteepness()));
        editLength_->SetText(String(waveSystem->GetLength()));
        editAmplitude_->SetText(String(waveSystem->GetAmplitude()));
        editSpeed_->SetText(String(waveSystem->GetSpeed()));
        editDirectionX_->SetText(String(waveSystem->GetDirection().x_));
        editDirectionY_->SetText(String(waveSystem->GetDirection().y_));
        editDirectionAngle_->SetText(String(waveSystem->GetAngle()));
        cbFading_->SetChecked(waveSystem->FadingEnabled());
        cbSpeed_->SetChecked(waveSystem->SpeedVariationEnabled());
    }
}

void WaveEditor::UpdateWaveSystem()
{
    if (SharedPtr<WaveSystem> waveSystem = waveManagerWeak_.Lock())
    {
        waveSystem->SetWaveCount(ToInt(editWaveCount_->GetText()));
        waveSystem->SetLifetime(ToFloat(editLifetime_->GetText()));
        waveSystem->SetSteepness(ToFloat(editSteepness_->GetText()));
        waveSystem->SetLength(ToFloat(editLength_->GetText()));
        waveSystem->SetAmplitude(ToFloat(editAmplitude_->GetText()));
        waveSystem->SetSpeed(ToFloat(editSpeed_->GetText()));
        waveSystem->SetDirection(Vector2(ToFloat(editDirectionX_->GetText()), ToFloat(editDirectionY_->GetText())));
        waveSystem->SetAngle(ToFloat(editDirectionAngle_->GetText()));
        waveSystem->EnableFading(cbFading_->IsChecked());
        waveSystem->EnableSpeedVariation(cbSpeed_->IsChecked());
    }
}

}
