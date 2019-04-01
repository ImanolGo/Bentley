import sys
sys.path.append("libraries/pykicad")

from pykicad.pcb import *
from pykicad.module import *

class PcbManager:

    def __init__(self, base_folder):
        self.base_folder = base_folder
        self.__initializeAttributes()
        self._setupPcb()
        print "PcbManager::Intitialized"

    def __initializeAttributes(self):
        
        # Define nets
        self.vi, self.vo, self.gnd = Net('VI'), Net('VO'), Net('GND')

        # Create PCB
        self.pcb = Pcb()
        self.clear()
        self.net_classes = []

    def _setupPcb(self):
        
        self.layers = [
            Layer('F.Cu'),
            Layer('Inner1.Cu'),
            Layer('Inner2.Cu'),
            Layer('B.Cu'),
            Layer('Edge.Cuts', type='user')
        ]

        for layer in ['Mask', 'Paste', 'SilkS', 'CrtYd', 'Fab']:
            for side in ['B', 'F']:
                self.layers.append(Layer('%s.%s' % (side, layer), type='user'))

        self.net_classes = [NetClass('default', trace_width=1, nets=['VI', 'VO', 'GND'])]


    def addVia(self, coords, radius):
        net = Net()
        self.nets.append(net)
        self.vias.append(Via(at=coords, size=radius+0.2, drill=radius, net=net.code))

    def addSegment(self, start, end, layer):
        net = Net()
        self.nets.append(net)
        s = Segment( start=start, end=end, net=net.code, layer = layer)
        self.segments.append(s)

    def addLine(self, start, end, layer):
        line = GrLine( start=start, end=end, layer = layer)
        self.lines.append(line)

    def addArc(self, start, end, angle, layer):
        arc = GrArc( start=start, end=end, angle = angle, layer = layer)
        self.arcs.append(arc)

    def addFootprint(self, name, coords, angle, side = 'front', offset = 0.0):
        filename = name + '.kicad_mod'
        footlog_path = os.path.join(self.base_folder, 'footprints', filename)
        m = Module.from_file(footlog_path)
        side = side.strip()
        if side == 'back':
            angle = -angle
            m.flip()
            offset = -90
    
        m.at = coords
        angle += offset
        m.rotate(angle)
        self.modules.append(m)

    def addZone(self, coords, net_name='GND', layer='F.Cu', clearance=0.3):
        zone = Zone(net_name, layer, coords, clearance)
        self.zones.append(zone)

    def addPolygon(self, coords,layer='F.Cu'):
        polygon = GrPolygon(coords, layer)
        self.polygons.append(polygon)

    def clear(self):
        self.vias = []
        self.segments = []
        self.polygons = []
        self.lines = []
        self.modules = []
        self.nets = [self.vi, self.vo, self.gnd]
        self.zones = []
        self.arcs = []
        #self.modules = [Module.from_file(self.base_folder + 'footprints/' 'R_0402.kicad_mod')]
        self.modules = []

    def save(self, filename):
        self.pcb.title = filename
        self.pcb.comment1 = 'Comment 1'
        self.pcb.page_type = [20, 20]
        self.pcb.num_nets = len(self.nets) + 2
        self.pcb.setup = Setup(grid_origin=[0, 0])
        self.pcb.layers = self.layers
        self.pcb.modules = self.modules
        self.pcb.net_classes = self.net_classes
        self.pcb.nets = self.nets
        self.pcb.segments = self.segments
        self.pcb.vias = self.vias
        self.pcb.zones = self.zones
        self.pcb.lines = self.lines
        self.pcb.arcs = self.arcs
        self.pcb.polygons = self.polygons

        path =  self.base_folder + filename
        print("PcbManager:: saving to -> " + path)
        self.pcb.to_file(filename)

   


   
