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



def rdxf(filename):
    print("Reading file: " + filename)
    dxf = dxfgrabber.readfile(filename)

    total_length = 0

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

            length = None
            ppoint = (0, 0)

            for p in e.points:

                print(p)

                if length is None:
                    length = 0
                    ppoint = p
                    continue

        elif typename == 'LINE':

            print('start point: {}\n'.format(e.start))
            print('end point: {}\n'.format(e.end))
            start = [e.start[0]- 200, e.start[1] - 600]
            end = [e.end[0]- 200, e.end[1] - 600]
            s = Segment( start=start, end=end, net=vo.code)
            segments.append(s)

    
        elif typename == 'INSERT':

           print('block reference: {}'.format(e.insert))
           #print('rows: {}'.format(e.row_count))

        elif typename == 'CIRCLE':

            print('center: {}'.format(e.center))
            print('radius: {}'.format(e.radius))

            layer_pair=('B.Cu', 'F.Cu')

            coords = [e.center[0]- 200, e.center[1] - 600]
            #v1 = Via(at=coords, size=.8, drill=.6, net=vo.code)
            vias.append(Via(at=coords, size=.8, drill=e.radius, net=vo.code))


            #pcb.add_track_via(coords, size =  e.radius)

        elif typename == 'POLYLINE':

            print('Polyline is closed? {}\n'.format(e.is_closed))

            length = None
            ppoint = (0, 0)
            coords = []
            for i in range(len(e.points) - 1 ):
                start = [e.points[i][0]- 200, e.points[i][1] - 600]
                end = [e.points[i+1][0]- 200, e.points[i+1][1] - 600]

                s = Segment( start=start, end=end, net=vo.code)
                segments.append(s)
        

        elif typename == 'ELLIPSE':

            a = euclidean_distance((0, 0), e.majoraxis)
            b = a * e.ratio

            print('center: {}'.format(e.center))
            print('majoraxis: {}'.format(e.majoraxis))
            print('a: {}'.format(a))
            print('b: {}'.format(b))

            total_length += length

        elif typename == 'SPLINE':
            # s = Segment( start=e.start, end=e.end, net=vo.code)
            # segments.append(s)
            pass

        else:

            print('Not Implemented')

        print('=' * 20)

    speed = 4

    print('{} mm / {} mmps = {} s'.format(total_length, speed, total_length / speed))

   
   

if __name__ == '__main__':

    base_folder = "../pcb/FrontGlassPCB/"
    #filename = "vias.dxf"
    filename = "1_lines.dxf"
    path = base_folder + filename
    #pcbnew_easy.test()
    rdxf(path)
    # base_filename = "1_top copper.dxf" 
    # rdxf(base_filename)

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
    #pcb.polygons += polygons
    
    filename = "FrontGlassPCB"
    path =  base_folder + filename
    pcb.to_file(path)
