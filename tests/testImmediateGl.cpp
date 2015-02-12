
#include <memory>
#include <iostream>

#include "../types/immediateGl.hpp"

#include <SDL.h>



int main(){

  if(SDL_Init(SDL_INIT_EVERYTHING) < 0){
	std::cout << "couldn't init SDL" << std::endl;
	return 1;
  }
  
  std::unique_ptr<SDL_Window, void(*)(SDL_Window*)>
	window{SDL_CreateWindow("3.2", SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		800,800,
		SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN),
	  &SDL_DestroyWindow};
  
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3); 
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  auto glContext = SDL_GL_CreateContext(window.get());
  
  SDL_SetWindowSize(window.get(),
					800,
					800);
  glViewport(0, 0, 800, 800);


  benlib::ImmediateGl gl;

  gl.setColor3D(1,0,0);
  gl.addVertex3D(0,0,0);
  gl.setColor3D(0,0,1);
  gl.addVertex3D(0,0,1);
  gl.setColor3D(0,1,0);
  gl.addVertex3D(0,1,0);
  gl.setColor4D(0,0,1,1);
  gl.addVertex3D(1,1,1);

  gl.addPoint(0);
  gl.addLine(1, 2);
  gl.addTriangle(0, 2, 3);
  

  glPointSize(10.0);
  glLineWidth(10.0);


  bool readyToExit = false;
  int frame = 0;


  while(!readyToExit){
	++frame;

	float camera[] = {static_cast<float>(3*cos(frame/50.)), 
					  0, 
					  static_cast<float>(3*sin(frame/50.)),

					  0, 0, 0,
					  0, 1, 0};

	gl.cameraLookAt(camera, camera + 3, camera + 6);
	gl.perspectiveProjection(45., 1., 0.1, 25.);
  
	glClearColor(0,0,0,0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	gl.render();

	glFlush();
	SDL_GL_SwapWindow(window.get());

	SDL_Event event;
	while(SDL_PollEvent(&event)){
	  switch(event.type){
	  case SDL_KEYDOWN:
		readyToExit = true;
	  default:
		; //nothing
	  }
	}
	
  }
  SDL_GL_DeleteContext(glContext);
  return 0;
}
