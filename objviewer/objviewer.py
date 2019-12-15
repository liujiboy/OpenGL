
from OpenGL.GL import *
from OpenGL.GLUT import *
from OpenGL.GLU import *
from objloader import *
import sys
ESCAPE = '\033'

window = 0
loader=None
angleX=0
angleY=0
size=3
wireframe=False
def InitGL(Width, Height):				
	glDepthFunc(GL_LESS)	
	glEnable(GL_DEPTH_TEST)
	glShadeModel(GL_SMOOTH)
	glClearColor (0.0, 0.0, 0.0, 0.0)
        glClearDepth(1.0)
        LightAmbient= [ 0.5, 0.5, 0.5, 1.0 ] 	
        LightDiffuse= [ 1.0, 1.0, 1.0, 1.0 ]		
        LightPosition= [ 0.0, 0.0, 10.0, 1.0 ]
        glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient)	
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse)
	glLightfv(GL_LIGHT1, GL_POSITION,LightPosition)
	#glEnable(GL_LIGHT1)
        #glEnable(GL_LIGHTING)
        glShadeModel(GL_SMOOTH)
        glDepthFunc(GL_LEQUAL)
        glEnable(GL_DEPTH_TEST)
        glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	
        #glEnable(GL_TEXTURE_2D);
	glMatrixMode(GL_PROJECTION)
	glLoadIdentity()
	gluPerspective(45.0, float(Width)/float(Height), 0.1, 100.0)
	glMatrixMode(GL_MODELVIEW)
def ReSizeGLScene(Width, Height):
	if Height == 0:
		Height = 1
	glViewport(0, 0, Width, Height)		
	glMatrixMode(GL_PROJECTION)
	glLoadIdentity()
        #gluLookAt(0,0,5,0,0,0,0,1,0)
	gluPerspective(45.0, float(Width)/float(Height), 0.1, 100.0)
	glMatrixMode(GL_MODELVIEW)
def DrawGLScene():
        global loader
        global size
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
        glLoadIdentity()
        gluLookAt(0,0,5,0,0,0,0,1,0)
        #glBegin(GL_TRIANGLES)                   # Start drawing a 4 sided polygon
        #glVertex3f(1, 0, 0.0)          # Top Left
        #glVertex3f(-1, 0, 0.0)           # Top Right
        #glVertex3f(0, 1, 0.0)          # Bottom Right
        #glEnd()     
        #print len(loader.groups)
        glRotatef(angleX,1,0,0)
        glRotatef(angleY,0,1,0)
        maxsize=max([loader.boundingBox.width,loader.boundingBox.height,loader.boundingBox.depth])
        scale=size/maxsize
        glScalef(scale,scale,scale)
        glTranslatef(-loader.boundingBox.center.x,-loader.boundingBox.center.y,-loader.boundingBox.center.z)
        if wireframe:
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE)
        else:
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL)
        loader.draw()
        glutSwapBuffers()
def keyPressed(*args):
	global window
	global angleX
        global angleY
        global size
        global wireframe
        if args[0] == ESCAPE:
		sys.exit()
        if args[0]=='w':
            angleX+=5
        if args[0]=='s':
            angleX-=5
        if args[0]=='a':
            angleY+=5
        if args[0]=='d':
            angleY-=5
        if args[0]=='1':
            size+=0.5
        if args[0]=='2':
            size-=0.5
        if args[0]=='3':
            wireframe=not wireframe
        glutPostRedisplay()
def main(fileDir,fileName):
	global window
        global loader
	glutInit(sys.argv)
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH)
	glutInitWindowSize(640, 480)
	glutInitWindowPosition(0, 0)
	# Okay, like the C version we retain the window id to use when closing, but for those of you new
	# to Python (like myself), remember this assignment would make the variable local and not global
	# if it weren't for the global declaration at the start of main.
	window = glutCreateWindow("Jeff Molofee's GL Code Tutorial ... NeHe '99")
	glutDisplayFunc(DrawGLScene)
	# Uncomment this line to get full screen.
	#glutFullScreen()
	glutIdleFunc(DrawGLScene)
	glutReshapeFunc(ReSizeGLScene)
	glutKeyboardFunc(keyPressed)
	InitGL(640, 480)
        loader=Loader(fileDir,fileName)
        glutMainLoop()

print "Hit ESC key to quit."
main(sys.argv[1],sys.argv[2])
		
