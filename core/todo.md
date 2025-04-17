# TODO :
- Si 3 pas en arrière -> Game Over ✅
- Génération procédurale de la carte avec possibilité de faire jusqu'à 2 pas en arrière. L'écart entre les safe-zones augmente avec le score. ✅
    - Prise en charge des écarts grandissants entre les safe-zones ✅
    - Ajout d'arbres sur les safezones, ceux-ci ne doivent pas obstruer le spawn du joueur ✅
- Ajout des véhicules, ceux-ci doivent se déplacer indépendamment du joueur, donc en fonction de leur vitesse qui leur est propre.
    - Mise en place de liste doublement chaîné pour permettre l'ajout des véhicules par le haut et leur sortie par le bas ✅
    - Avoir des routes avec plusieurs véhicules en fonction de la difficulté, ces véhicules ne doivent pas se superposés  
    - Prendre en charge la taille des véhicules qui doit être affichée (et notamment quand un véhicule avec size > 1 quitte l'écran, sa queue doit quitter progressivement)
- Gestion des collisions qui entrainent la fin de jeu ✅
- Gestion du score
- Gestion de l'abandon (pas bouger pendant trop longtemps)


## Bonus :
- ReadMe
- Documentation au propre