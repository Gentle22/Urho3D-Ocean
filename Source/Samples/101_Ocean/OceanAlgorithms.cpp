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

#include <Urho3D/Graphics/VertexBuffer.h>
#include <Urho3D/IO/Log.h>

#include "OceanAlgorithms.h"


namespace Urho3D
{

PODVector<Vector3> ExtractVertexPositions(VertexBuffer* vertexBuffer)
{
    PODVector<Vector3> vertices{};
    if (!vertexBuffer)
    {
        return vertices;
    }

    const unsigned char* vertexData = static_cast<const unsigned char*>(vertexBuffer->Lock(0, vertexBuffer->GetVertexCount()));
    if (!vertexData)
    {
        URHO3D_LOGERROR("Failed to lock the model vertex buffer to get original vertices");
        vertexBuffer->Unlock();
        return vertices;
    }

    unsigned numVertices = vertexBuffer->GetVertexCount();
    unsigned vertexSize = vertexBuffer->GetVertexSize();

    // Copy the original vertex positions
    for (unsigned i = 0; i < numVertices; ++i)
    {
        const Vector3& src = *reinterpret_cast<const Vector3*>(vertexData + i * vertexSize);
        vertices.Push(src);
    }
    vertexBuffer->Unlock();

    return vertices;
}

PODVector<unsigned> ExtractDuplicates(const PODVector<Vector3>& vertexPositions)
{
    PODVector<unsigned> duplicates{};

    // Detect duplicate vertices to allow seamless animation
    duplicates.Resize(vertexPositions.Size());
    for (unsigned i = 0; i < vertexPositions.Size(); ++i)
    {
        duplicates[i] = i; // Assume not a duplicate
        for (unsigned j = 0; j < i; ++j)
        {
            if (vertexPositions[i].Equals(vertexPositions[j]))
            {
                duplicates[i] = j;
                break;
            }
        }
    }
    return duplicates;
}

Vector3 CalculateGerstnerWavePosition(const Vector2 P, const float t, const float q, const float a, const Vector2& dir, const float w, const float phi)
{
    float inner = w * dir.DotProduct(P) + phi * t;

    float x = q * a * dir.x_ * cos(inner);
    float y = q * a * dir.y_ * cos(inner);
    float z = a * sin(inner);

    return Vector3(x, y, z);
}

Vector3 CalculateGerstnerWaveNormal(const Vector2 P, const float t, const float q, const float a, const Vector2& dir, const float w, const float phi)
{
    float inner = w * dir.DotProduct(P) + phi * t;

    float x = dir.x_ * w * a * cos(inner);
    float y = dir.y_ * w * a * cos(inner);
    float z = q * w * a * sin(inner);

    return Vector3(x, y, z);
};

std::pair<Vector3, Vector3> CalculateGerstnerWaves(const Vector2 P, const float t, const PODVector<WaveSystem::Wave*>& waves)
{
    Vector3 medianWave{};
    Vector3 normal{};
    for (const auto& wave : waves)
    {
        float w = 0.f;
        if (wave->l_ != 0.f)
            w = 2.0f * M_PI / wave->l_;

        float q = 0.f;
        if (w != 0.f && wave->a_ != 0.f && waves.Size() != 0)
            q = wave->q_ / (w * wave->a_ *  waves.Size());

        float phi = wave->s_ * w;
        medianWave += CalculateGerstnerWavePosition(P, t, q, wave->a_, wave->d_, w, phi);
        normal += CalculateGerstnerWaveNormal(P, t, q, wave->a_, wave->d_, w, phi);
    }

    return std::pair<Vector3, Vector3>{
        Vector3(P.x_ + medianWave.x_, P.y_ + medianWave.y_, medianWave.z_),
            Vector3(-normal.x_, -normal.y_, 1 - normal.z_)
    };
};
        
}
