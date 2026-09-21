# Tutoriel CMake pour l'équipe

Objectif : récupérer WaveEquation, comprendre sa construction et savoir quoi relancer après une modification. Les commandes partent de la **racine du dépôt**, là où se trouve le `CMakeLists.txt` principal, sauf indication contraire.

## 1. Comprendre les outils

| Outil | Rôle |
|---|---|
| Git / GitHub | Partager le code et son historique. |
| VS Code | Éditer les fichiers et piloter les autres outils. |
| CMake | Lire les `CMakeLists.txt`, trouver les dépendances et générer la construction. |
| MSVC ou GCC | Compiler le C++ en code machine. |
| MSBuild ou Ninja | Organiser l'exécution des commandes de compilation et de liaison. |

Une **cible** CMake est un élément à construire. Ici, `Mesh`, `NumMethods` et `Solver` sont des bibliothèques ; `WaveEquation` est l'exécutable qui les utilise.

Le travail suit trois étapes distinctes : **configurer**, **compiler**, puis **exécuter**. Modifier un `.cpp` ne change pas l'exécutable tant qu'on ne le recompile pas.

## 2. Préparer Windows et VS Code

Pour utiliser le preset fourni, installer :

1. [Git](https://git-scm.com/downloads).
2. [CMake](https://cmake.org/download/), version **4.2 ou plus récente**, accessible dans le `PATH`.
3. [Visual Studio Build Tools](https://visualstudio.microsoft.com/downloads/), édition **2026**, avec la charge de travail C++, les outils MSVC x64 et le Windows SDK. L'éditeur Visual Studio complet n'est pas nécessaire pour travailler dans VS Code.
4. [VS Code](https://code.visualstudio.com/), avec **C/C++** (`ms-vscode.cpptools`) et **CMake Tools** (`ms-vscode.cmake-tools`).

Ouvrir un nouveau terminal après l'installation, puis vérifier :

```powershell
git --version
cmake --version
```

Cloner le dépôt et ouvrir son dossier racine dans VS Code :

```powershell
git clone https://github.com/arensaid2002-lab/WaveEquation.git
cd WaveEquation
code .
```

Si `code` n'est pas reconnu, ouvrir le dossier avec **Fichier > Ouvrir un dossier**. Le dépôt possède déjà ses fichiers CMake : il n'est pas nécessaire de lancer « CMake: Quick Start » pour en créer d'autres.

## 3. Configurer, compiler, exécuter

### Configurer

```powershell
cmake --preset MyPreset
```

Cette commande sélectionne le générateur `Visual Studio 18 2026`, l'architecture x64 et le dossier `out/build/MyPreset`. CMake cherche Eigen et OpenMP, puis prépare les fichiers de construction. Elle ne lance pas la simulation.

Un **preset** est un ensemble nommé de réglages. Pour voir ceux du dépôt :

```powershell
cmake --list-presets
cmake --build --list-presets
```

### Compiler en Debug

```powershell
cmake --build --preset MyPreset-debug --target WaveEquation --parallel
```

Le preset `MyPreset-debug` choisit réellement **Debug**, même si son libellé contient aussi le mot « Release ». Debug facilite le débogage. `--target WaveEquation` construit l'exécutable et les bibliothèques dont il dépend.

### Exécuter

```powershell
Push-Location .\out\build\MyPreset
.\Debug\WaveEquation.exe
Pop-Location
```

Le CSV est écrit dans `out/build/MyPreset/results.csv`, un emplacement déjà ignoré par Git. Le [README](../README.md#lire-les-résultats) explique son organisation. Cette exécution vérifie le fonctionnement du programme ; elle ne valide pas encore le modèle physique.

### Compiler en Release pour mesurer les performances

Il n'existe pas encore de preset de build Release. Le même dossier Visual Studio accepte néanmoins cette configuration :

```powershell
cmake --build out/build/MyPreset --config Release --target WaveEquation --parallel
Push-Location .\out\build\MyPreset
.\Release\WaveEquation.exe
Pop-Location
```

`--parallel` accélère la **construction** lorsque plusieurs compilations peuvent se faire en même temps. Le parallélisme du **programme exécuté** dépend, lui, d'OpenMP et du code.

Les options `--preset`, `--build`, `--config` et `--target` sont décrites dans la [référence de la commande CMake](https://cmake.org/cmake/help/latest/manual/cmake.1.html).

## 4. Faire la même chose dans VS Code

Avec le dossier racine ouvert et CMake Tools installé, utiliser la palette de commandes (`Ctrl+Shift+P`) :

1. **CMake: Select Configure Preset** : choisir `MyPreset`.
2. **CMake: Configure** : préparer la construction.
3. **CMake: Select Build Preset** : choisir `MyPreset-debug`.
4. **CMake: Build** : compiler ; sélectionner `WaveEquation` si une cible est demandée.
5. Utiliser le terminal et la commande d'exécution ci-dessus pour conserver le même emplacement de CSV. Pour poser des points d'arrêt, lancer **CMake: Debug** sur `WaveEquation`.

Si les commandes de presets ne sont pas proposées, vérifier que le dossier contient `CMakePresets.json` et régler **CMake: Use CMake Presets** sur `always` dans les paramètres de l'espace de travail. L'intégration est présentée dans la [documentation CMake Tools](https://code.visualstudio.com/docs/cpp/cmake-quickstart).

## 5. Comprendre un module du projet

Le fichier [Mesh/CMakeLists.txt](../Mesh/CMakeLists.txt) contient l'équivalent de :

```cmake
add_library(Mesh src/MeshHandler.cpp)
target_include_directories(Mesh PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(Mesh PUBLIC Eigen3::Eigen)
```

- `add_library` définit la bibliothèque et ses sources à compiler.
- `target_include_directories` donne les chemins d'en-têtes : les consommateurs de `Mesh` peuvent écrire `#include "MeshHandler.h"`.
- `target_link_libraries` déclare les dépendances. La cible Eigen transmet notamment ses chemins d'en-têtes ; Eigen ne nécessite pas ici de bibliothèque binaire à lier.

| Portée | Effet sur les exigences d'utilisation |
|---|---|
| `PRIVATE` | Pour la cible elle-même. |
| `PUBLIC` | Pour la cible et les cibles qui l'utilisent. |
| `INTERFACE` | Pour les cibles qui l'utilisent. |

Ces portées permettent de transmettre les réglages au bon endroit, plutôt que d'ajouter partout des chemins absolus. Voir [target_link_libraries](https://cmake.org/cmake/help/latest/command/target_link_libraries.html).

Pour ajouter un `.cpp` dans un module, l'ajouter à son `add_library(...)`, puis reconfigurer et compiler. Pour un nouveau module, créer son `CMakeLists.txt`, l'ajouter avec `add_subdirectory(...)` à la racine et le lier à la cible qui l'utilise. `Solver` dépend déjà de `Mesh` et de `NumMethods`.

### Dépendances externes

La racine utilise `find_package(Eigen3 CONFIG QUIET)`. Une installation Eigen détectable est prioritaire ; sinon, `FetchContent` récupère la version `5.0.1` dans le dossier de build. Inutile de copier les en-têtes Eigen dans le dépôt. Pour une installation personnelle, `Eigen3_DIR` peut pointer vers le dossier contenant `Eigen3Config.cmake`. Voir [FetchContent](https://cmake.org/cmake/help/latest/module/FetchContent.html).

### OpenMP et la cible Solver

Actuellement, OpenMP est lié à `WaveEquation`, mais les directives `#pragma omp` sont dans les sources de `Solver`. Une dépendance de l'exécutable ne transmet pas ses options de compilation en remontant vers ses bibliothèques.

Pour activer OpenMP sur ces boucles, ajouter dans `Solver/CMakeLists.txt`, après `add_library(Solver ...)` :

```cmake
target_link_libraries(Solver PUBLIC OpenMP::OpenMP_CXX)
```

Puis reconfigurer et recompiler. C'est une modification de configuration à réaliser dans le projet ; le présent tutoriel ne l'applique pas. La cible importée transmet les options nécessaires au compilateur et à la liaison. Voir [FindOpenMP](https://cmake.org/cmake/help/latest/module/FindOpenMP.html).

## Autres environnements

Le preset partagé impose Visual Studio 2026. Les commandes suivantes choisissent un autre générateur sans modifier ce fichier. Utiliser un dossier de build distinct pour chaque générateur.

### Windows avec Visual Studio Build Tools 2022

Avec CMake récent et les outils C++ 2022 installés, depuis PowerShell :

```powershell
cmake -S . -B out/build/vs2022 -G "Visual Studio 17 2022" -A x64
cmake --build out/build/vs2022 --config Debug --target WaveEquation --parallel
Push-Location .\out\build\vs2022
.\Debug\WaveEquation.exe
Pop-Location
```

### Linux avec GCC et Ninja

Installer Git, CMake récent, GCC/G++ avec OpenMP et Ninja, puis depuis la racine :

```bash
cmake -S . -B build/linux-release -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build build/linux-release --target WaveEquation --parallel
cd build/linux-release
./WaveEquation
cd ../..
```

`-S` désigne les sources et `-B` le dossier généré. Avec **Ninja classique**, `CMAKE_BUILD_TYPE` choisit Debug ou Release pendant la configuration ; avec **Visual Studio**, le choix se fait à la construction via `--config`.

Sur macOS, le preset Windows ne convient pas et il faut un compilateur accompagné d'un runtime OpenMP détectable par CMake. Ne pas supposer que le Clang livré par défaut suffit à cette configuration.

## 6. Le cycle quotidien avec Git

| Situation | Action |
|---|---|
| Lire le code après un `git pull` | Aucune compilation nécessaire. |
| Exécuter les nouvelles modifications | Recompiler pour mettre à jour l'exécutable local. |
| Modifier un `.cpp` ou un `.h` déjà déclaré | Relancer le build ; les fichiers concernés et leurs dépendants sont reconstruits. |
| Modifier les fichiers CMake ou ajouter une source | Relancer la configuration, puis le build. |
| Changer de compilateur ou de générateur | Configurer un nouveau dossier de build. |

Les sources et `CMakePresets.json` sont partagés. `build/`, `out/` et `CMakeUserPresets.json` restent locaux grâce au `.gitignore`. Les réglages propres à une machine peuvent aller dans `CMakeUserPresets.json` ; ce fichier est facultatif et n'est pas à committer. Voir la [documentation des presets](https://cmake.org/cmake/help/latest/manual/cmake-presets.7.html).

Avant un commit, vérifier `git status` et sélectionner uniquement les modifications voulues. Un `results.csv` créé à la racine n'est pas ignoré actuellement ; les commandes de lancement ci-dessus le placent dans un dossier de build ignoré.

## 7. Dépannage rapide

| Message ou symptôme | Vérification |
|---|---|
| `cmake` ou `git` introuvable | Installation et `PATH`, puis nouveau terminal. |
| Générateur `Visual Studio 18 2026` absent | CMake 4.2 minimum et Build Tools 2026 ; sinon utiliser la procédure 2022. |
| `cl.exe` introuvable ou échec du compilateur | Installation de la charge C++ et du SDK ; essayer le terminal développeur x64 de Visual Studio. |
| `Could NOT find OpenMP` | Compilateur et runtime compatibles OpenMP ; consulter le journal de configuration. |
| Échec du téléchargement Eigen | Accès à GitLab et Git disponible, ou installation Eigen détectable avec `Eigen3_DIR`. |
| Message « downloading and building Kokkos » | Le texte du message est incorrect : le dépôt configuré est bien celui d'Eigen. |
| Générateur incompatible avec le cache | Choisir un nouveau dossier `-B`, sans réutiliser celui d'un autre générateur. |
| Résultats inchangés après une modification | Recompiler et vérifier le chemin ainsi que la configuration de l'exécutable lancé. |
| Programme terminé, mais CSV introuvable | Vérifier le dossier de travail au lancement. |

Pour les limites numériques du solveur, consulter les [points à connaître du README](../README.md#points-à-connaître-sur-la-version-actuelle).
