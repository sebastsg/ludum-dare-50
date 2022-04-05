#version 130

uniform sampler2D active_texture;
uniform vec4 color;

in vec2 v_TexCoords;

out vec4 out_Color;

void main() {
	out_Color = texture(active_texture, v_TexCoords).rgba * color;
}
