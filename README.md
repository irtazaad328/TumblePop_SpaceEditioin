# Tumble-POP (Space Edition) 👻
**A 2D Arcade Platformer** — built with C++ and SFML

Inspired by the classic arcade game, Tumble-POP (Space Edition) is a 2D platformer where you pilot a hero armed with a vacuum backpack across two sectors, capturing and shooting enemies to clear each stage. Features two playable characters, two active enemy types, slope physics, projectile combat, a combo scoring system, and full audio support.

---

## Gameplay

Choose your hero, select a sector, and clear all enemies to advance. Use the vacuum to stun and bag enemies, then launch them as projectiles at others. Watch out — touching an enemy costs a life.

| Sector | Description |
|--------|-------------|
| Sector 1 — The Beginning | 4 ghosts, 2 skeletons on a multi-platform layout |
| Sector 2 — The Deep | 6 ghosts, 4 skeletons on a tighter vertical map |

---

## Enemy Types

| Enemy | Behaviour |
|-------|-----------|
| **Ghost** | Patrols platforms, flips direction at edges |
| **Skeleton** | Walks, jumps to higher platforms, occasionally drops down |

---

## Requirements

- C++17 or later
- [SFML 2.5+](https://www.sfml-dev.org/download.php) (Graphics, Audio, Window modules)
- A C++ compiler (g++, MSVC, Clang)

---

## Building & Running

### Linux / macOS
```bash
g++ tumblepop.cpp -o sfml-app -lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system
./sfml-app
```

### Windows (MinGW)
```bash
g++ tumblepop.cpp -o tumblepop.exe -lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system
tumblepop.exe
```

### Windows (Visual Studio)
Link against the SFML `.lib` files in your project properties and add the SFML `include` folder to your include path.

> **Important:** The `Data/` folder must be in the same directory as the executable when you run the game.

---

## Controls

| Key | Action |
|-----|--------|
| `Left` / `Right` Arrow | Move |
| `Space` | Jump |
| `Down` Arrow | Drop down |
| `A` / `D` | Aim vacuum sideways |
| `W` / `S` | Aim vacuum up / down |
| `C` | Shoot a bagged enemy |
| `P` or `Esc` | Pause / Resume |
| `R` | Restart (Game Over screen) |

---

## Scoring

| Event | Points |
|-------|--------|
| Ghost captured | 50 |
| Skeleton captured | 75 |
| Projectile kill | 2× base points |
| Combo ×3–4 | 1.5× multiplier |
| Combo ×5+ | 2× multiplier |
| Hit by enemy | −50 |

Combos time out after ~3 seconds of no captures.

---

## Required Assets (`Data/` folder)

All assets must be placed in a `Data/` folder next to the executable. The following files are required:

**Sprites**
- `bg1.png` — Main menu background
- `ca.png` — In-game background
- `blockgg.png` — Platform tile
- `rsbrick.png`, `sbrick.png` — Slope tiles (right and left)
- `rplayer.png`, `y.png` — Character select previews
- `RPGwalk.png`, `PGwalk.png` — Player 1 walk sprites (right / left)
- `RPYwalk.png`, `PYwalk.png` — Player 2 walk sprites (right / left)
- `bpr.png` — Backpack sprite
- `vacuum_full.png`, `rvacuum_full.png` — Vacuum sprites (left / right)
- `ghost.png` — Ghost enemy sprite
- `skeleton.png`, `rskeleton.png` — Skeleton sprites (left / right)
- `ChelnovWalk.png`, `RChelnovWalk.png` — Chelnov sprites (left / right)
- `fireball.png` — Chelnov projectile sprite
- `pot.png` — Destructible pot object

**Fonts**
- `Arial.ttf` — Menu font
- `qe.ttf` — Character select font
- `Steelar-j9Vnj.otf` — HUD font

**Audio**
- `game.wav` — Main menu music
- `mus.ogg` — In-game background music
- `click.wav` — Menu hover sound

---

## Project Structure

```
tumble-pop/
├── tumblepop.cpp       # Full game source
├── Data/               # All assets (see list above)
└── README.md
```

---

## License

MIT License — free to use, modify, and distribute.
