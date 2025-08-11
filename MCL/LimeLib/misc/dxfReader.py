import sys
import ezdxf
import json

# Python dictionary
data = []

class Ford:
    def __init__(self, model, year, color):
        self.model = model
        self.year = year
        self.color = color
ford_instance = Ford("Mustang", 1964, "red")

print(ford_instance)

try:
    doc = ezdxf.readfile("misc/example.dxf")
    print(f"DXF file '{doc.filename}' loaded successfully.")
    for entity in doc.modelspace():
        if entity.dxftype() == 'LINE':
            data.append({
                'type': entity.dxftype(),
                'startX': entity.dxf.start.x,
                'startY': entity.dxf.start.y,
                'endX': entity.dxf.end.x,
                'endY': entity.dxf.end.y
            })
        elif entity.dxftype() == 'CIRCLE':
            data.append({
                'type': entity.dxftype(),
                'centerX': entity.dxf.center.x,
                'centerY': entity.dxf.center.y,
                'radius': entity.dxf.radius
            })
        elif entity.dxftype() == 'ARC':
            data.append({
                'type': entity.dxftype(),
                'centerX': entity.dxf.center.x,
                'centerY': entity.dxf.center.y,
                'radius': entity.dxf.radius,
                'startAngle': entity.dxf.start_angle,
                'endAngle': entity.dxf.end_angle
            })
except IOError:
    print(f"Not a DXF file or a generic I/O error.")
    sys.exit(1)
except ezdxf.DXFStructureError:
    print(f"Invalid or corrupted DXF file.")
    sys.exit(2)

# Writing JSON to a file
with open('dxfFile.json', 'w') as file:
    json.dump(data, file, indent=4)  # Converts Python dictionary to JSON

