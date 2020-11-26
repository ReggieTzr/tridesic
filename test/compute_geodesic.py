# 3p
import numpy as np
from tridesic import get_heat_geodesics


def read_off(file):
    file = open(file, "r")
    if file.readline().strip() != "OFF":
        raise "Not a valid OFF header"

    n_verts, n_faces, n_dontknow = tuple([int(s) for s in file.readline().strip().split(" ")])
    verts = [[float(s) for s in file.readline().strip().split(" ")] for i_vert in range(n_verts)]
    faces = [[int(s) for s in file.readline().strip().split(" ")][1:] for i_face in range(n_faces)]

    return np.array(verts), np.array(faces)


def main():
    verts, faces = read_off("012.off")

    geodesic_dist = get_heat_geodesics(verts, faces)
    print(geodesic_dist.shape)
    print(geodesic_dist[2])


if __name__ == "__main__":
    main()
