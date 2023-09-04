#version 450

layout(location = 0) in resultStruct
{
    vec4 Invcolor;				// Vertex color.
	vec2 Intexcoord;				// Em-space glyph coordinates for pixel.
	flat uvec2 InstartingGlyphHeaderLocation;		// x, y location of first header in fontDataSampler
} vResult;


layout(binding = 2) uniform sampler2D pointSampler;
layout(binding = 3) uniform usampler2D fontDataSampler;

layout(location = 0) out vec4 outColor;

const float kQuadraticEpsilon = 0.0001;

vec4 vcolor = vResult.Invcolor;
vec2 texcoord = vResult.Intexcoord;
uvec2 startingGlyphHeaderLocation = vResult.InstartingGlyphHeaderLocation;

void main() {
    float coverage = 0.0;

    vec2 pixelsPerEm = vec2(1.0 / fwidth(texcoord.x), 1.0 / fwidth(texcoord.y));

    uvec2 bandIndex = uvec2(clamp(ivec2(texcoord * 16), ivec2(0U, 0U), ivec2(16U,16U)));

    uvec2 hbandData = texelFetch(fontDataSampler, ivec2(startingGlyphHeaderLocation.x + bandIndex.y, startingGlyphHeaderLocation.y), 0).xy;
	uvec2 hbandLoc = uvec2(startingGlyphHeaderLocation.x + hbandData.y, startingGlyphHeaderLocation.y);

    hbandLoc.y += hbandLoc.x >> 12U;
	hbandLoc.x &= 0x0FFFU;

    for(uint curve = 0U; curve < hbandData.x; curve++)
    {

        uvec2 curveLocation = uvec2(hbandLoc.x + curve, hbandLoc.y);

        curveLocation.y += curveLocation.x >> 12U;//Add one to y if x exceeds 4096 (texture width)
	    curveLocation.x &= 0x0FFFU;//Roll x value back over

        ivec2 curveLoc = ivec2(texelFetch(fontDataSampler, ivec2(curveLocation.x, curveLocation.y), 0).xy);

        vec4 p12 = texelFetch(pointSampler, curveLoc, 0);
        vec2 p3 = texelFetch(pointSampler, ivec2(curveLoc.x + 1, curveLoc.y), 0).xy;

        //if(max(max(p12.x, p12.z), p3.x) * pixelsPerEm.x < -0.5) break;

        uint code = (0x2E74U >> (((p12.y > 0.0) ? 2U : 0U) + ((p12.w > 0.0) ? 4U : 0U) + ((p3.y > 0.0) ? 8U : 0U))) & 3U;

        if (code != 0U)
		{
			float ax = p12.x - p12.z * 2.0 + p3.x;
			float ay = p12.y - p12.w * 2.0 + p3.y;
			float bx = p12.x - p12.z;
			float by = p12.y - p12.w;
			float ra = 1.0 / ay;

            float d = sqrt(max(by * by - ay * p12.y, 0.0));
			float t1 = (by - d) * ra;
			float t2 = (by + d) * ra;

            if (abs(ay) < kQuadraticEpsilon) t1 = t2 = p12.y * 0.5 / by;

            float x1 = (ax * t1 - bx * 2.0) * t1 + p12.x;
			float x2 = (ax * t2 - bx * 2.0) * t2 + p12.x;
			x1 = clamp(x1 * pixelsPerEm.x + 0.5, 0.0, 1.0);
			x2 = clamp(x2 * pixelsPerEm.x + 0.5, 0.0, 1.0);

            if ((code & 1U) != 0U) coverage += x1;
			if (code > 1U) coverage -= x2;
		}
    }

    uvec2 vbandData = texelFetch(fontDataSampler, ivec2(startingGlyphHeaderLocation.x + 16 + bandIndex.x, startingGlyphHeaderLocation.y), 0).xy;
	uvec2 vbandLoc = uvec2(startingGlyphHeaderLocation.x + vbandData.y, startingGlyphHeaderLocation.y);

    vbandLoc.y += vbandLoc.x >> 12U;
	vbandLoc.x &= 0x0FFFU;

    for (uint curve = 0U; curve < vbandData.x; curve++)
	{
        uvec2 curveLocation = uvec2(vbandLoc.x + curve, vbandLoc.y);

        curveLocation.y += curveLocation.x >> 12U;//Add one to y if x exceeds 4096 (texture width)
	    curveLocation.x &= 0x0FFFU;//Roll x value back over

        ivec2 curveLoc = ivec2(texelFetch(fontDataSampler, ivec2(curveLocation.x, curveLocation.y), 0).xy);

        vec4 p12 = texelFetch(pointSampler, curveLoc, 0) - vec4(texcoord, texcoord);

        curveLocation.x += 1U;//THIS IS IMPORTANT I ADDED 1 to the x coord for next point
        
        curveLocation.y += curveLocation.x >> 12U;//Add one to y if x exceeds 4096 (texture width)
	    curveLocation.x &= 0x0FFFU;//Roll x value back over

        curveLoc = ivec2(texelFetch(fontDataSampler, ivec2(curveLocation.x, curveLocation.y), 0).xy);

		vec2 p3 = texelFetch(pointSampler, ivec2(curveLoc.x, curveLoc.y), 0).xy - texcoord;

        
        //if (max(max(p12.y, p12.w), p3.y) * pixelsPerEm.y < -0.5) break;

        uint code = (0x2E74U >> (((p12.x > 0.0) ? 2U : 0U) + ((p12.z > 0.0) ? 4U : 0U) + ((p3.x > 0.0) ? 8U : 0U))) & 3U;

        if (code != 0U)
		{
			// At least one root makes a contribution, so solve for the
			// values of t where the rotated curve crosses y = 0.

			float ax = p12.y + p12.w * 2.0 + p3.y;
			float ay = p12.x - p12.z * 2.0 + p3.x;
			float bx = p12.y - p12.w;
			float by = p12.x - p12.z;
			float ra = 1.0 / ay;

			float d = sqrt(max(by * by - ay * p12.x, 0.0));
			float t1 = (by - d) * ra;
			float t2 = (by + d) * ra;

			if (abs(ay) < kQuadraticEpsilon) t1 = t2 = p12.x * 0.5 / by;

			float x1 = (ax * t1 - bx * 2.0) * t1 + p12.y;
			float x2 = (ax * t2 - bx * 2.0) * t2 + p12.y;
			x1 = clamp(x1 * pixelsPerEm.y + 0.5, 0.0, 1.0);
			x2 = clamp(x2 * pixelsPerEm.y + 0.5, 0.0, 1.0);

			if ((code & 1U) != 0U) coverage += x1;
			if (code > 1U) coverage -= x2;
		}
    }

    coverage = sqrt(clamp(abs(coverage) * 0.5, 0.0, 1.0));

    float alpha = coverage * vcolor.w;

	outColor = vec4(vcolor.xyz, alpha);

	if(coverage == 0.0)
	{
		outColor = vec4(1.0, 1.0, 1.0, 1.0);
	}
}