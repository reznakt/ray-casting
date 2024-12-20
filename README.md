# 🚀 Ray Casting

![License](https://img.shields.io/github/license/reznakt/ray-casting)
![GitHub Workflow Status](https://img.shields.io/github/actions/workflow/status/reznakt/ray-casting/linux.yml)
![GitHub repo size](https://img.shields.io/github/repo-size/reznakt/ray-casting)
![GitHub repo file count](https://tokei.rs/b1/github/reznakt/ray-casting?category=files)
![GitHub repo LOC](https://tokei.rs/b1/github/reznakt/ray-casting?category=lines)

> A simple experiment with ray casting and C.

**Play online**: https://reznakt.github.io/ray-casting/

## 🤔 About the project

This project is a simple experiment with ray casting and C. It's not really a game, but rather a proof of concept. You
can walk around and observe the environment, which consists of walls and an open sky.

I previously made something similar in Python, but it was quite slow (~100 fps), so I wanted to see how fast it could be
in C (it's ~2000 fps). I also really like C and ray casting, so it's a fun project indeed.

## 💻 Built with

- C99
- CMake
- SDL2

> [!TIP]
> On Debian-based systems, you can install all necessary dependencies with:
> ```shell
> sudo apt-get install -y cmake libsdl2-dev libsdl2-gfx-dev
> ```

## 🚀 Getting Started

> [!TIP]
> You can also download
> a [pre-built binary](https://nightly.link/reznakt/ray-casting/workflows/linux/main/ray-casting.zip).

To get a local copy up and running, follow these simple steps:

```shell
git clone https://github.com/reznakt/ray-casting.git && cd ray-casting
cmake -B build/
cmake --build build/ -j$(nproc)
```

You can also run the app with Docker. Here's an example `docker-compose.yml` file:

```yaml
services:
  ray-casting:
    image: ghcr.io/reznakt/ray-casting:latest
    container_name: ray-casting
    tty: true
    user: 1000:1000
    environment:
      XDG_RUNTIME_DIR: /tmp
      WAYLAND_DISPLAY: $WAYLAND_DISPLAY
      SDL_VIDEODRIVER: wayland,x11
      DISPLAY: $DISPLAY
    volumes:
      - $XDG_RUNTIME_DIR/$WAYLAND_DISPLAY:/tmp/$WAYLAND_DISPLAY
      - /usr/share/wayland-sessions/:/usr/share/wayland-sessions/:ro
      - /tmp/.X11-unix:/tmp/.X11-unix:ro
    devices:
      - /dev/dri:/dev/dri
```

## 🎮 How to Play

Currently, there are no game features per se. You can walk around and observe the environment. However, you can
configure various parameters by modifying the header files:

- Screen size
- Field of view
- Number of rays
- Keymappings

## 📷 Screenshots

| ![image](https://github.com/reznakt/ray-casting/assets/56887011/b96d28c1-583f-4580-ab50-ed8af8d3f436) |
|:-----------------------------------------------------------------------------------------------------:| 
|                                             *Normal mode*                                             |

| ![image](https://github.com/reznakt/ray-casting/assets/56887011/41866d32-3a4d-4f0b-92db-74393efa8e9d) |
|:-----------------------------------------------------------------------------------------------------:| 
|                                              *Flat mode*                                              |

## 📝 License

This project is licensed under the MIT license - see the [LICENSE](LICENSE) file for details.

## ❤️ Acknowledgements

- [Lode Vandevenne](https://github.com/lvandeve) for the
  legendary [ray casting tutorial](http://lodev.org/cgtutor/raycasting.html)
