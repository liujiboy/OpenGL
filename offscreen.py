#! /usr/bin/env python

import OpenGL 
OpenGL.ERROR_ON_COPY = True 
from OpenGL.GL import *
from OpenGL.GLU import *
from OpenGL.GLUT import *
import numpy as np
from matplotlib.pyplot import imsave
from OpenGL.GL.shaders import *
def loadPointCloud(fileName):
    v=[]
    f=open(fileName)
    for line in f.readlines():
        if line.startswith("v"):
            values=line.split()
            v.append([float(values[1]),float(values[2]),float(values[3])])
    v=np.array(v)
    xmin=v[:,0].min()
    xmax=v[:,0].max()
    ymin=v[:,1].min()
    ymax=v[:,1].max()
    zmin=v[:,2].min()
    zmax=v[:,2].max()
    scale=1/max([xmax-xmin,ymax-ymin,zmax-zmin])
    center=((xmax+xmin)/2,(ymax+ymin)/2,(zmax+zmin)/2)
    return np.array(v),scale,center
def loadSkeleton(fileName):
    v=[]
    l=[]
    f=open(fileName)
    for line in f.readlines():
        if line.startswith("v"):
            values=line.split()
            v.append([float(values[1]),float(values[2]),float(values[3])])
        if line.startswith("l"):
            values=line.split()
            aIdx=int(values[1])
            bIdx=int(values[2])
            l.append((v[aIdx-1],v[bIdx-1]))
    return l
class OpenGLScene():
    def __init__(self,width,height,angle=50,near=0.1,far=1000):
        self.width=width
        self.height=height
        self.angle=angle
        self.near=near
        self.far=far
    def initGL(self):
        glClearColor(0.0, 0.0, 0.0, 0.0)    
        glClearDepth(1.0)                    
        glShadeModel(GL_SMOOTH)               
    def resize(self,width,height):
        self.width=width
        self.height=height
        glViewport(0, 0, self.width, self.height) 
        self.__setupProjectionMatrix()
        gluLookAt(0,0,1,0,0,0,0,1,0)
    def __setupProjectionMatrix(self):
        glMatrixMode(GL_PROJECTION)
        glLoadIdentity()
        gluPerspective(self.angle, float(self.width)/float(self.height), self.near, self.far)
    def drawPointClouds(self,scale,center,fileName,angle=0,vertices=[],lines=[]):
        self.resize(self.width,self.height)
        glClearColor(1.0, 1.0, 1.0,1.0)
        glClearDepth(1.0)                    
        glShadeModel(GL_SMOOTH)                
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)  
        glEnable(GL_BLEND)
        glDepthMask(GL_FALSE)
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)                 
        glMatrixMode(GL_MODELVIEW)
        glLoadIdentity()
        glRotated(angle,0,1,0)                    
        glScaled(scale,scale,scale)
        glTranslatef(-center[0], -center[1], -center[2])
        if len(vertices)!=0:
            glPointSize(1)
            glColor4d(0.1,0.1,0.1,0.8)
            glBegin(GL_POINTS)
            for v in vertices:
                glVertex3d(v[0],v[1],v[2])
            glEnd()
        if len(lines)!=0:
            glLineWidth(1)
            glColor4d(1,0,0,1.0)
            glBegin(GL_LINES)
            for l in lines:
                glVertex3d(l[0][0],l[0][1],l[0][2])
                glVertex3d(l[1][0],l[1][1],l[1][2])
            glEnd()
        glPixelStorei(GL_PACK_ALIGNMENT, 1)
        pixels=np.zeros(self.width*self.height*4,dtype=np.uint8)
        glReadPixels(0, 0, self.width, self.height, GL_RGBA, GL_UNSIGNED_BYTE, pixels)
        #glGetError()
        pixels=pixels.reshape((self.height,self.width,4))
        pixels=pixels[::-1,:,:]
        imsave(fileName,pixels)
        glutSwapBuffers()

def main():
    vertices,scale,center=loadPointCloud("/Users/liuji/Desktop/experiment/L-System/tree5/tree5_11433.obj")
    lines=loadSkeleton("/Users/liuji/Desktop/experiment/L-System/tree5/tree5_11433.obj.obj")
    width=400
    height=400
    glutInit(sys.argv)
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH)
    glutInitWindowSize(width, height)
    glutInitWindowPosition(0, 0)
    window = glutCreateWindow("offscreen")
    scene=OpenGLScene(width,height)
    scene.initGL()
    for i in range(8):
        scene.drawPointClouds(scale,center,"/Users/liuji/Desktop/experiment/L-System/tree5/result/"+str(i)+".png",angle=45*i,vertices=vertices,lines=lines)
    lines=loadSkeleton("/Users/liuji/Desktop/experiment/L-System/tree5/tree5_skeleton.obj")
    for i in range(8):
        scene.drawPointClouds(scale,center,"/Users/liuji/Desktop/experiment/L-System/tree5/GT/"+str(i)+".png",angle=45*i,vertices=vertices,lines=lines)
    #glutDisplayFunc(lambda: scene.drawPointClouds(vertices,scale,center,"1.png"))
    #glutIdleFunc(lambda:scene.drawPointClouds(vertices,scale,center,"1.png"))
    #glutReshapeFunc(lambda width,height:scene.resize(width,height))
    #glutMainLoop()
    #vertices,scale,center=loadPointCloud("/Users/liuji/Desktop/experiment/realdata/tree1/different number of vertices/tree1_6678.obj")
    #DrawGLScene(width,height,vertices,scale,center,"1.png")
    #DrawGLScene(width,height,"2.jpeg")
    #glutMainLoop()
if __name__ == "__main__":
    main()
