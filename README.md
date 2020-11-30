# tridesic
Multiple geodesic distance algorithms on triangular surface meshes in Python.

# Installation
Clone this repository and its submodules:
```shell
git clone --recurse-submodules git@github.com:pvnieo/tridesic.git
```

Use pip to install dependencies:
```shell
cd tridesic && pip3 install -r requirements.txt
```

Install the `tridesic` python module:
```shell
cd .. && pip3 install ./tridesic
```

# Usage

`tridesic` provides 3 different algorithms to compute the geodesic matrix of a triangle mesh.
 - `get_heat_geodesics(vertices: np.array, faces: np.array)` compute the geodesic matrix using the heat method [[1]](#references). This method is the fastest, but gives inaccurate approximation when the quality of the mesh is very bad.
 - `get_fmm_geodesics(vertices: np.array, faces: np.array)` compute the geodesic matrix using the fast marching method [[2]](#references). This method is pretty fast, and gives accurate approximations (recommended).
 - `get_exact_geodesics(vertices: np.array, faces: np.array)` compute the geodesic matrix using the Exact geodesic algorithm [[3]](#references). This method computes the **exact** geodesic matrix, however, it's **very** slow.

Basic usage looks like:
```python
from tridesic import get_fmm_geodesics

mesh = read_mesh("mesh.obj")  # some function for reading meshes
vertices = mesh.vertices  # numpy array, shape: Vx3
faces = mesh.faces # numpy array, shape: Fx3

fmm_geodesics = get_fmm_geodesics(vertices, faces)  # numpy array, shape: VxV
```

# Test
run the `compute_geodesic.py` to verify if the installation is working:
```shell
cd test && python3 compute_geodesic.py
```
The output should be similar to:

```
The heat method took: 0.4s
Geodesic matrix shape: (1499, 1499)
Geodesic distance from vertex 2: [0.71181172 0.30513065 0.  ... 0.2607065  0.86021767 0.29388024]

Fast Marching Method took: 0.6s
Geodesic matrix shape: (1499, 1499)
Geodesic distance from vertex 2: [0.7697969  0.3232959  0.  ... 0.29297618 0.94736526 0.29405564]

Exact Geodesic Method took: 54.4s
Geodesic matrix shape: (1499, 1499)
Geodesic distance from vertex 2: [0.7486653  0.30913563 0.  ... 0.27650707 0.92577171 0.2864793 ]
```

# Credits
This repo implements small python bindings to:
 - Fast Marching Method & The Heat Method for Distance Computationalgorithms provided by [geometry-central](https://geometry-central.net/)
 - The implementation of Exact geodesic algorithm provided by [libigl](https://libigl.github.io/)

# References
[1] [The Heat Method for Distance Computation](http://www.cs.cmu.edu/~kmcrane/Projects/HeatMethod/paper.pdf)

[2] [Computing geodesic paths on manifolds](http://www-evasion.imag.fr/Membres/Franck.Hetroy/Teaching/ProjetsImage/2006/Bib/sethian_kimmel-1995.pdf)

[3] [The discrete geodesic problem](https://code.google.com/archive/p/geodesic/)