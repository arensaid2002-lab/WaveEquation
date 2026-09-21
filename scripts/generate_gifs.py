"""Trace les CSV du solveur C++ en GIF, sans recalculer les schemas numeriques."""

import argparse
import hashlib
import json
from pathlib import Path

import matplotlib

matplotlib.use("Agg")
import matplotlib.pyplot as plt
from matplotlib.animation import PillowWriter
import numpy as np
from PIL import Image


def render(mesh, results, columns, parameters, title, color, output, fps):
    """Les deux GIF utilisent les memes axes, instants et vitesse de lecture."""
    background, ink = "#f8fafc", "#172c42"
    fig = plt.figure(figsize=(8, 4.5), dpi=100, facecolor=background)
    ax = fig.add_axes((0.10, 0.20, 0.85, 0.55), facecolor=background)
    fig.text(0.055, 0.92, title, fontsize=21, weight="bold", color=ink)
    cfl_text = f"{parameters['cfl']:g}".replace(".", ",")
    fig.text(0.055, 0.85,
             f"Advection périodique · {len(mesh)} points · c = {parameters['c']:g} · CFL = {cfl_text}",
             fontsize=11, color="#526577")
    clock = fig.text(0.95, 0.925, "", ha="right", fontsize=14, weight="bold", color=color)
    peak = fig.text(0.95, 0.85, "", ha="right", fontsize=11, color="#526577")

    ax.plot(mesh, results[1:, 0], color="#8d99a8", linewidth=1.5,
            linestyle=(0, (4, 3)), label="Profil initial (t = 0)", zorder=2)
    wave, = ax.plot(mesh, results[1:, 0], color=color, linewidth=2.6,
                    label="Résultat numérique", zorder=3)
    area, = ax.fill([], [], color=color, alpha=0.10, linewidth=0, zorder=1)
    ax.set(xlim=(mesh[0], mesh[-1]), ylim=(-5, 110), xlabel="Position x", ylabel="Amplitude u(x, t)")
    ax.set_xticks(np.linspace(mesh[0], mesh[-1], 6))
    ax.set_yticks([0, 25, 50, 75, 100])
    ax.tick_params(colors="#526577", labelsize=10, length=0, pad=7)
    ax.xaxis.label.set(color=ink, size=11)
    ax.yaxis.label.set(color=ink, size=11)
    ax.xaxis.labelpad = 10
    ax.yaxis.labelpad = 10
    ax.grid(color="#dde4ec", linewidth=0.7)
    ax.set_axisbelow(True)
    for name, spine in ax.spines.items():
        spine.set_visible(name in ("left", "bottom"))
        spine.set_color("#c2cdd8")
    ax.legend(loc="lower left", bbox_to_anchor=(0, 1.01), ncol=2,
              frameon=False, fontsize=10, handlelength=2.7, borderaxespad=0)
    fig.text(0.10, 0.045, "Différence amont · données du solveur C++ · lecture au ralenti",
             fontsize=10, color="#526577")

    writer = PillowWriter(fps=fps)
    with writer.saving(fig, str(output), dpi=100):
        for column in columns:
            values = results[1:, column]
            wave.set_ydata(values)
            area.set_xy(np.column_stack((np.r_[mesh[0], mesh, mesh[-1]], np.r_[0, values, 0])))
            clock.set_text(f"t = {results[0, column]:.3f}".replace(".", ","))
            peak.set_text(f"max u = {values.max():.1f}".replace(".", ","))
            writer.grab_frame(facecolor=background)
    plt.close(fig)
    with Image.open(output) as gif:
        if not getattr(gif, "is_animated", False):
            raise ValueError(f"Le GIF ne contient pas d'animation : {output}")
        return {"file": output.name, "frames": gif.n_frames, "bytes": output.stat().st_size}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--input-dir", type=Path, required=True, help="Dossier des quatre exports C++")
    parser.add_argument("--output-dir", type=Path, default=Path("docs/assets"))
    parser.add_argument("--max-time", type=float, default=0.674, help="Fin de la fenetre temporelle affichee")
    parser.add_argument("--frames", type=int, default=161)
    parser.add_argument("--fps", type=int, default=25)
    parser.add_argument("--source-revision", help="Commit du solveur utilise, si connu")
    args = parser.parse_args()
    if args.max_time <= 0 or args.frames < 2 or not 1 <= args.fps <= 100:
        parser.error("Il faut max-time > 0, frames >= 2 et 1 <= fps <= 100.")

    parameters = json.loads((args.input_dir / "parameters.json").read_text(encoding="utf-8"))
    mesh = np.loadtxt(args.input_dir / "mesh.csv", delimiter=",").reshape(-1)
    runs = {
        "explicit": np.loadtxt(args.input_dir / "explicit.csv", delimiter=","),
        "implicit": np.loadtxt(args.input_dir / "implicit.csv", delimiter=","),
    }
    for name, result in runs.items():
        if result.ndim != 2 or result.shape[0] != mesh.size + 1 or result.shape[1] < 2:
            raise ValueError(f"Dimensions incorrectes : {name}")
        if not np.isfinite(result).all() or not np.all(np.diff(result[0]) > 0):
            raise ValueError(f"Valeurs non finies ou temps non croissants : {name}")
        if result[1:].min() < -5 or result[1:].max() > 110:
            raise ValueError("Les valeurs depassent les axes fixes : adapter les limites du graphique.")
    if not np.isfinite(mesh).all() or not np.all(np.diff(mesh) > 0):
        raise ValueError("Le maillage doit etre fini et strictement croissant.")
    if not np.array_equal(runs["explicit"][0], runs["implicit"][0]):
        raise ValueError("Les deux simulations doivent utiliser les memes instants.")
    if not np.array_equal(runs["explicit"][1:, 0], runs["implicit"][1:, 0]):
        raise ValueError("Les deux simulations doivent avoir le meme profil initial.")

    times = runs["explicit"][0]
    last = int(np.searchsorted(times, args.max_time, side="right") - 1)
    if last < 1:
        raise ValueError("La fenetre choisie contient moins de deux instants.")
    columns = np.unique(np.rint(np.linspace(0, last, min(args.frames, last + 1))).astype(int))
    args.output_dir.mkdir(parents=True, exist_ok=True)
    outputs = []
    for name, title, color in [
        ("explicit", "Euler explicite", "#1f7a9b"),
        ("implicit", "Euler implicite", "#c06640"),
    ]:
        output = args.output_dir / f"euler-{name}.gif"
        outputs.append(render(mesh, runs[name], columns, parameters, title, color, output, args.fps))
        print(f"GIF cree : {output}", flush=True)

    root = Path(__file__).resolve().parents[1]
    sources = ["examples/export_animations.cpp", "Mesh/src/MeshHandler.cpp",
               "NumMethods/src/NumMethods.cpp", "Solver/src/SolverHandler.cpp",
               "Solver/src/SolverWaveEquation.cpp"]
    report = {
        "solver_revision": args.source_revision,
        "parameters": parameters,
        "csv_shape": list(runs["explicit"].shape),
        "displayed_time_interval": [float(times[columns[0]]), float(times[columns[-1]])],
        "frame_columns_zero_based": columns.tolist(),
        "fps": args.fps,
        "time_labels": "Premiere ligne des CSV exportes par le solveur",
        "interpolation": "Aucune interpolation temporelle ; segments entre points du maillage",
        "source_sha256": {path: hashlib.sha256((root / path).read_bytes()).hexdigest() for path in sources},
        "data_sha256": {path.name: hashlib.sha256(path.read_bytes()).hexdigest()
                        for path in [args.input_dir / name for name in
                                     ["mesh.csv", "explicit.csv", "implicit.csv", "parameters.json"]]},
        "animations": outputs,
    }
    (args.output_dir / "animations.json").write_text(
        json.dumps(report, ensure_ascii=False, indent=2) + "\n", encoding="utf-8")


if __name__ == "__main__":
    main()
