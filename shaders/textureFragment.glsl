#version 410 core

//----------------------------------------------------------------------------------------------------------------------
/// \file textureFragment.glsl
/// \brief Draws a texture to the screen. I mostly use this for drawing text.
/// \author Ben Hawkyard
/// \version 1.0
/// \date 11/04/16
/// Revision History :
/// This is an initial version used for the game
//----------------------------------------------------------------------------------------------------------------------

in vec2 UV;

uniform sampler2D diffuse;
uniform vec4 inColour;

layout(location = 0) out vec4 fragColour;

void main()
{
    fragColour = texture( diffuse, UV );
    fragColour *= inColour;
}
