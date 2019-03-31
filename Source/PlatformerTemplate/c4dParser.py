import c4d
from c4d import gui
#Welcome to the world of Python


def main():
    doc = c4d.documents.GetActiveDocument()
    docPath = doc.GetDocumentPath()#GetDocumentData(33).GetString()
    print(docPath)
    
    f = open(docPath + "\\test1", "w")

    obj = doc.GetActiveObjects(0)[0]
    print("Nb points: ", obj.GetPointCount())
    print("Nb polygons: ", obj.GetPolygonCount())
    selection = obj.GetPointS() #Selected points = hole

    #Transform the selection to a list of points
    pointsOut = []
    pointsIn = []

    for index, selected in enumerate(selection.GetAll(obj.GetPointCount())):
        if selected:
            pointsIn.append(obj.GetPoint(index))
        else:
            pointsOut.append(obj.GetPoint(index))

    #Get polygons
    polygons = obj.GetAllPolygons()

    #Fill the file
    f.write("// Outside vertices\n%s\n" % len(pointsOut))
    for point in pointsOut:
        coord = str(point.x) + " " + str(point.y)
        f.write("%s\n" % coord)

    f.write("// Inside vertices\n%s\n" % len(pointsIn))
    for point in pointsIn:
        coord = str(point.x) + " " + str(point.y)
        f.write("%s\n" % coord)

    f.write("// Polygons\n%s\n" % len(polygons))
    for poly in polygons:
        if not poly.IsTriangle():
            gui.MessageDialog("Don't forget to triangulate!")
        indexes = str(poly.a) + " " + str(poly.b) + " " + str(poly.c)
        f.write("%s\n" % indexes)

    f.close()
    print("Done")

if __name__=='__main__':
    main()