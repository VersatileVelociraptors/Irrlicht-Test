all:
	g++ -o irrlicht-test main.cpp -lIrrlicht -lGL -lX11 -lXxf86vm
clean:
	rm irrlicht-test
