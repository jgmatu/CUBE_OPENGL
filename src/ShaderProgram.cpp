#include "ShaderProgram.hpp"

ShaderProgram::ShaderProgram() {
      programId = glCreateProgram();

      if (programId == 0) {
            std::cerr << "ShaderProgram failed" << std::endl;
      }
      vertexShaderId = 0;
      fragmentShaderId = 0;
}

ShaderProgram::~ShaderProgram() {
      ;
}

void
ShaderProgram::createVertexShader(const std::string& sc) {
      vertexShaderId = createShader(sc, GL_VERTEX_SHADER);
}

void
ShaderProgram::createFragmentShader(const std::string& sc) {
      vertexShaderId = createShader(sc, GL_FRAGMENT_SHADER);
}

void
ShaderProgram::link() {
      int params;
      char inflog[1024];
      int size;

      glLinkProgram(programId);
      glGetProgramiv(programId, GL_LINK_STATUS, &params);
      if (!params) {
            glGetProgramInfoLog(programId, 1024, &size, inflog);
            std::cerr << "Error linking Shader code: " << inflog << std::endl;
            throw;
      }

      if (vertexShaderId != 0) {
            glDetachShader(programId, vertexShaderId);
      }

      if (fragmentShaderId != 0) {
            glDetachShader(programId, fragmentShaderId);
      }

      glValidateProgram(programId);
      glGetProgramiv(programId, GL_LINK_STATUS, &params);

      if (!params) {
            glGetProgramInfoLog(programId, 1024, &size, inflog);
            std::cerr << "Error linking Shader code: " << inflog << std::endl;
            throw;
      }
}

void
ShaderProgram::bind() { glUseProgram(programId); }

void
ShaderProgram::unbind() { glUseProgram(0); }

void
ShaderProgram::cleanup() {
      unbind();
      if (programId != 0) {
            glDeleteProgram(programId);
      }
}

void
ShaderProgram::createUniform(std::string uniformName) {
      int uniformLocation = glGetUniformLocation(programId, uniformName.c_str());

      if (uniformLocation < 0) {
            std::cerr << "Could not find uniform: " << uniformName  <<
            std::endl <<  "Value : " << uniformLocation << std::endl;
            throw;
      }
      uniforms.insert(std::pair<std::string, int>(uniformName, uniformLocation));
}

void
ShaderProgram::setUniform(std::string name, glm::vec3 value) {
      glUniform3f(uniforms[name], value.x, value.y, value.z);
}

void
ShaderProgram::setUniform(std::string name, glm::mat4 value) {
      glUniformMatrix4fv(uniforms[name], 1, GL_FALSE,  glm::value_ptr(value));
}

void
ShaderProgram::setUniform(std::string name, int value) {
      glUniform1i(uniforms[name], value);
}


int
ShaderProgram::createShader(const std::string& sc, int shaderType) {
      int params;
      const char* _sc = sc.c_str();
      int size = sc.size();

      int shaderId = glCreateShader(shaderType);
      if (shaderId == 0) {
            std::cerr << "Error creating shader. Type: " << shaderType << '\n';
      }
      glShaderSource(shaderId, 1, &_sc, &size);
      glCompileShader(shaderId);

      int InfoLogLength;

      glGetShaderiv(shaderId, GL_COMPILE_STATUS, &params);
      glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &InfoLogLength);

      if (InfoLogLength > 0) {
            std::vector<char> VertexShaderErrorMessage(InfoLogLength+1);
            glGetShaderInfoLog(shaderId   , InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		fprintf(stderr, "%s\n", &VertexShaderErrorMessage[0]);
      }
      glAttachShader(programId, shaderId);
      return shaderId;
}
