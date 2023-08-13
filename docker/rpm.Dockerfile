FROM rockylinux:9 as builder


RUN dnf install cmake make -y
RUN dnf group install "Development Tools" -y

WORKDIR /app

COPY . .

ARG BUILD_TYPE="Release"
RUN make config build BUILD_TYPE=${BUILD_TYPE}
RUN ls -l && make -C cmake-release-build package   


FROM scratch
COPY --from=builder /app/cmake*-build/*rpm /dist/
COPY --from=builder /app/cmake*-build/*gz /dist/
COPY --from=builder /app/cmake*-build/*sh /dist/
