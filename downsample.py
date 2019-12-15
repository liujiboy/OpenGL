import random
import os
import sys
def parseObj(dir,fileName,sampleRate):
    '''
    解析obj文件
    '''
    f=open(os.path.join(dir,fileName))
    v=[]
    for line in f.readlines():
        if line[0]=="v":
            if random.random()>(1-sampleRate):
                values=line.split()
                v.append([values[1],values[2],values[3]])
    return v
def parsePly(dir,fileName,sampleRate):
    '''
    解析ply文件
    '''
    f=open(os.path.join(dir,fileName))
    v=[]
    beginToParse=False
    for line in f.readlines():
        if not beginToParse and line.startswith("end_header"):
            beginToParse=True
        if beginToParse:
            if random.random()>(1-sampleRate):
                values=line.split()
                v.append([values[0],values[1],values[2]])
    return v
def writeObj(dir,fileName,v):
    '''
    导出obj文件
    '''
    f=open(os.path.join(dir,fileName),"w")
    for i in v:
        f.write("v %s %s %s\n"%(i[0],i[1],i[2]))
    f.close()
def writeOFF(dir,fileName,v):
    '''
    导出OFF文件
    '''
    f=open(os.path.join(dir,fileName),"w")
    f.write("OFF\n")
    f.write("%d 0\n"%(len(v)))
    for i in v:
        f.write("%s %s %s\n"%(i[0],i[1],i[2]))
    f.close()
def writePly(dir,fileName,v):
    '''
    导出ply文件
    '''
    f=open(os.path.join(dir,fileName),"w")
    f.write("ply\n")
    f.write("format ascii 1.0\n")
    f.write("comment VCGLIB generated\n")
    f.write("element vertex "+str(len(v))+"\n") 
    f.write("property float x\n")
    f.write("property float y\n")
    f.write("property float z\n")
    f.write("element face 0\n")
    f.write("property list uchar int vertex_indices\n")
    f.write("end_header\n")
    for i in v:
        f.write("%s %s %s\n"%(i[0],i[1],i[2]))
    f.close()

if len(sys.argv)<3:
    print("使用说明：downsample.py [inputDir] [inputFileName] [sampleRate] [outputDir]")
    print("例如：downsample.py tree1 tree1.obj 0.5")
    print("默认sampleRate等于0.5，可以不输入")
    print("默认outputDir等于inputDir，故此可以不输入")
else:
    inputDir=sys.argv[1]
    inputFileName=sys.argv[2]
    if len(sys.argv)<4:
        sampleRate=0.5
    else:
        sampleRate=float(sys.argv[3])
    if len(sys.argv)<5:
        outputDir=inputDir
    else:
        sampleoutputDirRate=sys.argv[4]
    if inputFileName.endswith(".obj"):
        v=parseObj(inputDir,inputFileName,sampleRate)
    if inputFileName.endswith(".ply"):
        v=parsePly(inputDir,inputFileName,sampleRate)
    outputFileName=inputFileName[:inputFileName.index(".")]+"_"+str(len(v))
    writeObj(outputDir,outputFileName+".obj",v)
    writePly(outputDir,outputFileName+".ply",v)
    writeOFF(outputDir,outputFileName+".off",v)