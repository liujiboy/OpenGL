#! /usr/bin/env python
'''
Quaint little Python/OpenGL/Shader example
 
Uses the x-ray shader found in MeshLab
 
Code is a slightly modified version of http://www.pygame.org/wiki/GLSLExample
'''
import OpenGL 
OpenGL.ERROR_ON_COPY = True 
from OpenGL.GL import *
from OpenGL.GLU import *
from OpenGL.GLUT import *
import numpy as np
from matplotlib.pyplot import imsave
# PyOpenGL 3.0.1 introduces this convenience module...
from OpenGL.GL.shaders import *
 
import time, sys
program = None
global falloffValue
global rotY
 
# A general OpenGL initialization function.  Sets all of the initial parameters. 
def InitGL(Width, Height):                # We call this right after our OpenGL window is created.
    # glClearColor(0.0, 0.0, 0.0, 0.0)    # This Will Clear The Background Color To Black
    # glClearDepth(1.0)                    # Enables Clearing Of The Depth Buffer
    # glShadeModel(GL_SMOOTH)                # Enables Smooth Color Shading
 
    # glMatrixMode(GL_PROJECTION)
    # glLoadIdentity()                    # Reset The Projection Matrix
    #                                     # Calculate The Aspect Ratio Of The Window
    # #gluPerspective(45.0, float(Width)/float(Height), 0.1, 100.0)
 
    # glMatrixMode(GL_MODELVIEW)
	pass
 
 
# The function called when our window is resized (which shouldn't happen if you enable fullscreen, below)
def ReSizeGLScene(Width, Height):
    # if Height == 0:                        # Prevent A Divide By Zero If The Window Is Too Small 
    #     Height = 1
 
    # glViewport(0, 0, Width, Height)        # Reset The Current Viewport And Perspective Transformation
    # glMatrixMode(GL_PROJECTION)
    # glLoadIdentity()
    # #gluPerspective(45.0, float(Width)/float(Height), 0.1, 100.0)
	# #gluo
    # glMatrixMode(GL_MODELVIEW)
	pass
 
# The main drawing function. 
def DrawGLScene():
	
	width=2000
	height=2000
	glViewport(0, 0, width, height) 
	glMatrixMode(GL_PROJECTION)
	glLoadIdentity() 
	glClearColor(0.0, 0.0, 0.0,1.0)    
	glClearDepth(1.0)                    
	glShadeModel(GL_SMOOTH)                
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
	                   
	glMatrixMode(GL_MODELVIEW)
	
	glLoadIdentity()                    # Reset The View 
 
 
	# Move Left 1.5 units and into the screen 6.0 units.
	glTranslatef(0, 0.0, 1.0)
 
	# Spin this business.
	#glRotatef(rotY,0.0,1.0,0.0)
 
 
	# Enable blending and disable depth masking (x-ray shader only applies the opacity falloff.
	glEnable(GL_BLEND)
	glDepthMask(GL_FALSE)
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)
	#glLineWidth(10)
	# Load something with some curves
	
	# glColor4d(0.0,1.0,0.0,0.5)
	# glBegin(GL_QUADS)
	# glVertex3f(-0.5,0.5,-1.5)
	# glVertex3f(0.0,0.5,-1.5)
	# glVertex3f(0.0,0.0,-1.5)
	# glVertex3f(-0.5,0.0,-1.5)
	# glEnd()
	# glColor4d(1.0,0.0,0.0,0.5)
	# glBegin(GL_QUADS)
	# glVertex3f(-0.7,0.5,-1.5)
	# glVertex3f(0.0,0.5,-1.5)
	# glVertex3f(0.0,0.0,-1.5)
	# glVertex3f(-0.5,0.0,-1.5)
	# glEnd()
	glPointSize(10)
	glColor4d(1.0,0.0,0.0,1.0)
	glBegin(GL_POINTS)
	glVertex3d(0,0,0)
	glVertex3d(0.5,0,0)
	glEnd()
	#glReadBuffer(GL_BACK)
	glPixelStorei(GL_PACK_ALIGNMENT, 1)
	pixels=np.zeros(width*height*4,dtype=np.uint8)
	glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels)
	pixels=pixels.reshape((height,width,4))
	imsave("a.jpeg",pixels)
	# Swap buffers
	glutSwapBuffers()
 
	#rotY += 1.0
 
def mod_falloff(val):
	global falloffValue
	if program:
		edgefalloff = glGetUniformLocation(program, "edgefalloff")
		if not edgefalloff in (None,-1):
			falloffValue = falloffValue + val
			glUniform1f(edgefalloff,falloffValue)
 
# The function called whenever a key is pressed. Note the use of Python tuples to pass in: (key, x, y)  
def keyPressed(*args):
 
	# If escape is pressed, kill everything.
	print(args)
	if args[0] == b'\x1b':
		sys.exit()
	elif args[0] == b'c':
		print("Decreasing falloff")
		mod_falloff(1.0)
	elif args[0] == b'x':
		print("Increasing falloff")
		mod_falloff(-1.0)
 
def main():
	global window
	global falloffValue
	global rotY
 
	# For now we just pass glutInit one empty argument. I wasn't sure what should or could be passed in (tuple, list, ...)
	# Once I find out the right stuff based on reading the PyOpenGL source, I'll address this.
	glutInit(sys.argv)
 
	# Select type of Display mode:   
	#  Double buffer 
	#  RGBA color
	# Alpha components supported 
	# Depth buffer
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH)
 
	# get a 640 x 480 window 
	glutInitWindowSize(2000, 2000)
 
	# the window starts at the upper left corner of the screen 
	glutInitWindowPosition(0, 0)
 
	# Okay, like the C version we retain the window id to use when closing, but for those of you new
	# to Python (like myself), remember this assignment would make the variable local and not global
	# if it weren't for the global declaration at the start of main.
	window = glutCreateWindow("Jeff Molofee's GL Code Tutorial ... NeHe '99")
 
	# Register the drawing function with glut, BUT in Python land, at least using PyOpenGL, we need to
	# set the function pointer and invoke a function to actually register the callback, otherwise it
	# would be very much like the C version of the code.    

	glutDisplayFunc(DrawGLScene)
 
	# Uncomment this line to get full screen.
	#glutFullScreen()
 
	# When we are doing nothing, redraw the scene.
	glutIdleFunc(DrawGLScene)
 
	# Register the function called when our window is resized.
	glutReshapeFunc(ReSizeGLScene)
 
	# Register the function called when the keyboard is pressed.  
	glutKeyboardFunc(keyPressed)
 
	# Initialize our window. 
	InitGL(2000, 2000)
 
	#Start using our program
	#glUseProgram(program)
 
	#Set defaults
	#falloffValue = 1.0
	#rotY = 0.0
 
	#Trigger a fall off modify to update the shader
	#mod_falloff(0.0)
 
	#glEnable (GL_BLEND)
	#glBlendFunc (GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA)
 
	# Start Event Processing Engine    
	glutMainLoop()
 
# Print message to console, and kick off the main to get it rolling.
 
if __name__ == "__main__":
	print("Hit ESC key to quit.")
	print("x - increase shader falloff")
	print("c - decrease shader falloff")
	main()
