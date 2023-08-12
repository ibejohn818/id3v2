FROM rockylinux:9 as builder


RUN dnf install cmake make -y
RUN dnf group install "Development Tools" -y

WORKDIR /app

COPY . .

ARG BUILD_TYPE="Debug"
# RUN make config build BUILD_TYPE=${BUILD_TYPE}
RUN make config build
RUN make -C cmake-debug-build package


FROM scratch
COPY --from=builder /app /dist
