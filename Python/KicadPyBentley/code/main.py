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

def log(info):
    if DEBUG:
        print(info)

def euclidean_distance(point1, point2):
    total = 0

    for count, (pos1, pos2) in enumerate(zip(point1, point2)):
        total += (pos2 - pos1)**2

    count += 1
    total = total ** (1/count)

    return total


def ellipse(a, b):
    h = ((a-b)**2) / ((a+b)**2)
    result = 3.14159265358979323846 * (a+b) * (1 + 3 * h / (10 + (4 - 3 * h) ** 0.5))
    return result

def rdxf(filename):
    log("Reading file: " + filename)
    dxf = dxfgrabber.readfile(filename)

    total_length = 0

    ########################################
    # Info
    ########################################

    log("DXF version: {}".format(dxf.dxfversion))

    # dict of dxf header vars
    log('header_var_coun: {}'.format(len(dxf.header)))

    # collection of layer definitions
    log('layer_count: {}'.format(len(dxf.layers)))

    # dict like collection of block definitions
    log('block_definition_count'.format(len(dxf.blocks)))

    # list like collection of entities
    log('entity_count: {}'.format(len(dxf.entities)))
    log('')

   


    ########################################
    # Entities
    ########################################
    
    for e in dxf.entities:

        typename = e.dxftype

        log('=' * 20)
        log('DXF Entity: {}\n'.format(typename))
        log('Layer: {}\n'.format(e.layer))

        if typename == 'LWPOLYLINE':

            print('LWPolyline is closed? {}\n'.format(e.is_closed))

            length = None
            ppoint = (0, 0)

            for p in e.points:

                log(p)

                if length is None:
                    length = 0
                    ppoint = p
                    continue

                length += euclidean_distance(ppoint, p)
                ppoint = p

            log('Length = {}\n'.format(length))

            total_length += length

        elif typename == 'LINE':

            log('start point: {}\n'.format(e.start))
            log('end point: {}\n'.format(e.end))
            start = [e.start[0]- 200, e.start[1] - 600]
            end = [e.end[0]- 200, e.end[1] - 600]

            length = euclidean_distance(e.start, e.end)

            log('Length = {}\n'.format(length))
            s = Segment( start=start, end=end, net=vo.code)
            segments.append(s)

            total_length += length

        elif typename == 'INSERT':

           log('block reference: {}'.format(e.insert))
           #log('rows: {}'.format(e.row_count))

        elif typename == 'CIRCLE':

            log('center: {}'.format(e.center))
            log('radius: {}'.format(e.radius))

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

            log('center: {}'.format(e.center))
            log('majoraxis: {}'.format(e.majoraxis))
            log('a: {}'.format(a))
            log('b: {}'.format(b))

            length = ellipse(a, b)

            log('distance: {}'.format(length))

            total_length += length

        elif typename == 'SPLINE':
            # s = Segment( start=e.start, end=e.end, net=vo.code)
            # segments.append(s)
            pass

        else:

            print('Not Implemented')

        log('=' * 20)

    speed = 4

    log('{} mm / {} mmps = {} s'.format(total_length, speed, total_length / speed))

   
   

if __name__ == '__main__':

    base_filename = "18_holes.dxf"
    #pcbnew_easy.test()
    rdxf(base_filename)
    base_filename = "1_top copper.dxf" 
    rdxf(base_filename)

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

    s1 = Segment( start=[-100000,-100000], end=[-100000,-100000], net=vo.code)


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
    pcb.to_file('ccc')
