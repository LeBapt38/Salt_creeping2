import matplotlib.pyplot as plt
from matplotlib.ticker import MultipleLocator, FuncFormatter
import matplotlib.animation as animation
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

taille *= 1e6

plt.bar(taille, proportion, width = 1, edgecolor='black', alpha=0.7)
plt.xlabel("Size of crystals in µm", fontsize = 16)
plt.ylabel("Proportion of crystals", fontsize = 16)
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

## Animation
nb_fichiers = 104

df = pd.read_csv("image0.dat", delimiter="\s+", header=None)
grille_types = df.to_numpy()
grille_animation = np.full((nb_fichiers, 100, 100,3), 0)

for i in range(nb_fichiers) :
    df = pd.read_csv(f"image{i}.dat", delimiter="\s+", header=None)
    grille_types = df.to_numpy()
    grille_types = np.flip(grille_types.T, axis=0)
    dx = 1e-7

    nb_cristaux = grille_types.max()

    grille_col = np.zeros((grille_types.shape[0], grille_types.shape[1], 3), dtype=np.uint8)
    mask_eau = grille_types == -1
    grille_col[mask_eau] = [0, 0, 255]
    mask_air = grille_types == -2
    grille_col[mask_air] = [255,255,255]
    for j in range(nb_cristaux + 1) :
        mask_cristaux = grille_types == j
        grille_col[mask_cristaux] = [30 + j, 30+j, 30+j]
    grille_animation[i,:,:,:] = grille_col[-100:, -100:, :]


# Create figure and axis
fig, ax = plt.subplots()
im = ax.imshow(grille_animation[0], animated=True)  # Initialize with the first frame
ax.axis('off')  # Hide axes for a clean look

# Update function for the animation
def update(frame):
    im.set_data(grille_animation[frame])  # Update image data
    return [im]

# Create the animation
ani = animation.FuncAnimation(fig, update, frames=nb_fichiers, interval=100, blit=False, repeat=True)

ani.save("animation77frame.gif", fps=5)

# Show the animation
plt.show()





