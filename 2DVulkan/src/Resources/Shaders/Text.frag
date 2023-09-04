#version 450

layout(location = 0) in resultStruct
{
    vec4 Invcolor;				// Vertex color.
	vec2 Intexcoord;				// Em-space glyph coordinates for pixel.
	flat uint glyphID;		// x, y location of first header in fontDataSampler
} vResult;


layout(binding = 2) uniform sampler2D pointSampler;
layout(binding = 3) uniform usampler2D fontDataSampler;

layout(location = 0) out vec4 outColor;

const float kQuadraticEpsilon = 0.0001;

vec4 vcolor = vResult.Invcolor;
vec2 pixelLoc = vResult.Intexcoord;
uint glyphID = vResult.glyphID;

float sample_y(float t, vec2 curve[3])
{
	vec2 p0 = curve[0], p1 = curve[1], p2 = curve[2];
	return t * t * (p0.y - 2 * p1.y + p2.y) + t * (2 * p1.y - 2 * p0.y) + p0.y;
}

int wind_delta(vec2 curve[3])
{
	vec2 p0 = curve[0], p1 = curve[1], p2 = curve[2];

	float[2] res;
	bool[2] valid;

	float a = p0.x - 2 * p1.x + p2.x;
	float b = 2 * p1.x - 2 * p0.x;
	float c = p0.x - pixelLoc.x;
	float det = b * b - 4 * a * c;
	res[0] = (-b - sqrt(det)) / (2 * a);
	res[1] = (-b + sqrt(det)) / (2 * a);
	valid[0] = valid[1] = det >= 0;

	if(abs(a) < 1e-3)
	{
		res[0] = res[1] = -c / b;
		valid[0] = valid[1] = true;
	}

	valid[0] = valid[0] && (sample_y(res[0], curve) < pixelLoc.y);//Try flipping the less than
	valid[1] = valid[1] && (sample_y(res[1], curve) < pixelLoc.y);//Try flipping the less than


	int ret = 0;
	int klass = 0x2e74 >> ((p0.x > pixelLoc.x ? 2 : 0) + (p1.x > pixelLoc.x ? 4 : 0) + (p2.x > pixelLoc.x ? 8 : 0));

	if((klass & 1) != 0 && valid[0])
	{
		ret++;
	}
	if((klass & 2) != 0 && valid[1])
	{
		ret--;
	}

	return ret;
}

void main() {
    int w = 0;

	uint bezierCount = texelFetch(fontDataSampler, ivec2(0, glyphID), 0).x;

	for(int i = 0; i < bezierCount; i++)
	{
		vec2 points[3];
		
		points[0] = texelFetch(pointSampler, ivec2((i * 2), glyphID), 0).xy;
		points[1] = texelFetch(pointSampler, ivec2((i * 2) + 1, glyphID), 0).xy;
		points[2] = texelFetch(pointSampler, ivec2((i * 2) + 2, glyphID), 0).xy;

		if(points[1].x < 0 && points[1].y < 0)
		{
			continue;
		}

		points[0] = abs(points[0] - 1);
		points[1] = abs(points[1] - 1);
		points[2] = abs(points[2] - 1);

		w += wind_delta(points);
	}

	if(w != 0)
	{
	outColor = vec4(vcolor.xyz, 1.0);
	}
	else
	{
		outColor = vec4(1.0, 1.0, 1.0, 1.0);
	}

}