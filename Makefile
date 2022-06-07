#build & test automation

APP_NAME=pacman

build:
	gcc ${APP_NAME}.c -o ${APP_NAME}
test: build
	./${APP_NAME}
	make clean

clean:
	rm -rf ${APP_NAME}