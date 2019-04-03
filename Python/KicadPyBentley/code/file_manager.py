import sys
sys.path.append("libraries/dxfgrabber")

import dxfgrabber
import os
import math
import csv

class FileManager:

    def __init__(self, base_folder, pcbManager):
        self.pcb = pcbManager
        self.base_folder = os.path.join(base_folder, 'pcbs')
        self.__initializeAttributes()
        print "FileManager::Intitialized"

    def __initializeAttributes(self):
        pass

    def parse(self):
        
        subdirs = [x[0] for x in os.walk(self.base_folder)]
        #subdirnames = [x[1] for x in os.walk(self.base_folder)]
        print(subdirs)
        #print(subdirnames)
        for i in range(1, len(subdirs)):
            subdir = subdirs[i]
            print "FileManager::Reading folder -> " + subdir
            self.readDxfFiles(subdir)
            self.readCsvFiles(subdir)
            self.savePcb(subdir)

    def savePcb(self, directory):
        #filename = directory.split('/')[-1] 
        filename = os.path.basename(directory) + '.kicad_pcb'
        print "FileManager::savePcb -> filename: " + filename 
        path = os.path.join(directory, filename)
       # path = directory + '/' + filename + '.kicad_pcb'
        self.pcb.save(path)
        self.pcb.clear()

    def readDxfFiles(self, directory):

        files = []   
        files += [each for each in os.listdir(directory) if each.endswith('.dxf')]
        for file in files:
            path = os.path.join(directory, file)
            self.parseDfx(path)

    def readCsvFiles(self, directory):

        files = []   
        files += [each for each in os.listdir(directory) if each.endswith(('.csv','.CSV'))]
        for file in files:
            path = os.path.join(directory, file)
            self.parseCsv(path)


    def parseCsv(self, path):
        print "FileManager::parseCsv -> " + path
        with open(path) as csvfile:
            readCSV = csv.reader(csvfile, delimiter=',')
            for row in readCSV:
                name = row[1].lstrip(' ')
                coords = [float(row[2]),-float(row[3])]
                angle = float(row[4])
                side = row[5].lower()
                side = side.strip()
                #print "FileManager::parseCsv -> side " + side
                self.pcb.addFootprint(name,coords, angle, side)

    def parseDfx(self, path):
        print "FileManager::parseDfx -> " + path
        dxf = dxfgrabber.readfile(path)

        for e in dxf.entities:

            typename = e.dxftype
            #print('FileManager::parseDfx -> Layer: {}\n'.format(e.layer))

            viasStr = "vias"
            edgeStr = "edge_cuts"
            fcu = "f_cu"
            bcu = "b_cu"
            name = "name"
            bcu_plus = "b_cu_plus"
            bcu_minus = "b_cu_minus"
            fcu_plus = "f_cu_plus"
            fcu_minus = "f_cu_minus"

            layer = e.layer.lower()
            #print "layer : " + layer

            if name in layer:
                pass

            elif viasStr in layer:
                self.parseVia(e)

            elif edgeStr in layer:
                self.parseEdge(e)

            elif bcu_plus in layer:
                self.parseZone(e, 'VI', 'B.Cu')

            elif bcu_minus in layer:
                self.parseZone(e, 'GND', 'B.Cu')

            elif fcu_plus in layer:
                self.parseZone(e, 'VI', 'F.Cu')

            elif fcu_minus in layer:
                self.parseZone(e, 'GND', 'F.Cu')

            elif fcu in layer:
                width = self.getTrackWidth(layer)
                self.parseTrack(e,'F.Cu', width)

            elif bcu in layer:
                width = self.getTrackWidth(layer)
                self.parseTrack(e,'B.Cu', width)

    def getTrackWidth(self, layer_name):
        lines = layer_name.split('_')
        size = len(lines) 
        if size < 4:
            return None

        dec_str = lines[size-1]
        dec_mult = 0.01
        if len(dec_str) == 1:
            dec_mult = 0.1

        unit_val =  float(lines[size-2])
        number = unit_val + float(dec_str)*dec_mult
        #print number
        return number

    def parseVia(self, entity):

        typename = entity.dxftype
        #print('FileManager::parseVia ->  type :' + typename)
        if typename == 'CIRCLE':

            coords = [entity.center[0], entity.center[1]]
            coords[1] = -coords[1]
            coords[0] = coords[0]*entity.extrusion[2]
            self.pcb.addVia(coords, entity.radius)

            #print('FileManager::parseDfx -> Layer: {}\n'.format(e.layer))
            #
            #print('FileManager::parseVia ->  coords: {}\n'.format(coords))
            #print('FileManager::parseVia ->  extrusion: {}\n'.format(entity.extrusion[2]))


    def parseZone(self, entity, net_name, layer):

        #print('FileManager::parseZone -> added polyline -> ' + entity.dxftype)
        typename = entity.dxftype
        if typename == 'LWPOLYLINE' or typename == 'POLYLINE':
            #self.pcb.addZone(entity.points,net_name, layer)
            coords = []
            for coord in entity.points:
                c = [coord[0], -coord[1]]
                coords.append(c)
            #self.pcb.addPolygon(coords,layer)
            self.pcb.addZone(coords,net_name, layer)

    def parseTrack(self, entity, layer, width = None):

        typename = entity.dxftype
    
       #print('FileManager::parseTrack -> added segment -> layer: ' + entity.layer + ', type = ' + typename + ', thickness = ' + str(width))

        if typename == 'LINE':

            start = [entity.start[0], entity.start[1] ]
            end = [entity.end[0], entity.end[1]]
            start[1] = -start[1]
            end[1] = -end[1]
            self.pcb.addSegment(start, end, layer, width)

        elif typename == 'LWPOLYLINE' or typename == 'POLYLINE':

            #width = entity.width

            for i in range(len(entity.points) - 1 ):
                start = [entity.points[i][0], entity.points[i][1]]
                end = [entity.points[i+1][0], entity.points[i+1][1]]
                start[1] = -start[1]
                end[1] = -end[1]
            
                self.pcb.addSegment(start, end, layer, width)

            if entity.is_closed == True:
                index = len(entity.points) - 1
                start = [entity.points[index][0], entity.points[index][1]]
                end = [entity.points[0][0], entity.points[0][1]]
                start[1] = -start[1]
                end[1] = -end[1]
                self.pcb.addSegment(start, end, layer, width)

    def parseEdge(self, entity):

        typename = entity.dxftype
        layer = 'Edge.Cuts'

        #print('FileManager::parseEdge -> added line -> ' + entity.dxftype)

        if typename == 'LWPOLYLINE' or typename == 'POLYLINE':

            for i in range(len(entity.points) - 1 ):
                start = [entity.points[i][0], entity.points[i][1]]
                end = [entity.points[i+1][0], entity.points[i+1][1]]
                start[1] = -start[1]
                end[1] = -end[1]

                self.pcb.addLine(start, end, layer)

            if entity.is_closed == True:
                index = len(entity.points) - 1
                start = [entity.points[index][0], entity.points[index][1]]
                end = [entity.points[0][0], entity.points[0][1]]
                start[1] = -start[1]
                end[1] = -end[1]
                self.pcb.addLine(start, end, layer)

        elif typename == 'LINE':

            start = [entity.start[0], entity.start[1] ]
            end = [entity.end[0], entity.end[1]]
            start[1] = -start[1]
            end[1] = -end[1]
            self.pcb.addLine(start, end, layer)

        elif typename == 'SPLINE':
            for i in range(len(entity.fit_points) - 1 ):
                start = [entity.fit_points[i][0], entity.fit_points[i][1]]
                end = [entity.fit_points[i+1][0], entity.fit_points[i+1][1]]
                start[1] = -start[1]
                end[1] = -end[1]
                self.pcb.addLine(start, end, layer) 

        elif typename == 'ARC':
            # print('FileManager::parseEdge -> start arc angle -> ' + str(entity.start_angle))
            # print('FileManager::parseEdge -> end arc angle -> ' + str(entity.end_angle))        
            start = [0.0,0.0]
            start[0] = entity.center[0] + entity.radius*math.cos(math.radians(entity.start_angle))
            start[1] = entity.center[1] + entity.radius*math.sin(math.radians(entity.start_angle))

            end = [0.0,0.0]
            end[0] = entity.center[0] + entity.radius*math.cos(math.radians(entity.end_angle))
            end[1] = entity.center[1] + entity.radius*math.sin(math.radians(entity.end_angle))

            angle = entity.end_angle - entity.start_angle

            self.pcb.addArc(start, end, angle, layer) 



    
        

    