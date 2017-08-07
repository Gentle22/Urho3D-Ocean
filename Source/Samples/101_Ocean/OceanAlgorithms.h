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

#include <Urho3D/Container/Vector.h>
#include <Urho3D/Math/Vector3.h>

#include "WaveSystem.h"

#include <tuple>


namespace Urho3D
{
// Forward declarations
class VertexBuffer;

using PositionAndNormal = std::pair<Vector3, Vector3>;

/// Extract the Vertex positions from a VertexBuffer
PODVector<Vector3> ExtractVertexPositions(VertexBuffer* vertexBuffer);
/// Extract duplicated vertices from a list of vertices
PODVector<unsigned> ExtractDuplicates(const PODVector<Vector3>& vertexPositions);

/// Calculate the new vertex position by applying the Gerstner Wave function
Vector3 CalculateGerstnerWavePosition(const Vector2 P, const float t, const float q, const float a,
    const Vector2& dir, const float w, const float phi);
/// Calculate the new vertex normal by applying the Gerstner Wave function
Vector3 CalculateGerstnerWaveNormal(const Vector2 P, const float t, const float q, const float a,
    const Vector2& dir, const float w, const float phi);
/// Calculate the sum of all Gerstner waves and return the new vertex position and normal
PositionAndNormal CalculateGerstnerWaves(const Vector2 P, const float t, const PODVector<WaveSystem::Wave*>& waves);

}
