#version 330 core

layout (location = 0) in vec3 aPosCube;
layout (location = 1) in vec3 aNormalCube;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec3 normal;
out vec3 fragPos;
out vec2 texCoord;

void main()
{
      gl_Position = projection * view * model * vec4(aPosCube, 1.0);

      normal = aNormalCube;
      mat3 normalMatrix = mat3(transpose(inverse(model))); // normal que este acorde con el modelo..

      normal = normalMatrix * aNormalCube;
      fragPos = vec3(model * vec4(aPosCube, 1.0));
      texCoord = aTexCoord;
}
