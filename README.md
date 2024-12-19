# FPT 2025, Salt Creeping
L'objectif de cette deuxième modélisation est de reproduire les observations microscopiques expérimentale sur le phénomène du salt creeping. On va reprendre l'idée d'une méthode type Monte-Carlo pour reproduire la cristallisation. Les hypothèses fortes créant une dépendanse dans l'algorithme précédent seront relachées. De plus des phénomènes de recombination des cristaux seront considérés. 

## Le Modèle
...

### Gestion des condition aux limites

Afin de gérer les bords de la simulation, le choix à été fait de prendre une case typique qui sera renvoyé dans le cas d'une sortie du tableau. 

Une autre possibilité serait de faire des conditions périodiques pour une direction et des cases typiques dans l'autre, reproduisant ainsi un tube.

## Organisation du code
Le code s'oganise sur 3 classes "amies". La première classe "case" permet de savoir les propriétés autour d'un point donner. Ces points sont regroupés dans un tableau qui fait partie de la deuxième classe, "reseau". Les cases sont aussi regroupé par ensemble simplement connectés de type >0 (présence de cristal) dans une classe "cristal".