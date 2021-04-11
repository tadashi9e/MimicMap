all: MapTest

MapTest: MapTest.cc MimicMap.h RigidMap.h
	$(CXX) -Wall MapTest.cc -o $@
