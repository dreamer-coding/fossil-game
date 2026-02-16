# Fossil Game by Fossil Logic

**Fossil Game** is a modular, cross-platform library for building text-based and turn-based games in pure C, with optional C++ wrappers. Designed to provide a consistent, extensible framework for player management, quizzes, scoring, and AI-driven gameplay, Fossil Game leverages **string IDs**, flexible type abstractions, and runtime-safe functional interfaces to enable rich game systems without sacrificing portability or performance.

Fossil Game acts as the game-engine backbone for building interactive text experiences, multiplayer sessions, and AI-assisted challenges, while maintaining a clean and auditable C interface.

## Key Features

- **Cross-Platform & ABI-Stable**  
  Fully portable across Windows, macOS, Linux, and embedded targets. Offers a C-friendly ABI with optional C++ namespace wrappers for convenience and safety.

- **Player & NPC Management**  
  Create, update, and remove players and AI-controlled NPCs. Supports inventories, per-player attributes, session management, and fine-grained control toggles.

- **Quiz & Knowledge Systems**  
  Build single-choice, multi-choice, or timed quizzes. Includes AI-assisted question generation and dynamic scoring for adaptive gameplay experiences.

- **Advanced Score & Matchmaking**  
  Track player performance with skill-based ranking systems, leaderboards, achievements, and matchmaking for both competitive and casual games.

- **String ID Architecture**  
  All entities—players, items, quizzes, and sessions—use string IDs for consistent runtime identification, dynamic registration, and serialization.

- **AI-Ready Integration**  
  NPC behavior, question generation, and matchmaking logic can leverage AI tricks, deterministic simulation, or scripted strategies while remaining fully auditable.

- **Extensible Modular Design**  
  Sub-libraries (`player`, `quizzed`, `score`) allow building everything from simple quizzes to multiplayer RPG-like experiences. Functions follow a `fossil_game_<domain>_<verb>` naming convention for clarity and discoverability.

- **Zero External Dependencies**  
  Implemented entirely in portable C, with optional C++ wrappers. Ideal for embedded, server, or text-based game environments where transparency, performance, and maintainability are critical.

- **Foundation for Text-Based Games**  
  Fossil Game provides a stable, AI-aware framework for building narrative-driven, competitive, or educational games, forming the base for future Fossil interactive systems.

## ***Prerequisites***

To get started, ensure you have the following installed:

- **Meson Build System**: If you don’t have Meson `1.8.0` or newer installed, follow the installation instructions on the official [Meson website](https://mesonbuild.com/Getting-meson.html).

### Adding Dependency

#### Adding via Meson Git Wrap

To add a git-wrap, place a `.wrap` file in `subprojects` with the Git repo URL and revision, then use `dependency('fossil-game')` in `meson.build` so Meson can fetch and build it automatically.

#### Integrate the Dependency:

Add the `fossil-game.wrap` file in your `subprojects` directory and include the following content:

```ini
[wrap-git]
url = https://github.com/fossillogic/fossil-game.git
revision = v0.1.0

[provide]
dependency_names = fossil-game
```

**Note**: For the best experience, always use the latest releases. Visit the [releases](https://github.com/fossillogic/fossil-game/releases) page for the latest versions.

## Build Configuration Options

Customize your build with the following Meson options:
	•	Enable Tests
To run the built-in test suite, configure Meson with:

```sh
meson setup builddir -Dwith_test=enabled
```

### Tests Double as Samples

The project is designed so that **test cases serve two purposes**:

- ✅ **Unit Tests** – validate the framework’s correctness.  
- 📖 **Usage Samples** – demonstrate how to use these libraries through test cases.  

This approach keeps the codebase compact and avoids redundant “hello world” style examples.  
Instead, the same code that proves correctness also teaches usage.  

This mirrors the **Meson build system** itself, which tests its own functionality by using Meson to test Meson.  
In the same way, Fossil Logic validates itself by demonstrating real-world usage in its own tests via Fossil Test.  

```bash
meson test -C builddir -v
```

Running the test suite gives you both verification and practical examples you can learn from.

## Contributing and Support

For those interested in contributing, reporting issues, or seeking support, please open an issue on the project repository or visit the [Fossil Logic Docs](https://fossillogic.com/docs) for more information. Your feedback and contributions are always welcome.
