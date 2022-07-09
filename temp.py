def printJuxta(name, size, step):
    for i in range(12):
        if (i % 3 == 0):
                print("\t\t\t\\makebox[\\textwidth][c]{%")
        print(f"\t\t\t\\subfloat[$t={(i*step/size):.4f}$]" + "{%")
        print(f"\t\t\t\t\\includegraphics[width=0.325\\textwidth]{{images/datasets/{name}/t{i*step}.png}}%")
        print(f"\t\t\t\t\\label{{fig:{name}_t{i*step}}}%")
        print("\t\t\t}%")
        if (i == 11):
            print("\t\t\t}%")
        elif (i % 3 == 2):
            print("\t\t\t}")

def printfigure(title, name, size, step):
    print(f"\t\\section{{{title}}}")
    print("\t\t\\begin{figure}[h]")
    printJuxta(name, size, step)
    print(f"\t\t\\caption{{Time Juxtaposing {title}}}")
    print("\t\t\\end{figure}")

printfigure("Droplet Collision", "droplet", 250, 20)
printfigure("Beating Heart (2D)", "heart2D", 60, 5)
printfigure("Beating Heart (3D)", "heart3D", 64, 5)
printfigure("Expanding Circle", "circle", 120, 10)
printfigure("Expanding Sphere", "sphere", 120, 10)
printfigure("Spinning", "spinning", 64, 5)
printfigure("Sliding Square", "square", 64, 5)
printfigure("Sliding Cube", "cube", 64, 5)
printfigure("Twinkling Star", "star", 64, 5)