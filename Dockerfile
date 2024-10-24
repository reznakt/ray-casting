FROM alpine AS builder

RUN apk update && apk add --no-cache \
  git \
  cmake \
  gcc \
  make \
  musl-dev \
  build-base \
  pkgconf \
  sdl2-dev \
  sdl2_gfx-dev

WORKDIR /ray-casting
COPY . .
WORKDIR /ray-casting/build
RUN cmake .. && make -j$(nproc)

FROM alpine AS runner

RUN apk update && apk add --no-cache \
  sdl2 \
  sdl2_gfx \
  wayland \
  wayland-protocols \
  mesa-dri-gallium \
  weston

COPY --from=builder /ray-casting/build/ app/

ENTRYPOINT ["app/ray-casting"]

