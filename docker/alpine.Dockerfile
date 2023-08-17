FROM alpine:3.17

RUN apk --update --no-cache add \
 build-base clang15 clang15-extra-tools make cmake valgrind gdb lldb


WORKDIR /app

RUN make config
RUN make build
