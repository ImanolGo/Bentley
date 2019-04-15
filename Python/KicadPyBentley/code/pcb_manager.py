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
        self.vi, self.vo, self.gnd = Net('VCC'), Net('VO'), Net('GND')

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
        diameter = 2*radius
        self.vias.append(Via(at=coords, size=diameter*1.5, drill=diameter, net=net.code))

    def addSegment(self, start, end, layer, width = None):
        net = Net()
        self.nets.append(net)
        s = Segment( start=start, end=end, net=net.code, layer = layer, width = width)
        self.segments.append(s)

    def addLine(self, start, end, layer, width=None):
        line = GrLine( start=start, end=end, layer = layer, width = width)
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
        ref = self.addModuleName(name)
        m.set_reference(ref)
        m.set_value("")
        self.modules.append(m)

    def addModuleName(self, name):

        name_ = ""
        lowname = name.lower()
        if "c_" in lowname:
            name_ = "C" + str(len(self.capacitors)+1)
            self.capacitors.append(name)
        elif "r_" in lowname:
            name_ = "R" + str(len(self.resistors)+1)
            self.resistors.append(name)
        elif "cl-" in lowname:
            name_ = "LED" + str(len(self.diodes)+1)
            self.diodes.append(name)
        elif "cut_point" in lowname:
            name_ = "J" + str(len(self.joints)+1)
            self.joints.append(name)
        else:
            name_ = "U" + str(len(self.circuits)+1)
            self.circuits.append(name)

        return name_


    def addZone(self, coords, net_name='GND', layer='F.Cu', clearance=0.3):
        zone = Zone(net_name = net_name, layer = layer, polygon = coords, clearance = clearance, filled_polygon = coords)
        self.zones.append(zone)

        coords,net_name, layer

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
        self.resistors = []
        self.diodes = []
        self.capacitors = []
        self.joints = []
        self.circuits = []

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

   


   
