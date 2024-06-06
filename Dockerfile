FROM gcc:14.1.0-bookworm AS builder


ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && \
    apt-get install -y --no-install-recommends --no-install-suggests cmake libsdl2-dev libsdl2-gfx-dev

WORKDIR /app
COPY . .

RUN cmake -B build -S . && cmake --build build -- -j$(nproc)


FROM debian:bookworm-slim

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && \
    apt-get install -y --no-install-recommends --no-install-suggests libsdl2-2.0-0 libsdl2-gfx-1.0-0 libgomp1

COPY --from=builder /app/build/app /app/ray-casting
COPY --from=builder /app/build/test /app/test
COPY --from=builder /app/build/assets /app/assets

ENTRYPOINT ["/bin/sh", "-c", "/app/test && /app/ray-casting"]
