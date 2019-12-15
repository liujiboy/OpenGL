#coding=utf-8
import os
import Image
from OpenGL.GL import *
class Textures:
    __textures={}
    @staticmethod
    def getTexture(name):
        texture=Textures.__textures.get(name)
        if texture!=None:
            return texture
        image = Image.open(name)
        ix = image.size[0]
        iy = image.size[1]
        image = image.convert("RGBA").tostring("raw", "RGBA")
        texture=glGenTextures(1)
        Textures.__textures[name]=texture
        glBindTexture(GL_TEXTURE_2D, texture)   
        glPixelStorei(GL_UNPACK_ALIGNMENT,1)
        glTexImage2D(GL_TEXTURE_2D, 0, 3, ix, iy, 0, GL_RGBA, GL_UNSIGNED_BYTE, image)
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP)
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP)
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT)
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT)
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST)
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST)
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL)
        return texture
class Vec3:
    def __init__(self,x=0,y=0,z=0):
        self.x=x
        self.y=y
        self.z=z
class BoundingBox:
    def __init__(self,vertices):
        v=vertices[0]
        minv=Vec3(v.x,v.y,v.z)
        maxv=Vec3(v.x,v.y,v.z)
	for v in vertices:
            if v.x>maxv.x:
                maxv.x=v.x
            if v.x<minv.x:
                minv.x=v.x
            if v.y>maxv.y:
                maxv.y=v.y
            if v.y<minv.y:
                minv.y=v.y
            if v.z>maxv.z:
                maxv.z=v.z
            if v.z<minv.z:
                minv.z=v.z
        self.maxv=maxv
        self.minv=minv
        self.width=maxv.x-minv.x
        self.height=maxv.y-minv.y
        self.depth=maxv.z-minv.z
        self.center=Vec3((maxv.x+minv.x)/2,(maxv.y+minv.y)/2,(maxv.z+minv.z)/2)
#class Face:
    #def __init__(self,v,vt,vn):
        #self.v=v
        #self.vt=vt
#        self.vn=vn
class Material:
    def __init__(self,name="",ka=Vec3(0.2,0.2,0.2),kd=Vec3(0.8,0.8,0.8),\
            ke=Vec3(0,0,0.1),ks=Vec3(1,1,1),ns=64,mapKd=None):
        self.name=name
        self.ka=ka
        self.kd=kd
        self.ke=ke
        self.ks=ks
        self.mapKd=mapKd
class Group:
    def __init__(self,name,material,v,vt,vn):
        self.name=name
        self.material=material
        self.v=v
        self.vt=vt
        self.vn=vn
        self.boundingBox=BoundingBox(v)
    def draw(self):
        glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,[self.material.ka.x,self.material.ka.y,self.material.ka.z,1.0])
        glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,[self.material.kd.x,self.material.kd.y,self.material.kd.z,1.0])
        glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,[self.material.ke.x,self.material.ke.y,self.material.ke.z,1.0])
        glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,[self.material.ks.x,self.material.ks.y,self.material.ks.z,1.0])
        glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,self.material.ns)
        glColor4f(self.material.kd.x,self.material.kd.y,self.material.kd.z,1.0)
        if self.material.mapKd!=None:
            glEnable(GL_TEXTURE_2D)
            texture=Textures.getTexture(self.material.mapKd)
            glBindTexture(GL_TEXTURE_2D,texture)
        else:
            glDisable(GL_TEXTURE_2D)
        glBegin(GL_TRIANGLES)
        for i in range(len(self.v)):
            if self.material.mapKd!=None:
                glTexCoord2f(self.vt[i].x,self.vt[i].y)
            glNormal3f(self.vn[i].x,self.vn[i].y,self.vn[i].z)
            glVertex3f(self.v[i].x,self.v[i].y,self.v[i].z)
            #print "v",i,self.v[i].x,self.v[i].y,self.v[i].z
            #raw_input("press any key to continue...")
        glEnd()
class Loader:
    def __init__(self,fileDir,fileName,flipZY=False):
        self.vertices=[]
        self.normals=[]
        self.texCoords=[]
        self.matLib={}
        self.groups=[]
        file=open(fileDir+os.sep+fileName,"r")
        material=None
        group_name=None
        v=None
        vt=None
        vn=None
        for line in file:
            line=line.strip()
            if line.startswith("mtllib"):
                line=line[7:]
                self.__load_mtllib(fileDir,line)
            elif line.startswith("v"):
                if line.startswith("vn"):
                    line=line[3:].strip()
                    values=line.split(" ")
                    if flipZY:
                        self.normals.append(Vec3(float(values[0]),float(values[2]),float(values[1])))
                    else:
                        self.normals.append(Vec3(float(values[0]),float(values[1]),float(values[2])))
                elif line.startswith("vt"):
                    line=line[3:].strip()
                    values=line.split(" ")
                    if flipZY:
                        self.texCoords.append(Vec3(float(values[0]),float(values[2]),float(values[1])))
                    else:
                        self.texCoords.append(Vec3(float(values[0]),float(values[1]),float(values[2])))
                else:
                    line=line[2:].strip()
                    values=line.split(" ")
                    if flipZY:
                        self.vertices.append(Vec3(float(values[0]),float(values[2]),float(values[1])))
                    else:
                        self.vertices.append(Vec3(float(values[0]),float(values[1]),float(values[2])))
            elif line.startswith("g"):
                line=line[2:].strip()
                #group.name=line
                group_name=line
            elif line.startswith("f"):
                line=line[2:]
                values=line.split(" ")
                indices=[]
                for value in values:
                    indices.extend(value.split("/"))
                value_from_index=lambda list,index: index!="" and list[int(index)-1] or None
                for i in range(0,len(indices),3):
                    v.append(value_from_index(self.vertices,indices[i]))
                    vt.append(value_from_index(self.texCoords,indices[i+1]))
                    vn.append(value_from_index(self.normals,indices[i+2]))
            elif line.startswith("usemtl"):
                if material!=None:
                    group=Group(group_name,material,v,vt,vn)
                    self.groups.append(group)
                line=line[7:].strip()
                material=self.matLib.get(line)
                v=[]
                vt=[]
                vn=[]
        if material!=None:
            group=Group(group_name,material,v,vt,vn)
            self.groups.append(group)
        self.boundingBox=BoundingBox(self.vertices)
    def __load_mtllib(self,fileDir,fileName):
        file=open(fileDir+os.sep+fileName)
        material=None
        for line in file:
            line=line.strip()
            if line.startswith("newmtl"):
                if material!=None:
                    self.matLib[material.name]=material;
                material=Material()
                line=line[7:].strip()
                material.name=line
            elif line.startswith("Ka"):
                line=line[3:].strip()
                values=line.split(" ")
                material.ka.x=float(values[0])
                material.ka.y=float(values[1])
                material.ka.z=float(values[2])
            elif line.startswith("Kd"):
                line=line[3:].strip()
                values=line.split(" ")
                material.kd.x=float(values[0])
                material.kd.y=float(values[1])
                material.kd.z=float(values[2])
            elif line.startswith("Ks"):
                line=line[3:].strip()
                values=line.split(" ")
                material.ks.x=float(values[0])
                material.ks.y=float(values[1])
                material.ks.z=float(values[2])
            elif line.startswith("Ke"):
                line=line[3:].strip()
                values=line.split(" ")
                material.ke.x=float(values[0])
                material.ke.y=float(values[1])
                material.ke.z=float(values[2])
            elif line.startswith("Ns"):
                line=line[3:].strip()
                material.ns=float(line)
            elif line.startswith("map_Kd"):
                line=line[7:].strip()
                material.mapKd=(fileDir+os.sep+line).replace("\\",os.sep)
        if material!=None:
            self.matLib[material.name]=material;
    def __atoi(self,str):
        try:
            return int(str)
        except ValueError:
            return 0
    def draw(self):
        for group in self.groups:
            group.draw()
if __name__=="__main__":
    l=Loader("Obj/Apple","Apple.obj")
    for group in l.groups:
        print "group name %s"%(group.name)
        print "group material name %s"%(group.material.name)
        for face in group.faces:
            for i in range(0,2):
                print "f:(%f,%f,%f),(%f,%f,%f),(%f,%f,%f)"%(face.v[i].x,face.v[i].y,\
                        face.v[i].z,face.vt[i].x,face.vt[i].y,face.vt[i].z,face.vn[i].x,\
                        face.vn[i].y,face.vn[i].z)
