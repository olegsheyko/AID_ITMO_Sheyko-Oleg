#version 330 core

layout (location = 0) in vec3 aPos;

uniform vec2 uPosition;
uniform float uScale;
uniform float uRotation;
uniform vec2 uCenter;

void main() {
	vec2 pos = aPos.xy * uScale;

	pos -= uCenter * uScale;

	float cosTheta = cos(uRotation);
	float sinTheta = sin(uRotation);
	vec2 rotatedPos = vec2(
		pos.x * cosTheta - pos.y * sinTheta,
		pos.x * sinTheta + pos.y * cosTheta
	);

	rotatedPos += uPosition;

	gl_Position = vec4(rotatedPos, aPos.z, 1.0);
}