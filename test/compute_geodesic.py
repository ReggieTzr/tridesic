# stdlib
from time import time
# 3p
import numpy as np
import scipy.io as sio
from tridesic import get_heat_geodesics, get_fmm_geodesics, get_exact_geodesics


def read_off(file):
    file = open(file, "r")
    if file.readline().strip() != "OFF":
        raise "Not a valid OFF header"

    n_verts, n_faces, n_dontknow = tuple([int(s) for s in file.readline().strip().split(" ")])
    verts = [[float(s) for s in file.readline().strip().split(" ")] for i_vert in range(n_verts)]
    faces = [[int(s) for s in file.readline().strip().split(" ")][1:] for i_face in range(n_faces)]

    return np.array(verts), np.array(faces)


def main():
    mesh = "001"
    verts, faces = read_off(mesh + ".off")

    since = time()
    geodesic_dist = get_heat_geodesics(verts, faces)
    print(f"\nThe heat method took: {time() - since:.1f}s")
    sio.savemat(f"{mesh}_heat.mat", {"geod_dist": geodesic_dist})
    print(f"Geodesic matrix shape: {geodesic_dist.shape}")
    print(f"Geodesic distance from vertex 2: {geodesic_dist[2]}")

    since = time()
    geodesic_dist = get_fmm_geodesics(verts, faces)
    print(f"\nFast Marching Method took: {time() - since:.1f}s")
    sio.savemat(f"{mesh}_fmm.mat", {"geod_dist": geodesic_dist})
    print(f"Geodesic matrix shape: {geodesic_dist.shape}")
    print(f"Geodesic distance from vertex 2: {geodesic_dist[2]}")

    since = time()
    geodesic_dist = get_exact_geodesics(verts, faces)
    print(f"\nExact Geodesic Method took: {time() - since:.1f}s")
    sio.savemat(f"{mesh}_exact.mat", {"geod_dist": geodesic_dist})
    print(f"Geodesic matrix shape: {geodesic_dist.shape}")
    print(f"Geodesic distance from vertex 2: {geodesic_dist[2]}")


if __name__ == "__main__":
    main()
