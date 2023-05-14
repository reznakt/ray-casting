# 🚀 Ray Casting

> A simple experiment with ray casting and C.

## 🤔 About the project

This project is a simple experiment with ray casting and C. It's not really a game, but rather a proof of concept. You can walk around and observe the environment, which consists of walls and an open sky. 

I previously made something similar in Python, but it was quite slow (~100 fps), so I wanted to see how fast it could be in C (it's ~2000 fps). I also really like C and ray casting, so it's a fun project indeed.

## 💻 Built with

- C99
- CMake
- SDL2

## 🚀 Getting Started

To get a local copy up and running, follow these simple steps:

1. Clone the repository: `git clone https://github.com/reznakt/ray-casting.git`
2. Compile the code: `cd ray-casting && mkdir build && cd build && cmake .. && make`
3. Run the executable: `./RayCastingSDL`

## 🎮 How to Play

Currently, there are no game features per se. You can walk around and observe the environment. However, you can configure various parameters by modifying the header files:

- Screen size
- Field of view
- Number of rays
- Keymappings

## 📷 Screenshots

![image](https://github.com/reznakt/ray-casting/assets/56887011/b96d28c1-583f-4580-ab50-ed8af8d3f436)


## 📝 License

This project is licensed under the GNU General Public License v3.0 - see the [LICENSE](LICENSE) file for details.

## ❤️ Acknowledgements

- [Lode Vandevenne](http://lodev.org/cgtutor/raycasting.html) for the excellent tutorial on ray casting.

