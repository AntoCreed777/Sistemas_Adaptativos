from pathlib import Path
from typing import List

import pandas as pd

ROOT = Path(__file__).resolve().parents[1]
PATH_CSV = ROOT / "experiments" / "results" / "experiments_taboo.csv"
SUMMARY_CSV = ROOT / "analisis" / "results" / "summary_taboo.csv"


def _read_experiments_csv(path: Path) -> pd.DataFrame:
    df = pd.read_csv(path, sep=";", engine="python")
    df.columns = df.columns.str.strip()

    for cand in ("Unnamed: 0", "index", "Index"):
        if cand in df.columns:
            df = df.drop(columns=[cand])

    if "respuesta" in df.columns and df["respuesta"].dtype == object:
        try:
            df["respuesta"] = (
                df["respuesta"]
                .astype(str)
                .str.extract(r"(-?\d+(?:\.\d+)?)$", expand=False)
            )
        except Exception:
            pass

    numeric_cols: List[str] = [
        "respuesta",
        "tiempo",
        "taboo_len",
        "densidad",
        "vertices",
    ]
    for col in numeric_cols:
        if col in df.columns:
            df[col] = pd.to_numeric(df[col], errors="coerce")

    needed = [c for c in ("vertices", "taboo_len", "densidad") if c in df.columns]
    if needed:
        df = df.dropna(subset=needed)
    return df


def main():
    if not PATH_CSV.exists():
        raise FileNotFoundError(f"No se encontró el archivo: {PATH_CSV}")

    df = _read_experiments_csv(PATH_CSV)

    required = {"vertices", "densidad", "respuesta", "tiempo"}
    missing = required - set(df.columns)
    if missing:
        raise ValueError(f"Faltan columnas en el CSV: {sorted(missing)}")

    # Agrupar por vertices y densidad, calcular medias y desviación estándar
    summary = (
        df.groupby(["vertices", "densidad"], as_index=False)
        .agg(
            media_respuesta=("respuesta", "mean"),
            media_tiempo=("tiempo", "mean"),
            std_tiempo=("tiempo", "std"),
        )
        .sort_values(["vertices", "densidad"])
    )

    # Redondear a 2 decimales
    for col in ["media_respuesta", "media_tiempo", "std_tiempo"]:
        if col in summary.columns:
            summary[col] = summary[col].round(2)

    SUMMARY_CSV.parent.mkdir(parents=True, exist_ok=True)
    summary.to_csv(SUMMARY_CSV, index=False)
    print(f"Resumen guardado en: {SUMMARY_CSV}")


if __name__ == "__main__":
    main()
