import dxfgrabber
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


def readViasLayer(filename, layer):
    print("Reading file: " + filename)
    dxf = dxfgrabber.readfile(filename)

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
            vias.append(Via(at=coords, size=e.radius+0.2, drill=e.radius, net=vo.code))


def readCopperLayer(filename, layer):
    print("Reading file: " + filename)
    dxf = dxfgrabber.readfile(filename)

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

                print('polyline: {}'.format(e.points[i]))

                s = Segment( start=start, end=end, net=vo.code, layer = layer)
                segments.append(s)

            if e.is_closed == True:
                index = len(e.points) - 1
                start = [e.points[index][0], e.points[index][1]]
                end = [e.points[0][0], e.points[0][1]]
                print('polyline: {}'.format(e.points[index]))
                s = Segment( start=start, end=end, net=vo.code, layer = layer)
                segments.append(s)

        elif typename == 'LINE':

            print('start point: {}\n'.format(e.start))
            print('end point: {}\n'.format(e.end))
            start = [e.start[0], e.start[1] ]
            end = [e.end[0], e.end[1]]
            s = Segment( start=start, end=end, net=vo.code, layer = layer)
            segments.append(s)


        elif typename == 'POLYLINE':

            print('Polyline is closed? {}\n'.format(e.is_closed))

            for i in range(len(e.points) - 1 ):
                start = [e.points[i][0], e.points[i][1]]
                end = [e.points[i+1][0], e.points[i+1][1]]

                print('polyline: {}'.format(e.points[i]))

                s = Segment( start=start, end=end, net=vo.code, layer = layer)
                segments.append(s)

            if e.is_closed == True:
                index = len(e.points) - 1
                start = [e.points[index][0], e.points[index][1]]
                end = [e.points[0][0], e.points[0][1]]
                print('polyline: {}'.format(e.points[index]))
                s = Segment( start=start, end=end, net=vo.code, layer = layer)
                segments.append(s)

        elif typename == 'SPLINE':
            print('length: {}'.format(len(e.fit_points)))
            for i in range(len(e.fit_points) - 1 ):
                start = [e.fit_points[i][0], e.fit_points[i][1]]
                end = [e.fit_points[i+1][0], e.fit_points[i+1][1]]
                print('spline: {}'.format(e.fit_points[i]))
                s = Segment( start=start, end=end, net=vo.code, layer = layer)
                segments.append(s)   
   
def readOutlineLayer(filename, layer):
    print("Reading file: " + filename)
    dxf = dxfgrabber.readfile(filename)

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

                print('polyline: {}'.format(e.points[i]))

                l = GrLine( start=start, end=end, layer = layer)
                lines.append(l)

            if e.is_closed == True:
                index = len(e.points) - 1
                start = [e.points[index][0], e.points[index][1]]
                end = [e.points[0][0], e.points[0][1]]
                print('polyline: {}'.format(e.points[index]))
                l = GrLine( start=start, end=end, layer = layer)
                lines.append(l)

        elif typename == 'LINE':

            print('start point: {}\n'.format(e.start))
            print('end point: {}\n'.format(e.end))
            start = [e.start[0], e.start[1] ]
            end = [e.end[0], e.end[1]]
            line = GrLine( start=start, end=end, layer = layer)
            lines.append(l)


        elif typename == 'POLYLINE':

            print('Polyline is closed? {}\n'.format(e.is_closed))

            for i in range(len(e.points) - 1 ):
                start = [e.points[i][0], e.points[i][1]]
                end = [e.points[i+1][0], e.points[i+1][1]]

                print('polyline: {}'.format(e.points[i]))

                l = GrLine( start=start, end=end, layer = layer)
                lines.append(l)

            if e.is_closed == True:
                index = len(e.points) - 1
                start = [e.points[index][0], e.points[index][1]]
                end = [e.points[0][0], e.points[0][1]]
                print('polyline: {}'.format(e.points[index]))
                l = GrLine( start=start, end=end, layer = layer)
                lines.append(l)

        elif typename == 'SPLINE':
            print('length: {}'.format(len(e.fit_points)))
            for i in range(len(e.fit_points) - 1 ):
                start = [e.fit_points[i][0], e.fit_points[i][1]]
                end = [e.fit_points[i+1][0], e.fit_points[i+1][1]]
                print('spline: {}'.format(e.fit_points[i]))
                l = GrLine( start=start, end=end, layer = layer)
                lines.append(l)  
   


if __name__ == '__main__':

    base_folder = "../pcb/FrontGlassPCB/"

    filename = "vias.dxf"
    path = base_folder + filename
    readViasLayer(path, 'F.Cu')

    filename = "1_lines.dxf"
    path = base_folder + filename
    readCopperLayer(path, 'F.Cu')

    filename = "16_lines.dxf"
    path = base_folder + filename
    readCopperLayer(path, 'B.Cu')

    filename = "20_board_outline.dxf"
    path = base_folder + filename
    readOutlineLayer(path, 'Edge.Cuts')

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

    r1 = Module.from_library('Resistor_SMD', 'R_0815_2038Metric')
    r1.at = [0, 0]
    
    # r1 = Module('M1')
    # s1 = Segment( start=[-100000,-100000], end=[-100000,-100000], net=vo.code)


    pcb.title = 'A title'
    pcb.comment1 = 'Comment 1'
    pcb.page_type = [20, 20]
    pcb.num_nets = 5
    pcb.setup = Setup(grid_origin=[10, 10])
    pcb.layers = layers
    pcb.modules += [r1]
    pcb.net_classes += [nc1]
    pcb.nets += [vi, vo, gnd]
    pcb.segments += segments
    pcb.vias += vias
    pcb.zones += [gndplane_top]
    pcb.lines += lines
    #pcb.polygons += polygons
    
    filename = "FrontGlassPCB"
    path =  base_folder + filename
    pcb.to_file(path)
