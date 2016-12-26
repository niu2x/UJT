#ifndef __defaultShader
#define __defaultShader

#define GLSL(src) "#version 430 core\n" #src

const GLchar* default_vertexSource = GLSL(
    in vec4 position;
    in vec3 color;
    in vec2 uv;
    out vec3 mColor;
    void main() {
        mColor = color;
        gl_Position = vec4(position, 0.0f, 1.0f);
    }
);

const GLchar* default_fragmentSource = GLSL(
    in vec3 mColor;
    out vec4 oColor;
    void main() {
        oColor = vec4(mColor, 1.0f);
    }
);

#endif