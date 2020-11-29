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
 - [Fast Marching Method](http://www-evasion.imag.fr/Membres/Franck.Hetroy/Teaching/ProjetsImage/2006/Bib/sethian_kimmel-1995.pdf) & [The Heat Method for Distance Computation](http://www.cs.cmu.edu/~kmcrane/Projects/HeatMethod/paper.pdf) algorithms provided by [geometry-central](https://geometry-central.net/)
 - The implementation of [Exact geodesic algorithm](https://code.google.com/archive/p/geodesic/) provided by [libigl](https://libigl.github.io/)
