
#include <cassert>
#include <iostream>

#include "immediateGl.hpp"
#include "../algorithms/cAlg.hpp"

namespace benlib{

  void ImmediateGl::addPoint(unsigned int index){
	points.push_back(index);
	dirty = true;
  }
  
  void ImmediateGl::addLine(
	  unsigned int index1, 
	  unsigned int index2){
	lines.push_back(index1);
	lines.push_back(index2);
	dirty = true;
  }
  
  void ImmediateGl::addTriangle(
	  unsigned int index1, 
	  unsigned int index2,
	  unsigned int index3){
	triangles.push_back(index1);
	triangles.push_back(index2);
	triangles.push_back(index3);
	dirty = true;
  }
  
  std::array<float, 4> 
  ImmediateGl::getCurrentColor() const{
	return currentColor;
  }
  
  std::array<float, 4> 
  ImmediateGl::getColor(size_t vertexIndex){
	return {{vertexData[vertexIndex][4],
		  vertexData[vertexIndex][5],
		  vertexData[vertexIndex][6],
		  vertexData[vertexIndex][7]}};
  }


  void ImmediateGl::render(){

	if(dirty){
	  setupBuffers();
	}
	dirty = false;


	glBindVertexArray(vertexArray.value);
	glUseProgram(glslProgram.value);
	
	auto projectionLocation = 
	  glGetUniformLocation(glslProgram.value, "projection");
	auto cameraLocation = 
	  glGetUniformLocation(glslProgram.value, "camera");

	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, projectionMatrix.data());
	glUniformMatrix4fv(cameraLocation, 1, GL_FALSE, cameraMatrix.data());

	const char * const lineOffset = static_cast<const char *>(nullptr) + 
	  triangles.size()*sizeof(decltype(combinedIndices)::value_type);
	const char * const pointOffset = lineOffset +
	  lines.size()*sizeof(decltype(combinedIndices)::value_type);

	glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(triangles.size()), 
		GL_UNSIGNED_INT, nullptr);
	glDrawElements(GL_LINES, static_cast<GLsizei>(lines.size()), 
		GL_UNSIGNED_INT, lineOffset);
	glDrawElements(GL_POINTS, static_cast<GLsizei>(points.size()), 
		GL_UNSIGNED_INT, pointOffset);

	glUseProgram(0);
	glBindVertexArray(0);

  }

  void ImmediateGl::clear(){
	dirty = true;
	vertexData.clear();
	points.clear();
	lines.clear();
	triangles.clear();
  }


  void ImmediateGl::setupBuffers(){
	
	combinedIndices.resize(triangles.size() + lines.size() + points.size());
	using benlib::cAlg::copy;
	copy(points, 
		copy(lines, 
			copy(triangles, combinedIndices.begin())));
	
	glBindVertexArray(vertexArray.value);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer.value);
	glBufferData(GL_ARRAY_BUFFER, 
		static_cast<GLsizeiptr>(vertexData.size()*sizeof(VertexData)),
		vertexData.data(),
		GL_STATIC_DRAW);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer.value);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 
		static_cast<GLsizeiptr>(combinedIndices.size()*
			sizeof(decltype(combinedIndices)::value_type)),
		combinedIndices.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0); //positions
	glEnableVertexAttribArray(1); //colors
	
	//positions
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(VertexData), nullptr);
	//color
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(VertexData), 
		static_cast<const char*>(nullptr) + (4*sizeof(float)));

	glBindVertexArray(0);

  }


  const char vertShaderSource[] =
	"#version 410\n"
	"layout (location = 0) in vec4 position;\n\n"
	"layout (location = 1) in vec4 color;\n"
	"smooth out vec4 smoothColor;\n"
	"uniform mat4 projection, camera;\n"
	"void main() { \n"
	"  gl_Position = projection*camera*position;\n"
	"  smoothColor = color;\n"
	"}\n";

  const char fragShaderSource[] =
	"#version 410\n"
	"smooth in vec4 smoothColor;\n"
	"out vec4 colorOut;\n"
	"void main() { colorOut = smoothColor;}\n";
  
  
  void ImmediateGl::loadShaders(){
	assert(glslProgram.value >= 0);
	VertexShaderResource vs;
	FragmentShaderResource fs;
	
	auto loadAndCheck = [](GLuint shaderId, const char* shaderSource){
	  glShaderSource(shaderId, 1, &shaderSource, nullptr);
	  glCompileShader(shaderId);
	  GLint result = GL_FALSE;
	  glGetShaderiv(shaderId, GL_COMPILE_STATUS, &result);
	  if(!result){
		GLint logLength;
		glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &logLength);
		std::vector<char> errorString(
			static_cast<std::size_t>((logLength > 1) ? logLength : 1));
		glGetShaderInfoLog(shaderId, logLength, nullptr, errorString.data());
		std::cout << errorString.data() << std::endl;
	  }
	};

	loadAndCheck(vs.value, vertShaderSource);
	loadAndCheck(fs.value, fragShaderSource);
	
	glAttachShader(glslProgram.value, vs.value);
	glAttachShader(glslProgram.value, fs.value);
	glLinkProgram(glslProgram.value);
	
	GLint result = GL_FALSE;
	glGetProgramiv(glslProgram.value, GL_LINK_STATUS, &result);
	if(!result){
	  GLint logLength;
	  glGetProgramiv(glslProgram.value, GL_INFO_LOG_LENGTH, &logLength);
	  std::vector<char> linkError(
		  static_cast<std::size_t>((logLength > 1) ? logLength : 1));
	  glGetProgramInfoLog(glslProgram.value, logLength, nullptr, linkError.data());
	  std::cout << linkError.data() << std::endl;
	}
	//shaders auto deleted.  Thanks RAII
	
  }

  void ImmediateGl::cameraSetIdentity(){
	cameraMatrix.fill(0.0f);
	cameraMatrix[0] = 1.0f;
	cameraMatrix[5] = 1.0f;
	cameraMatrix[10] = 1.0f;
	cameraMatrix[15] = 1.0f;
  }
  void ImmediateGl::projectionSetIdentity(){
	projectionMatrix.fill(0.0f);
	projectionMatrix[0] = 1.0f;
	projectionMatrix[5] = 1.0f;
	projectionMatrix[10] = 1.0f;
	projectionMatrix[15] = 1.0f;
  }

  //projection, like GLUOrtho2D
  void ImmediateGl::orthoProjection2D(float left, float right, float bottom, float top){
	projectionMatrix.fill(0.0f);
	
	projectionMatrix[0] = 2/(right - left);  //0,0
	projectionMatrix[12] = (right + left)/(left - right); //0, 3
	
	projectionMatrix[5] = 2/(top - bottom); //1,1	
	projectionMatrix[13] = (top + bottom)/(bottom - top);	//1,3
	
	projectionMatrix[10] = 1.f;
	projectionMatrix[15] = 1.f;
	
  }


  void ImmediateGl::perspectiveProjection(float fovy, float aspect, float zNear, float zFar){
	
	float f = std::tan(M_PI_2*(1 - fovy/180.f));
	float diff = zNear - zFar;
	
	projectionMatrix[0] = f/aspect;
	projectionMatrix[1] = 0.f;
	projectionMatrix[2] = 0.f;
	projectionMatrix[3] = 0.f;
  
	projectionMatrix[4] = 0.f;
	projectionMatrix[5] = f  ;
	projectionMatrix[6] = 0.f;
	projectionMatrix[7] = 0.f;

	projectionMatrix[8] = 0.f;
	projectionMatrix[9] = 0.f;
	projectionMatrix[10] = (zNear + zFar)/diff;
	projectionMatrix[11] = -1.f;

	projectionMatrix[12] = 0.f;
	projectionMatrix[13] = 0.f;
	projectionMatrix[14] = 2*zNear*zFar/diff;
	projectionMatrix[15] = 0.f;
  

  }



}
