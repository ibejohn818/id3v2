
BUILD_TYPE ?= debug
BUILD_TYPE_LOWER ?= $(shell echo $(BUILD_TYPE) | tr A-Z a-z)

clean:
	rm -rf cmake-*-*
	rm -rf ./dist

config:
	cmake . -B cmake-$(BUILD_TYPE_LOWER)-build -DCMAKE_BUILD_TYPE=$(BUILD_TYPE) -G "Unix Makefiles"

build:
	$(MAKE) -C cmake-$(BUILD_TYPE_LOWER)-build

gcc-build-image:
	docker buildx build -f docker/gcc.ubuntu.Dockerfile -t tools/ubuntu:gcc .

package-alma9:
	docker buildx build \
		-t id3v2lib:rpm \
		--output type=local,dest=./ \
		-f docker/rpm.Dockerfile .
