import dxfgrabber
import csv
from numpy import array
from pykicad.pcb import *
from pykicad.module import *

DEBUG = True

 # Define nets
vi, vo, gnd = Net('VI'), Net('VO'), Net('GND')


# Create PCB
pcb = Pcb()
vias = []
segments = []
polygons = []
lines = []
modules = []
nets = [vi, vo, gnd]
base_folder = "../pcb/FrontGlassPCB/"
footprints_folder = "../pcb/Footprints/"


def readViasLayer(filename, layer):
    
    path  = base_folder + filename
    print("Reading file: " + path)
    dxf = dxfgrabber.readfile(path)

    ########################################
    # Info
    ########################################

    print("DXF version: {}".format(dxf.dxfversion))

    # dict of dxf header vars
    print('header_var_coun: {}'.format(len(dxf.header)))

    # collection of layer definitions
    print('layer_count: {}'.format(len(dxf.layers)))

    # dict like collection of block definitions
    print('block_definition_count'.format(len(dxf.blocks)))

    # list like collection of entities
    print('entity_count: {}'.format(len(dxf.entities)))
    print('')

    ########################################
    # Entities
    ########################################
    
    for e in dxf.entities:

        typename = e.dxftype

        print('DXF Entity: {}\n'.format(typename))
        print('Layer: {}\n'.format(e.layer))

        if typename == 'CIRCLE':

            print('center: {}'.format(e.center))
            print('radius: {}'.format(e.radius))
            coords = [e.center[0], e.center[1]]
            #coords[1] = -coords[1]
            net_ = Net()
            nets.append(net_)
            vias.append(Via(at=coords, size=e.radius+0.2, drill=e.radius, net=net_.code))


def readCopperLayer(filename, layer, net):
    
    path = base_folder + filename
    print("Reading file: " + path)
    dxf = dxfgrabber.readfile(path)

    ########################################
    # Info
    ########################################

    print("DXF version: {}".format(dxf.dxfversion))

    # dict of dxf header vars
    print('header_var_coun: {}'.format(len(dxf.header)))

    # collection of layer definitions
    print('layer_count: {}'.format(len(dxf.layers)))

    # dict like collection of block definitions
    print('block_definition_count'.format(len(dxf.blocks)))

    # list like collection of entities
    print('entity_count: {}'.format(len(dxf.entities)))
    print('')


    ########################################
    # Entities
    ########################################
    
    for e in dxf.entities:

        typename = e.dxftype

        print('=' * 20)
        print('DXF Entity: {}\n'.format(typename))
        print('Layer: {}\n'.format(e.layer))

        if typename == 'LWPOLYLINE':

            print('LWPolyline is closed? {}\n'.format(e.is_closed))

            for i in range(len(e.points) - 1 ):
                start = [e.points[i][0], e.points[i][1]]
                end = [e.points[i+1][0], e.points[i+1][1]]

               #start[1] = -start[1]
 #               end[1] = -end[1]
                print('polyline: {}'.format(e.points[i]))

                net_ = Net()
                nets.append(net_)
                s = Segment( start=start, end=end, net=net_.code, layer = layer)
                segments.append(s)

            if e.is_closed == True:
                index = len(e.points) - 1
                start = [e.points[index][0], e.points[index][1]]
                end = [e.points[0][0], e.points[0][1]]
               #start[1] = -start[1]
 #               end[1] = -end[1]
                print('polyline: {}'.format(e.points[index]))
                net_ = Net()
                nets.append(net_)
                s = Segment( start=start, end=end, net=net_.code, layer = layer)
                segments.append(s)

        elif typename == 'LINE':

            print('start point: {}\n'.format(e.start))
            print('end point: {}\n'.format(e.end))
            start = [e.start[0], e.start[1] ]
            end = [e.end[0], e.end[1]]
            # start[1] = -start[1]
            # end[1] = -end[1]
            net_ = Net()
            nets.append(net_)
            s = Segment( start=start, end=end, net=net_.code, layer = layer)
            segments.append(s)


        elif typename == 'POLYLINE':

            print('Polyline is closed? {}\n'.format(e.is_closed))

            for i in range(len(e.points) - 1 ):
                start = [e.points[i][0], e.points[i][1]]
                end = [e.points[i+1][0], e.points[i+1][1]]

                print('polyline: {}'.format(e.points[i]))
               #start[1] = -start[1]
 #               end[1] = -end[1]
                net_ = Net()
                nets.append(net_)
                s = Segment( start=start, end=end, net=net_.code, layer = layer)
                segments.append(s)

            if e.is_closed == True:
                index = len(e.points) - 1
                start = [e.points[index][0], e.points[index][1]]
                end = [e.points[0][0], e.points[0][1]]
               #start[1] = -start[1]
 #               end[1] = -end[1]
                print('polyline: {}'.format(e.points[index]))
                net_ = Net()
                nets.append(net_)
                s = Segment( start=start, end=end, net=net_.code, layer = layer)
                segments.append(s)

        elif typename == 'SPLINE':
            print('length: {}'.format(len(e.fit_points)))
            for i in range(len(e.fit_points) - 1 ):
                start = [e.fit_points[i][0], e.fit_points[i][1]]
                end = [e.fit_points[i+1][0], e.fit_points[i+1][1]]
               #start[1] = -start[1]
 #               end[1] = -end[1]
                print('spline: {}'.format(e.fit_points[i]))
                net_ = Net()
                nets.append(net_)
                s = Segment( start=start, end=end, net=net_.code, layer = layer)
                segments.append(s)   
   
def readOutlineLayer(filename, layer):
    
    path = base_folder + filename
    print("Reading file: " + path)
    dxf = dxfgrabber.readfile(path)

    ########################################
    # Info
    ########################################

    print("DXF version: {}".format(dxf.dxfversion))

    # dict of dxf header vars
    print('header_var_coun: {}'.format(len(dxf.header)))

    # collection of layer definitions
    print('layer_count: {}'.format(len(dxf.layers)))

    # dict like collection of block definitions
    print('block_definition_count'.format(len(dxf.blocks)))

    # list like collection of entities
    print('entity_count: {}'.format(len(dxf.entities)))
    print('')


    ########################################
    # Entities
    ########################################
    
    for e in dxf.entities:

        typename = e.dxftype

        print('=' * 20)
        print('DXF Entity: {}\n'.format(typename))
        print('Layer: {}\n'.format(e.layer))

        if typename == 'LWPOLYLINE':

            print('LWPolyline is closed? {}\n'.format(e.is_closed))

            for i in range(len(e.points) - 1 ):
                start = [e.points[i][0], e.points[i][1]]
                end = [e.points[i+1][0], e.points[i+1][1]]
               #start[1] = -start[1]
 #               end[1] = -end[1]

                print('polyline: {}'.format(e.points[i]))

                l = GrLine( start=start, end=end, layer = layer)
                lines.append(l)

            if e.is_closed == True:
                index = len(e.points) - 1
                start = [e.points[index][0], e.points[index][1]]
                end = [e.points[0][0], e.points[0][1]]
               #start[1] = -start[1]
 #               end[1] = -end[1]
                print('polyline: {}'.format(e.points[index]))
                l = GrLine( start=start, end=end, layer = layer)
                lines.append(l)

        elif typename == 'LINE':

            print('start point: {}\n'.format(e.start))
            print('end point: {}\n'.format(e.end))
            start = [e.start[0], e.start[1] ]
            end = [e.end[0], e.end[1]]
            # start[1] = -start[1]
            # end[1] = -end[1]
            line = GrLine( start=start, end=end, layer = layer)
            lines.append(l)


        elif typename == 'POLYLINE':

            print('Polyline is closed? {}\n'.format(e.is_closed))

            for i in range(len(e.points) - 1 ):
                start = [e.points[i][0], e.points[i][1]]
                end = [e.points[i+1][0], e.points[i+1][1]]
               #start[1] = -start[1]
 #               end[1] = -end[1]
                print('polyline: {}'.format(e.points[i]))

                l = GrLine( start=start, end=end, layer = layer)
                lines.append(l)

            if e.is_closed == True:
                index = len(e.points) - 1
                start = [e.points[index][0], e.points[index][1]]
                end = [e.points[0][0], e.points[0][1]]
               #start[1] = -start[1]
 #               end[1] = -end[1]
                print('polyline: {}'.format(e.points[index]))
                l = GrLine( start=start, end=end, layer = layer)
                lines.append(l)

        elif typename == 'SPLINE':
            print('length: {}'.format(len(e.fit_points)))
            for i in range(len(e.fit_points) - 1 ):
                start = [e.fit_points[i][0], e.fit_points[i][1]]
                end = [e.fit_points[i+1][0], e.fit_points[i+1][1]]
               #start[1] = -start[1]
 #               end[1] = -end[1]
                print('spline: {}'.format(e.fit_points[i]))
                l = GrLine( start=start, end=end, layer = layer)
                lines.append(l)  
   
def readEntity(filename):
    
    path = base_folder + filename
    print("Reading file: " + path)
    dxf = dxfgrabber.readfile(path)

    ########################################
    # Info
    ########################################

    print("DXF version: {}".format(dxf.dxfversion))

    # dict of dxf header vars
    print('header_var_coun: {}'.format(len(dxf.header)))

    # collection of layer definitions
    print('layer_count: {}'.format(len(dxf.layers)))

    # dict like collection of block definitions
    print('block_definition_count'.format(len(dxf.blocks)))

    # list like collection of entities
    print('entity_count: {}'.format(len(dxf.entities)))
    print('')

    for e in dxf.entities:

        typename = e.dxftype
        print('DXF Entity: {}\n'.format(typename))

def readFootprints(filename, side = 'Front'):

    path = base_folder + filename
    print("Reading Footprint File: " + path)
    with open(path) as csvfile:
        readCSV = csv.reader(csvfile, delimiter=',')
        for row in readCSV:
            print(row)
            footprint_path = footprints_folder + row[0] + '.kicad_mod'
            print("Reading Footprint: " + footprint_path)
            m = Module.from_file(footprint_path)
            if side == 'Back':
                m.flip()

            m.at = [float(row[1]),float(row[2])]
            #m.at = [float(row[1]),-float(row[2])]
            angle = float(row[3]) 
            m.rotate(-30)
            modules.append(m)
            # print(row[0])
            # print(row[0],row[1],row[2],)


if __name__ == '__main__':

   

    filename = "vias.dxf"
    readViasLayer(filename, 'F.Cu')

    filename = "1_lines.dxf"
    #readEntity(filename)
    readCopperLayer(filename, 'F.Cu', "")

    filename = "16_lines.dxf"
    readCopperLayer(filename, 'B.Cu', "")

    filename = "20_board_outline.dxf"
    readOutlineLayer(filename, 'Edge.Cuts')

    # filename = "1_outlines.dxf"
    # path = base_folder + filename
    # readOutlineLayer(path, 'F.Paste')

    # filename = "16_outlines.dxf"
    # path = base_folder + filename
    # readOutlineLayer(path, 'B.Paste')
    # 
    filename = "RGE0024H.csv"
    readFootprints(filename, 'Back', -90)

     # Create zones
    coords = [(0, 0), (10, 0), (10, 10), (0, 10)]
    gndplane_top = Zone(net_name='GND', layer='F.Cu', polygon=coords, clearance=0.3)


    layers = [
        Layer('F.Cu'),
        Layer('Inner1.Cu'),
        Layer('Inner2.Cu'),
        Layer('B.Cu'),
        Layer('Edge.Cuts', type='user')
    ]

    for layer in ['Mask', 'Paste', 'SilkS', 'CrtYd', 'Fab']:
        for side in ['B', 'F']:
            layers.append(Layer('%s.%s' % (side, layer), type='user'))

    nc1 = NetClass('default', trace_width=1, nets=['VI', 'VO', 'GND'])



    
    r1 = Module.from_file(footprints_folder + 'R_0402.kicad_mod')
    r1.at = [0, 0]
    c1 = Module.from_file(footprints_folder + 'QFN-24-1EP_4x4mm_Pitch0.5mm.kicad_mod')
    c1.at = [10, 0]
    m1 = Module.from_file(footprints_folder + 'C_0603.kicad_mod')
    m1.at = [20, 0]
    m2 = Module.from_file(footprints_folder + 'CL-Z891.kicad_mod')
    m2.at = [30, 0]
    m3 = Module.from_file(footprints_folder + 'Texas_RGE0024H_EP2.7x2.7mm.kicad_mod')
    m3.at = [40, 0]
    m4 = Module.from_file(footprints_folder + 'CL-341S_SQ2.kicad_mod')
    m4.at = [50, 0]

    
    if len(modules) == 0:
        modules = [r1, c1, m1, m2, m3, m4]
    # r1 = Module('M1')
    # s1 = Segment( start=[-100000,-100000], end=[-100000,-100000], net=vo.code)


    pcb.title = 'A title'
    pcb.comment1 = 'Comment 1'
    pcb.page_type = [20, 20]
    pcb.num_nets = len(nets) + 2
    pcb.setup = Setup(grid_origin=[10, 10])
    pcb.layers = layers
    pcb.modules += modules
    pcb.net_classes += [nc1]
    pcb.nets += nets
    pcb.segments += segments
    pcb.vias += vias
    pcb.zones += [gndplane_top]
    pcb.lines += lines
    #pcb.polygons += polygons
    
    filename = "FrontGlassPCB"
    path =  base_folder + filename
    pcb.to_file(path)
