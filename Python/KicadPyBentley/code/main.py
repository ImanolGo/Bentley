import os
from pcb_manager import PcbManager
from file_manager import FileManager

DEBUG = False
base_folder = os.path.join('..', 'pcb')

print base_folder

def log(info):
    if DEBUG:
        print(info)


if __name__ == '__main__':

    print "Bentley PCB Exported Started ..."
    pcb = PcbManager(base_folder)
    files = FileManager(base_folder, pcb)
    files.parse()

    print "Bentley PCB Exported Ended ..."
