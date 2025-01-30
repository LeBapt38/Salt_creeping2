import matplotlib.pyplot as plt
from matplotlib.ticker import MultipleLocator, FuncFormatter
import numpy as np
import pandas as pd
from math import pi

## taille cristaux
test = np.random.rand(200)

df = pd.read_csv("taille_cristaux.dat", delimiter="\s+", header=None)
taille_crist = df.to_numpy()
taille_crist = taille_crist[taille_crist < 1e-5]
nbCas = 10

mini = taille_crist.min()
maxi = taille_crist.max()

taille = np.linspace(mini, maxi, nbCas)
pas = taille[1] - taille[0]
nombre = np.zeros(nbCas)

for i in range(len(taille)) :
    mask1 = taille_crist < taille[i] + pas/2
    mask2 = taille_crist > taille[i] - pas/2
    mask = mask1 & mask2
    nombre[i] = len(taille_crist[mask])

proportion = nombre / nombre.sum()

plt.plot(taille, proportion)
plt.xlabel("Taille des cristaux en m")
plt.ylabel("Proportion de cristaux de cette taille")
plt.show()

## affichage
test = np.array([[-2, -1, -1],[0,0,1],[3,3,4]])

df = pd.read_csv("image90.dat", delimiter="\s+", header=None)
grille_types = df.to_numpy()
grille_types = np.flip(grille_types.T, axis=0)
dx = 1e-7

nb_cristaux = grille_types.max()

grille_col = np.zeros((grille_types.shape[0], grille_types.shape[1], 3), dtype=np.uint8)
mask_eau = grille_types == -1
grille_col[mask_eau] = [0, 0, 255]
mask_air = grille_types == -2
grille_col[mask_air] = [255,255,255]
for i in range(nb_cristaux + 1) :
    mask_cristaux = grille_types == i
    grille_col[mask_cristaux] = [30 + i, 30+i, 30+i]



extent = [0, dx * grille_types.shape[0], 0, dx * grille_types.shape[1]]

plt.imshow(grille_col, extent = extent)
plt.show()