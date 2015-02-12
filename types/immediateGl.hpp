#pragma once

/*
  simulate immediate mode openGL on modern opengl.
  //add vertices and primitives like you're using immediate mode
  //and this class sets up vertex buffers/arrays and shaders

  There's only one set of matrices (model, projection, camera)
  rather than a stack like there would be in openGL classic

*/

#include <vector>
#include <array>
#include <cmath>

#include "../helpers/glHeaders.hpp"
#include "glResource.hpp"

namespace benlib{
  class ImmediateGl{
  public:

	ImmediateGl(){ loadShaders(); }

	//uncopyable
	ImmediateGl(const ImmediateGl& other) = delete;
	ImmediateGl& operator=(const ImmediateGl& other) = delete;

	
	ImmediateGl(ImmediateGl&& other) = default;
	ImmediateGl& operator=(ImmediateGl&& other) = default;


	using VertexData = std::array<float, 8>; //x,y,z,w,r,g,b,a

	void render();  //draw this stuff to the current opengl buffer


	//Add Vertex operations

	//T should have operator [] and at least 3 elements
	//returns the index of the new vertex
	template<typename T>
	unsigned int addVertex3D(const T& vertex);

	template<typename T>
	unsigned int addVertex4D(const T& vertex);
  
	template<typename T>
	unsigned int addVertex3D(T x, T y, T z);
  
	template<typename T>
	unsigned int addVertex4D(T x, T y, T z, T w);


	//primitives
	void addPoint(unsigned int index);
	void addLine(unsigned int index1, unsigned int index2);
	void addTriangle(unsigned int index1, unsigned int index2, unsigned int index3);
  
	//pass in the geometry, not indices
	// T must have operator [] of appropriate size
	template <typename T>
	void addPoint3D(const T& vertex);
	template <typename T>
	void addPoint4D(const T& vertex);
  
	template<typename T, typename U>
	void addLine3D(const T& v1, const U& v2);
	template<typename T, typename U>
	void addLine4D(const T& v1, const U& v2);

	template<typename T, typename U, typename W>
	void addTriangle3D(const T& v1, const U& v2, const W& v3);
	template<typename T, typename U, typename W>
	void addTriangle4D(const T& v1, const U& v2, const W& v3);


	//color state
	//T has operator [] and appropriate dimension
	template <typename T>
	void setColor3D(const T& col);
	template <typename T>
	void setColor4D(const T& col);
	template <typename T, typename U, typename V>
	void setColor3D(T r, U g, V b);
	template <typename T, typename U, typename V, typename W>
	void setColor4D(T r, U g, V b, W a); 


	std::array<float, 4> getCurrentColor() const;
	std::array<float, 4> getColor(size_t vertexIndex);
  
	//camera state
	//just like gluLookAt
	//args should have operator [] and length >= 3
	template <typename T, typename U, typename W>
	void cameraLookAt(const T& eye,
		const U& center,
		const W& up);

	//projection matrix
	//just like GluPerspective
	template <typename T>
	void perspectiveProjection(T fovy, T aspect, T zNear, T zFar);
  
  

  private:

	void setupBuffers();

	std::array<float, 4> currentColor; 
	std::vector<VertexData> vertexData;
	std::vector<unsigned int> points;
	std::vector<unsigned int> lines;
	std::vector<unsigned int> triangles;
	std::vector<unsigned int> combinedIndices; //for one buffer
  
	std::array<float, 16> cameraMatrix, projectionMatrix;

	bool dirty; //need to update buffers, etc?

	VertexShaderResource vertShader;
	FragmentShaderResource fragShader;
	GlslProgramResource glslProgram;
	BufferResource vertexBuffer, indexBuffer;
	VertexArrayResource vertexArray;
	

	void loadShaders();

  };


  //template/inline function defs

  //add vertices

  template <typename T>
  unsigned int ImmediateGl::addVertex3D(const T& vertex){
	return addVertex3D<float>(vertex[0], vertex[1], vertex[2]);
  }

  template <typename T>
  unsigned int ImmediateGl::addVertex4D(const T& vertex){
	return addVertex4D<float>(vertex[0], vertex[1], vertex[2], vertex[3]);
  }

  template <typename T>
  unsigned int ImmediateGl::addVertex3D(T x, T y, T z){
	return addVertex4D<float>(x, y, z, 1.f);
  }

  template <typename T>
  unsigned int ImmediateGl::addVertex4D(T x, T y, T z, T w){
	vertexData.push_back({{x, y, z, w, 
			  currentColor[0], currentColor[1], currentColor[2], currentColor[3]}});
	dirty = true;
	return static_cast<unsigned int>(vertexData.size() - 1);
  }



  //primitives

  //points (add vertex AND primitives)
  template <typename T>
  void ImmediateGl::addPoint3D(const T& vertex){
	points.push_back(addVertex3D(vertex));
  }

  template <typename T>
  void ImmediateGl::addPoint4D(const T& vertex){
	points.push_back(addVertex4D(vertex));
  }

  //line
  template <typename T, typename U>
  void ImmediateGl::addLine3D(
	  const T& v1, 
	  const U& v2){
	lines.push_back(addVertex3D(v1));
	lines.push_back(addVertex3D(v2));
  }

  template <typename T, typename U>
  void ImmediateGl::addLine4D(
	  const T& v1,
	  const U& v2){
	lines.push_back(addVertex4D(v1));
	lines.push_back(addVertex4D(v2));
  }

  //triangle
  template <typename T, typename U, typename W>
  void ImmediateGl::addTriangle3D(
	  const T& v1, 
	  const U& v2,
	  const W& v3){
	triangles.push_back(addVertex3D(v1));
	triangles.push_back(addVertex3D(v2));
	triangles.push_back(addVertex3D(v3));
  }

  template <typename T, typename U, typename W>
  void ImmediateGl::addTriangle4D(
	  const T& v1,
	  const U& v2,
	  const W& v3){
	triangles.push_back(addVertex4D(v1));
	triangles.push_back(addVertex4D(v2));
	triangles.push_back(addVertex4D(v3));
  }

  //color state
  //T has operator [] and appropriate dimension
  template <typename T>
  void ImmediateGl::setColor3D(const T& col){
	setColor4D(col[0], col[1], col[2], 1);
  }

  template <typename T>
  void ImmediateGl::setColor4D(const T& col){
	setColor4D(col[0], col[1], col[2], col[3]);
  }

  template <typename T, typename U, typename V>
  void ImmediateGl::setColor3D(T r, U g, V b){
	setColor4D(r, g, b, 1);
  }
  
  template <typename T, typename U, typename V, typename W>
  void ImmediateGl::setColor4D(T r, U g, V b, W a){
	currentColor[0] = r;
	currentColor[1] = g;
	currentColor[2] = b;
	currentColor[3] = a;
  }




  //matrix setup

  template <typename T, typename U, typename W>
  void ImmediateGl::cameraLookAt(
	  const T& eye,
	  const U& center,
	  const W& up){
  
	//avoid eigen deps here even though it would shorten the code significantly
  
	std::array<float, 3> f {{
	  center[0] - eye[0],
		center[1] - eye[1],
		center[2] - eye[2]}};
  
	//normalize f
	float fNorm = std::sqrt(f[0]*f[0] + f[1]*f[1] + f[2]*f[2]);
	f[0] /= fNorm;
	f[1] /= fNorm;
	f[2] /= fNorm;

	//s = f cross up
	std::array<float, 3> s{{
		f[1]*up[2] - f[2]*up[1],
		  f[2]*up[0] - f[0]*up[2],
		  f[0]*up[1] - f[1]*up[0]
		  }};
	
	//normalize s
	float sNorm = std::sqrt(s[0]*s[0] + s[1]*s[1] + s[2]*s[2]);
	s[0] /= sNorm;
	s[1] /= sNorm;
	s[2] /= sNorm;

	/*//col 1 = s
	cameraMatrix[0] = s[0] ;
	cameraMatrix[1] = s[1] ;
	cameraMatrix[2] = s[2] ;
	cameraMatrix[3] = 0.f ;
	//col 2 = s cross f
	cameraMatrix[4] = s[1]*f[2] - s[2]*f[1];
	cameraMatrix[5] = s[2]*f[0] - s[0]*f[2];
	cameraMatrix[6] = s[0]*f[1] - s[1]*f[0];
	cameraMatrix[7] = 0.f;
	//col 3 = -f
	cameraMatrix[8] = -f[0];
	cameraMatrix[9] = -f[1];
	cameraMatrix[10] = -f[2];
	cameraMatrix[11] = 0;
	*/
	//row 1 = s
	cameraMatrix[0] = s[0] ;
	cameraMatrix[4] = s[1] ;
	cameraMatrix[8] = s[2] ;
	cameraMatrix[12] = 0.f ;
	//row 2 = s cross f
	cameraMatrix[1] = s[1]*f[2] - s[2]*f[1];
	cameraMatrix[5] = s[2]*f[0] - s[0]*f[2];
	cameraMatrix[9] = s[0]*f[1] - s[1]*f[0];
	cameraMatrix[13] = 0.f;
	//row 3 = -f
	cameraMatrix[2] = -f[0];
	cameraMatrix[6] = -f[1];
	cameraMatrix[10] = -f[2];
	cameraMatrix[14] = 0;

	//col 4 = -eye translation
	cameraMatrix[12] = -eye[0]*cameraMatrix[0] - eye[1]*cameraMatrix[4] - eye[2]*cameraMatrix[8];
	cameraMatrix[13] = -eye[0]*cameraMatrix[1] - eye[1]*cameraMatrix[5] - eye[2]*cameraMatrix[9];
	cameraMatrix[14] = -eye[0]*cameraMatrix[2] - eye[1]*cameraMatrix[6] - eye[2]*cameraMatrix[10];
	cameraMatrix[15] = 1.f;

  }



  template <typename T>
  void ImmediateGl::perspectiveProjection(T fovy, T aspect, T zNear, T zFar){
  
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
