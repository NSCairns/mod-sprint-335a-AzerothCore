# mod-sprint

An AzerothCore module that introduces a dynamic stamina system, an additive sprint speed modifier, a customisable hold-to-sprint key, and a live tracking UI bar for World of Warcraft (3.3.5a).

100% AI VibeCoded, do not know C++ or LUA not gonna start now :)

---

## Features
* **True Hold-to-Sprint Mechanics**: Sprints while holding your designated key, stopping instantly upon release.
* **Additive Speed Modifier**: Stacks cleanly on top of your existing base speed rates.
* **Stamina Management**: Consumes stamina dynamically with a configurable pool.
* **Custom UI Bar**: Includes a movable, lockable stamina bar with real-time updates.
* **In-Game Configuration**: Easily rebind keys or lock the UI using simple chat commands.

---

## Commands
* `/sprintbind <KEY>` — Sets your hold-to-sprint key (e.g., `/sprintbind F`, `/sprintbind R`).
* `/sprintlock` — Toggles locking/unlocking the sprint bar position for drag-and-drop movement.

---

## Installation

### 1. Server-Side Module
1. Download or clone this repository.
2. Place the folder inside your AzerothCore source directory under: 
   `modules/mod-sprint`
3. Re-run your CMake configuration and compile AzerothCore.

### 2. Client-Side UI Addon
1. Locate the `ModSprintUI` folder included in this module.
2. Copy or move it into your World of Warcraft client directory:
   `Interface/AddOns/ModSprintUI`

---

## Configuration
You can modify settings like the speed multiplier bonus and max stamina via the module's configuration file (`mod_sprint.conf.dist` in your server config directory).

---

## License
This project is licensed under the **GNU Affero General Public License v3.0**.
