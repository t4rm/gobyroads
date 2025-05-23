# DONE :
- Structure de projet qui reprend les concepts de core (les inclus et les compiles) : Encapsulation ✅

# TODO :
- Affichage de la grille virtuelle (celle dans le core) sur la fenêtre
    - Comment faire le lien entre les cases et les dimensions réelles ? Option choisie : On calcule la taille d'une "case" du jeu virtuel (core) et le jeu réel (sdl) sera composé de case de cette taille. ✅
    - Affichage du joueur ✅
    - Fluidité des déplacements : Pas de *sauts* de cases mais plutôt des *animations*
- Réintégrer les logiques de gestion de jeu dans l'encapsulateur de GameState : UIGameState
    - Les cooldowns (qui ne doivent plus être en *frames* mais en réelles *secondes*) :
        - Utilisation de SDL2 GetTicks : Permet de récupérer le temps depuis le lancement du jeu en *ms*
        - Piste : Le delta permettrait de faire diminuer les *accumulateurs* des voitures qui deviendrait un accumulateur à *secondes* (actuellement *frames*)
    - En somme : Permettre la mise à jour des effets, etc, depuis SDL et non pas notre moteur (core) qui 
- Générer/Créer les sprites nécessaires et aux bonnes dimensions, pour l'instant des placeholders
    - Joueur
    - Voiture (Taille 1 ... 6, 6 = Long Bus) dans les deux sens (SDL_FLIP)
    - Nénuphar
    - Rondins
    - Route
    - Herbe (safezone)
    - Arbre
