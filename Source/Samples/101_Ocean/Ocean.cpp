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
#include "Urho3D/Core/Context.h"
#include "Urho3D/Core/CoreEvents.h"
#include "Urho3D/Graphics/Geometry.h"
#include "Urho3D/Graphics/Model.h"
#include "Urho3D/Graphics/VertexBuffer.h"
#include "Urho3D/IO/Log.h"
#include "Urho3D/Resource/ResourceEvents.h"
#include "Urho3D/Core/Profiler.h"

#include "Ocean.h"
#include "OceanAlgorithms.h"


namespace Urho3D
{

extern const char* GEOMETRY_CATEGORY;

Ocean::Ocean(Context* context) : StaticModel(context)
{
    waveSystem_ = new WaveSystem(context);

    // Subscribe HandleUpdate() function for processing update events
    SubscribeToEvent(E_UPDATE, URHO3D_HANDLER(Ocean, HandleUpdate));
}

Ocean::~Ocean()
{
}

void Ocean::RegisterObject(Context* context)
{
    context->RegisterFactory<Ocean>(GEOMETRY_CATEGORY);

    URHO3D_COPY_BASE_ATTRIBUTES(StaticModel);
}

void Ocean::SetModel(Model* model)
{
    // Set the model for the base class StaticModel
    StaticModel::SetModel(model);

    // Extract the original vertices and duplicates from the water plane model
    Geometry* geom = model->GetGeometry(0, 0);
    waterVertexBuffer_ = geom ? SharedPtr<VertexBuffer>(geom->GetVertexBuffer(0)) : nullptr;
    if (waterVertexBuffer_)
    {
        originalVertices_ = ExtractVertexPositions(waterVertexBuffer_);
        vertexDuplicates_ = ExtractDuplicates(originalVertices_);
    }
}

void Ocean::HandleUpdate(StringHash eventType, VariantMap& eventData)
{
    URHO3D_PROFILE(Ocean);

    using namespace Update;

    // Take the frame time step, which is stored as a float
    float timeStep = eventData[P_TIMESTEP].GetFloat();

    // Increase overall time
    time_ += timeStep;

    if (!waterVertexBuffer_)
        return;

    // Get the offset for the normals
    unsigned int normalOffset = waterVertexBuffer_->GetElementOffset(SEM_NORMAL, 0);

    // Get the active waves from WaveSystem
    const PODVector<WaveSystem::Wave*>& waves = waveSystem_->GetWaves();

    if (waves.Size() > 0)
    {
        // Lock the vertex buffer for update and rewrite positions with sine wave modulated ones
        // Cannot use discard lock as there is other data (normals, UVs) that we are not overwriting
        unsigned char* vertexData = (unsigned char*)waterVertexBuffer_->Lock(0, waterVertexBuffer_->GetVertexCount());
        if (vertexData)
        {
            unsigned vertexSize = waterVertexBuffer_->GetVertexSize();
            unsigned numVertices = waterVertexBuffer_->GetVertexCount();

            {
                URHO3D_PROFILE(AnimateVertices);
                // Apply the Gerstner Wave calculations on each vertex in the water plane
                for (unsigned j = 0; j < numVertices; ++j)
                {
                    Vector3& src = originalVertices_[j];
                    Vector3& dest = *reinterpret_cast<Vector3*>(vertexData + j * vertexSize);
                    Vector3& destNormal = *reinterpret_cast<Vector3*>(vertexData + j * vertexSize + normalOffset);

                    // Calculate the new vertex position and normals
                    std::pair<Vector3, Vector3> gerstnerWave = CalculateGerstnerWaves(Vector2(src.x_, src.z_), time_, waves);
                    const Vector3 position = gerstnerWave.first;
                    const Vector3 normal = gerstnerWave.second;
                    dest = Vector3(position.x_, position.z_, position.y_);
                    destNormal = Vector3(normal.x_, normal.z_, normal.y_);
                }
            }

            waterVertexBuffer_->Unlock();
        }
    }

    // Update the WaveSystem
    waveSystem_->Update(timeStep);
}

}
