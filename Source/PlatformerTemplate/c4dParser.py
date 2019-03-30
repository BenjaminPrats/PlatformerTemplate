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
    selection = obj.GetPointS() #Selected points = hole

    #Transform the selection to a list of points
    pointsOut = []
    pointsIn = []
    for index, selected in enumerate(selection.GetAll(obj.GetPointCount())):
        if selected:
            pointsIn.append(obj.GetPoint(index))
        else:
            pointsOut.append(obj.GetPoint(index))

    #Fill the file as describes by cgal
    f.write("%s\n" % len(pointsOut))
    for point in pointsOut:
        coord = str(point.x) + " " + str(point.y)
        f.write("%s\n" % coord)

    f.write("1\n%s\n" % len(pointsIn))
    for point in pointsIn:
        coord = str(point.x) + " " + str(point.y)
        f.write("%s\n" % coord)

    f.close()
    gui.MessageDialog('Done!')

if __name__=='__main__':
    main()