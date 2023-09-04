#version 450

layout(binding = 0) uniform modelMatrix {
    mat4 modelLocation;
} model;

layout(binding = 1) uniform ProjectionMatrix {
    mat4 ortho;
} projMatrix;

layout(location = 0) in vec2 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inTexCoord;

layout( push_constant ) uniform constants
{
	uint glyphID;
} PushConstants;

struct resultStruct
{
    vec4 Invcolor;				// Vertex color.
	vec2 Intexcoord;				// Em-space glyph coordinates for pixel.
	uint glyphID;		// x, y location of first header in fontDataSampler
};

layout(location = 0) out resultStruct vResult;

void main() {
    gl_Position = projMatrix.ortho * model.modelLocation * vec4(inPosition, 0.5f, 1.0f);

    vResult = resultStruct(vec4(inColor, 1.0), inTexCoord, PushConstants.glyphID);
}