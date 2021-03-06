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

#include <Urho3D/Graphics/StaticModel.h>

#include "WaveSystem.h"

namespace Urho3D
{

class Model;

/// Ocean component.
class URHO3D_API Ocean : public StaticModel
{
    URHO3D_OBJECT(Ocean, StaticModel);

public:
    Ocean(Context* context);
    ~Ocean();

    /// Register object factory. Drawable must be registered first.
    static void RegisterObject(Context* context);

    /// Set the model to use as the water plane
    void SetModel(Model* model);

    SharedPtr<WaveSystem> GetWaveManager() { return waveSystem_; }

private:
    /// Handle the logic update event.
    void HandleUpdate(StringHash eventType, VariantMap& eventData);
    /// Handle model reload finished.
    void HandleModelReloadFinished(StringHash eventType, VariantMap& eventData);

    /// Water plane's vertex buffer that we will animate.
    SharedPtr<VertexBuffer> waterVertexBuffer_;
    /// Stores the original vertices of the water plane model
    PODVector<Vector3> originalVertices_;
    /// Stores vertex duplicates
    PODVector<unsigned> vertexDuplicates_;

    SharedPtr<WaveSystem> waveSystem_;

    float time_ = 0.0f;
};

}
