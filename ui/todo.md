# DONE :
- Structure de projet qui reprend les concepts de core (les inclus et les compiles) : Encapsulation ✅
- Intégrer les différents écrans de Game Over/Introduction ✅
- Affichage de la grille virtuelle (celle dans le core) sur la fenêtre ✅
    - Comment faire le lien entre les cases et les dimensions réelles ? Option choisie : On calcule la taille d'une "case" du jeu virtuel (core) et le jeu réel (sdl) sera composé de case de cette taille. ✅
    - Affichage du joueur ✅
- Afficher les obstacles différemment de la carte de base (contrainte de taille des véhicules/rondins) ✅

# NOT DONE :
- Fluidité des déplacements : Pas de *sauts* de cases mais plutôt des *animations* : ❌
    - Piste : faire des cases virtuelles de 8x8 pixels, qui seraient occupée par des sprites plus gros (32x32), ce qui implique qu'un objet n'occupe plus une seule et unique case mais 4 cases. Les déplacements se font de 8 en 8 pixels, ce qui serait beaucoup plus fluide que 32 par 32 ou 48 par 48 actuellement. 
    - Implique d'importantes modifications du cœur du jeu qui estime qu'une case = un objet.
        Conclusion : On évite, SDL2 est une surcouche graphique et ne doit pas impliquer d'aussi gros changement du cœur du jeu.

# TODO :
- Réintégrer les logiques de gestion de jeu dans l'encapsulateur de GameState : UIGameState
    - Les cooldowns (qui ne doivent plus être en *frames* mais en réelles *secondes*) :
        - Utilisation de SDL2 GetTicks : Permet de récupérer le temps depuis le lancement du jeu en *ms* ✅
        - Piste : Le delta permettrait de faire diminuer les *accumulateurs* des voitures qui deviendrait un accumulateur à *secondes* (actuellement *frames*)
    - En somme : Permettre la mise à jour des effets, etc, depuis SDL et non pas notre moteur (core) ✅
- Générer/Créer les sprites nécessaires et aux bonnes dimensions, pour l'instant des placeholders
    - Joueur ✅
    - Voiture (Taille 1 ... 6, 6 = Long Bus) dans les deux sens (SDL_FLIP) ✅
    - Nénuphar
    - Rondins ✅
    - Route ✅ (Avec séparateurs en fonction de l'environnement)
    - Herbe (safezone) ✅
    - Eau
    - Arbre ✅
- Ajouter le RowManager de Nils
- Mettre des sons (trains, collisions, plouf dans l'eau)
- Inclure les sources des différents assets que l'on n'a pas fais