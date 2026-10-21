# opengl-test

This repo contains all shader that I made reading [The Book of Shaders](https://thebookofshaders.com/) with a very handy program, I wrote for that.

## Features

This program can:

- Log shaders and shader program compilation with errors, space presses with shader switch.
- Hot reload shaders if they were updated in runtime.
- Add more shaders in runtime if more shaders were added in `./resources/fragment/` folder (sometimes doesn't work for some reason).
- Switch between shaders with `Space` key, toggle shader window resolution between 640x640 and 320x320 with `R` key.

## Build

1. Enter nix-shell with `nix-shell` or install dependencies:
    ```sh
    # Build systems
    cmake ninja \
    # OpenGL
    libGL \
    # If you use Wayland
    wayland-scanner wayland gtk3 libxkbcommon pkg-config \
    # If you use X11
    libX11 libXcursor libXi libXinerama libXrandr
    ```

2. Build:
    ```sh
    cmake -B build -G Ninja && cmake --build build
    ```

3. Run:
    ```sh
    build/game
    ```

## Shaders

### Algorithmic drawing

[Link to the book chapter](https://thebookofshaders.com/05/)

01-algorithm.frag:

![](./docs/01-algorithm.png)

02-algorithm.frag:

![](./docs/02-algorithm2.png)

03-step.frag:

![](./docs/03-step.png)

04-smoothstep.frag:

![](./docs/04-smoothstep.png)

05-sin.frag:

![](./docs/05-sin.gif)

06-mod.frag:

![](./docs/06-mod.gif)


### Colors

[Link to the book chapter](https://thebookofshaders.com/06/)

07-color1.frag:

![](./docs/07-color1.gif)

08-color2.frag:

![](./docs/08-color2.gif)

09-color3.frag:

![](./docs/09-color3.gif)

10-hsb.frag:

![](./docs/10-hsb.png)

11-hsb-polar.frag:

![](./docs/11-hsb-polar.gif)


### Shapes

[Link to the book chapter](https://thebookofshaders.com/07/)

12-shapes1.frag:

![](./docs/12-shapes1.png)

13-shapes2.frag:

![](./docs/13-shapes2.gif)

14-circle.frag:

![](./docs/14-circle.gif)

15-shapes3.frag:

![](./docs/15-shapes3.png)

16-polar-shapes.frag:

![](./docs/16-polar-shapes.gif)

17-polar-shapes2.frag:

![](./docs/17-polar-shapes2.gif)


### 2d matrices

[Link to the book chapter](https://thebookofshaders.com/08/)

18-2d-matrices.frag:

![](./docs/18-2d-matrices.gif)

19-rotation.frag:

![](./docs/19-rotation.gif)

20-scale.frag:

![](./docs/20-scale.gif)


### Patterns

[Link to the book chapter](https://thebookofshaders.com/09/)

21-patterns.frag:

![](./docs/21-patterns.gif)

22-patterns2.frag:

![](./docs/22-patterns2.gif)

23-bricks.frag:

![](./docs/23-bricks.gif)

24-tiles.frag:

![](./docs/24-tiles.gif)


### Generative designs

[Link to the book chapter](https://thebookofshaders.com/10/)

25-random.frag:

![](./docs/25-random.gif)

26-random2.frag:

![](./docs/26-random2.png)

27-random3.frag:

![](./docs/27-random3.gif)


### Noise

[Link to the book chapter](https://thebookofshaders.com/11/)

28-noise.frag:

![](./docs/28-noise.gif)

29-noise2.frag:

![](./docs/29-noise2.png)


### Cellular noise

[Link to the book chapter](https://thebookofshaders.com/12/)

30-mouse.frag:

![](./docs/30-mouse.gif)
