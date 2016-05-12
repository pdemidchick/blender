/*
 * Copyright 2011-2013 Blender Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __SUBD_DICE_H__
#define __SUBD_DICE_H__

/* DX11 like EdgeDice implementation, with different tessellation factors for
 * each edge for watertight tessellation, with subpatch remapping to work with
 * DiagSplit. For more algorithm details, see the DiagSplit paper or the
 * ARB_tessellation_shader OpenGL extension, Section 2.X.2. */

#include "util_types.h"
#include "util_vector.h"
#include "kernel_types.h"

CCL_NAMESPACE_BEGIN

class Camera;
class Patch;

struct SubdParams {
	int test_steps;
	int split_threshold;
	float dicing_rate;
	int max_level;
	int max_T;
	Camera *camera;
	Transform objecttoworld;

	TessellatedSubPatch* subpatch;

	SubdParams()
	{
		test_steps = 3;
		split_threshold = 1;
		dicing_rate = 0.1f;
		max_level = 12;
		max_T = 128;
		camera = NULL;
		subpatch = NULL;
	}
};

/* EdgeDice Base */

class EdgeDice {
public:
	SubdParams params;
	size_t vert_offset;
	size_t tri_offset;

	explicit EdgeDice(const SubdParams& params);

	void reserve(int num_verts, int num_tris);

	int add_vert(Patch *patch, float2 uv);
	void add_triangle(Patch *patch, int v0, int v1, int v2);

	void stitch_triangles(Patch *patch, vector<int>& outer, vector<int>& inner);
};

/* Quad EdgeDice
 *
 * Edge tessellation factors and subpatch coordinates are as follows:
 *
 *            tu1
 *     P01 --------- P11 
 *     |               |
 * tv0 |               | tv1
 *     |               |
 *     P00 --------- P10
 *            tu0
 */

class QuadDice : public EdgeDice {
public:
	struct SubPatch {
		Patch *patch;

		float2 P00;
		float2 P10;
		float2 P01;
		float2 P11;
	};

	struct EdgeFactors {
		int tu0;
		int tu1;
		int tv0;
		int tv1;
	};

	explicit QuadDice(const SubdParams& params);

	void diced_size(SubPatch& sub, EdgeFactors& ef, uint* num_verts, uint* num_tris);
	void calc_size(EdgeFactors& ef, int Mu, int Mv, uint* num_verts, uint* num_tris);
	void reserve(EdgeFactors& ef, int Mu, int Mv);
	float3 eval_projected(SubPatch& sub, float u, float v);

	float2 map_uv(SubPatch& sub, float u, float v);
	int add_vert(SubPatch& sub, float u, float v);

	void add_corners(SubPatch& sub);
	void add_grid(SubPatch& sub, int Mu, int Mv, int offset);

	void add_side_u(SubPatch& sub,
		vector<int>& outer, vector<int>& inner,
		int Mu, int Mv, int tu, int side, int offset);

	void add_side_v(SubPatch& sub,
		vector<int>& outer, vector<int>& inner,
		int Mu, int Mv, int tv, int side, int offset);

	float quad_area(const float3& a, const float3& b, const float3& c, const float3& d);
	float scale_factor(SubPatch& sub, EdgeFactors& ef, int Mu, int Mv);

	void dice(SubPatch& sub, EdgeFactors& ef);
};

/* Triangle EdgeDice
 *
 * Edge tessellation factors and subpatch coordinates are as follows:
 *
 *        Pw
 *        /\
 *    tv /  \ tu
 *      /    \
 *     /      \
 *  Pu -------- Pv
 *        tw     
 */

class TriangleDice : public EdgeDice {
public:
	struct SubPatch {
		Patch *patch;

		float2 Pu;
		float2 Pv;
		float2 Pw;
	};

	struct EdgeFactors {
		int tu;
		int tv;
		int tw;
	};

	explicit TriangleDice(const SubdParams& params);

	void diced_size(SubPatch& sub, EdgeFactors& ef, uint* num_verts, uint* num_tris);
	void calc_size(EdgeFactors& ef, int M, uint* num_verts, uint* num_tris);
	void reserve(EdgeFactors& ef, int M);

	float2 map_uv(SubPatch& sub, float2 uv);
	int add_vert(SubPatch& sub, float2 uv);

	void add_grid(SubPatch& sub, EdgeFactors& ef, int M);
	void dice(SubPatch& sub, EdgeFactors& ef);
};

CCL_NAMESPACE_END

#endif /* __SUBD_DICE_H__ */

