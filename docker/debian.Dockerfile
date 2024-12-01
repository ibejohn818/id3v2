FROM debian:12 as builder

RUN apt update && \
  apt install -y build-essential cmake libsodium-dev

WORKDIR /app
COPY . .

ARG BUILD_TYPE="Release"
RUN make config build BUILD_TYPE=${BUILD_TYPE}
RUN ls -l && make -C cmake-release-build package   


FROM scratch
COPY --from=builder /app/cmake*-build/*deb /dist/
COPY --from=builder /app/cmake*-build/*gz /dist/
COPY --from=builder /app/cmake*-build/*sh /dist/

