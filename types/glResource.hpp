#pragma once

#include "../helpers/glHeaders.hpp"

namespace benlib{

  template <typename AllocFunc, typename DeallocFunc>
  class GLResource{
  public:
	GLuint value;
	bool needsDealloc;
	GLResource(){
	  AllocFunc{}(&value);
	  needsDealloc = true;
	}
	~GLResource(){
	  if(needsDealloc){
		DeallocFunc{}(&value);
	  }
	}
	
	//move only
	GLResource(const GLResource& ) = delete;
	GLResource& operator=(const GLResource&) = delete;
	
	GLResource(GLResource&& other)
	  :value{other.value}, needsDealloc{true}
	{
	  other.needsDealloc = false;
	  other.value = -1; //safety?
	}
	
	GLResource& operator =(GLResource&& other){
	  value = other.value;
	  needsDealloc = true;
	  other.needsDealloc = false;
	  other.value = -1;
	}
  };

  struct AllocVertShader{
	void operator()(GLuint* val){ *val = glCreateShader(GL_VERTEX_SHADER);}
  };
  struct AllocFragShader{
	void operator()(GLuint* val){ *val = glCreateShader(GL_FRAGMENT_SHADER);}
  };
  struct DeallocShader{
	void operator()(GLuint* val){ glDeleteShader(*val);}
  };
  struct AllocBuffer{
	void operator()(GLuint* val){ glGenBuffers(1, val);}
  };
  struct DeallocBuffer{
	void operator()(GLuint* val){ glDeleteBuffers(1, val);}
  };
  struct AllocVertexArrays{
	void operator()(GLuint* val){ glGenVertexArrays(1, val);}
  };
  struct DeallocVertexArrays{
	void operator()(GLuint* val){ glDeleteVertexArrays(1, val);}
  };
  struct AllocGlslProgram{
	void operator()(GLuint* val){ *val = glCreateProgram();}
  };
  struct DeallocGlslProgram{
	void operator()(GLuint* val){ glDeleteProgram(*val);}
  };
  
  using VertexShaderResource = GLResource<AllocVertShader, DeallocShader>;
  using FragmentShaderResource = GLResource<AllocFragShader, DeallocShader>;
  using GlslProgramResource = GLResource<AllocGlslProgram, DeallocGlslProgram>;
  using BufferResource = GLResource<AllocBuffer, DeallocBuffer>;
  using VertexArrayResource = GLResource<AllocVertexArrays, DeallocVertexArrays>;

}

