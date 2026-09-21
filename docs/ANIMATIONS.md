# Générer les animations explicite et implicite

Les deux GIF du [README](../README.md#résultats-animés) représentent des résultats calculés par `SolverWaveEquation`. L'exécutable optionnel `WaveEquationAnimations` appelle successivement `Solve("E")` et `Solve("I")`, puis exporte les matrices. Le script Python lit ces données et produit les animations.

## Paramètres de la démonstration

| Paramètre | Valeur |
|---|---|
| Maillage | 100 points, bornes 0 et 100 incluses |
| Profil initial | 100 pour `40 < x < 60`, 0 ailleurs |
| Vitesse `c` | 300 |
| Nombre de Courant `CFL` | 0,5 pour les deux schémas |
| Durée calculée | 10 |
| Fenêtre affichée | Pas 0 à 400, soit `t ≈ 0,6734` |
| Animation | 161 images, 25 images/s, boucle d'environ 6,44 s |

Les paramètres de calcul sont dans [examples/export_animations.cpp](../examples/export_animations.cpp). Cet exemple utilise les classes du projet et ne modifie pas les réglages de `Main/main.cpp`. Le calcul génère ici **101 lignes et 5 941 colonnes** par CSV, état initial inclus.

Chaque image correspond à un instant déjà calculé : le script sélectionne des colonnes communes aux deux CSV, sans interpolation temporelle. Les lignes relient les points du maillage. La courbe grise est le profil initial fixe, et le temps affiché provient de la première ligne des CSV. Les unités sont celles du modèle ; la vitesse de lecture du GIF ne représente pas le temps de calcul.

## Préparer le projet

Installer les outils et effectuer une première configuration avec le [tutoriel CMake](TUTORIEL_CMAKE.md), en suivant la procédure de son système. Sous macOS, conserver la configuration Apple Clang et les chemins OpenMP indiqués dans le tutoriel.

Python **3.11 ou plus récent** et les paquets NumPy, Matplotlib et Pillow sont nécessaires uniquement pour créer les GIF. Toutes les commandes ci-dessous partent de la racine du dépôt.

## Windows, PowerShell et Visual Studio 2026

Créer un environnement Python local :

```powershell
py -m venv build/animation-python
.\build\animation-python\Scripts\python.exe -m pip install numpy matplotlib pillow
```

Activer l'exporteur, le compiler et exécuter les deux schémas :

```powershell
cmake --preset MyPreset -DWAVE_EQUATION_BUILD_ANIMATIONS=ON
cmake --build out/build/MyPreset --config Release --target WaveEquationAnimations --parallel
New-Item -ItemType Directory -Force out/build/MyPreset/animation-data | Out-Null
Push-Location .\out\build\MyPreset\animation-data
$env:OMP_NUM_THREADS = "2"
..\Release\WaveEquationAnimations.exe
Pop-Location
.\build\animation-python\Scripts\python.exe scripts/generate_gifs.py --input-dir out/build/MyPreset/animation-data --output-dir docs/assets
```

## macOS et Linux

Créer l'environnement Python :

```bash
python3 -m venv build/animation-python
build/animation-python/bin/python -m pip install numpy matplotlib pillow
```

Choisir le dossier **déjà configuré** dans le tutoriel. Sous Linux, remplacer la première ligne par `WAVE_BUILD_DIR=build/linux-release`.

```bash
WAVE_BUILD_DIR=build/macos-release
cmake -S . -B "$WAVE_BUILD_DIR" -DWAVE_EQUATION_BUILD_ANIMATIONS=ON
cmake --build "$WAVE_BUILD_DIR" --target WaveEquationAnimations --parallel
mkdir -p "$WAVE_BUILD_DIR/animation-data"
(
  cd "$WAVE_BUILD_DIR/animation-data" || exit 1
  OMP_NUM_THREADS=2 ../WaveEquationAnimations
)
build/animation-python/bin/python scripts/generate_gifs.py --input-dir "$WAVE_BUILD_DIR/animation-data" --output-dir docs/assets
```

Ces commandes peuvent aussi être exécutées dans le terminal intégré de VS Code. `OMP_NUM_THREADS=2` règle le nombre de threads du calcul ; `--parallel` règle le parallélisme de compilation.

## Fichiers produits

L'exporteur écrit `explicit.csv`, `implicit.csv`, `mesh.csv` et `parameters.json` dans le dossier de travail choisi. Ces données et l'environnement Python restent dans `build/` ou `out/`, déjà ignorés par Git.

Le script écrit [euler-explicit.gif](assets/euler-explicit.gif), [euler-implicit.gif](assets/euler-implicit.gif) et [animations.json](assets/animations.json) dans `docs/assets/`. Le fichier JSON conserve les paramètres, les colonnes affichées et les empreintes des données et sources utilisées. Ces trois fichiers sont versionnés pour afficher les animations directement sur GitHub.

Les options `--max-time`, `--frames` et `--fps` permettent de modifier la fenêtre et la lecture du GIF sans relancer le solveur. Une modification de la vitesse, du maillage ou du CFL dans l'exporteur demande une recompilation, puis une nouvelle exécution. Après une modification de `Main/main.cpp`, reporter les paramètres voulus dans l'exporteur si l'on souhaite les mêmes conditions pour les animations.

La génération utilise [Matplotlib et son export GIF avec Pillow](https://matplotlib.org/stable/api/_as_gen/matplotlib.animation.PillowWriter.html). La procédure complète a été exécutée sous Linux ; les commandes Windows et macOS sont adaptées aux configurations du tutoriel.
