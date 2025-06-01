# DONE :
- Si 3 pas en arrière -> Game Over ✅
- Génération procédurale de la carte avec possibilité de faire jusqu'à 2 pas en arrière. L'écart entre les safe-zones augmente avec le score. ✅
    - Prise en charge des écarts grandissants entre les safe-zones ✅
    - Ajout d'arbres sur les safezones, ceux-ci ne doivent pas obstruer le spawn du joueur ✅
- Ajout des véhicules, ceux-ci doivent se déplacer indépendamment du joueur, donc en fonction de leur vitesse qui leur est propre.
    - Mise en place de liste doublement chaîné pour permettre l'ajout des véhicules par le haut et leur sortie par le bas ✅
    - Avoir des routes avec plusieurs véhicules en fonction de la difficulté, ces véhicules ne doivent pas se superposés  : ✅
        - Création d'une "EffectQueue", celle-ci sera peuplée d'évènements qui sont prévus mais pas encore fait, à chaque frame on décrémente le temps d'attente de tous les évènements, quand un évènement a son temps à 0, il est réalisé. ✅
        - Ajout de cars dans les effects queues pour en avoir plusieurs dans la ligne, le temps doit prendre en compte la vitesse et la taille du véhicule actuel pour que le car soit ajouté seulement lorsque le car actuel n'empiète pas sur la zone de départ : Attendre qu'il parcourt 6 + 1 cases. Soit T = D/V = 7... ✅
    - Prendre en charge la taille des véhicules qui doit être affichée (et notamment quand un véhicule avec size > 1 quitte l'écran, sa queue doit quitter progressivement) ✅
- Gestion des collisions qui entrainent la fin de jeu, même pour les grands véhicules (queue, corps et tête du véhicule) ✅ [REWORKED] ✅
- Gestion de l'abandon (pas bouger pendant trop longtemps) ✅
- Correction du scintillement de l'écran (clignottage) ✅
- Véhicule différencé selon leurs directions ">" et "<". ✅
- Nettoyage des zones mémoires lors de la fin du jeu. ✅
- Plus de deque, que des queues simples. ✅
- Séparation des logiques. ✅
- Impossible d'aller au bord de map grid starting -1 et finishing -1, faut +1 des deux côtés. ✅
- Ajout des rivières et des rondins de bois sur celles-ci.
    - Ajout d'eau et de rondins sans logique supplémentaire. ✅
    - Une fois sur un rondin, on est porté par celui-ci. Si le rondin sort de la carte, on perd.  ✅
    - Les rondins ont une vitesse plus lentes, et s'accourcissent avec le temps, leur fonctionnement est contraire aux voitures. ✅
- Rework de la logique de mise à jour des effets. Buggé, freeze.  ✅
- Le score final vaut 1 de + que le vrai score si le joueur meurt en avançant, idem quand il avance dans un obstacle. ✅
- Gestion du score ✅
- ReadMe ✅

# TODO :
- Ecarts entre les voitures aléatoires en fonction de la difficulté, actuellement fixe à 5 (LOGS)/12 (CARS).
- Refactoriser les tests unitaires :
    - Utiliser exclusivement la fonction playermove ? A voir si c'est une bonne idée.

## Bonus :
- Documentation au propre (et l'inclure dans le readme)