all: performance_find.png performance_insert.png performance_op.png

PerformanceTest: PerformanceTest.cc ../MimicMap.h ../RigidMap.h
	$(CXX) -Wall -DNDEBUG -O3 -I.. PerformanceTest.cc -lboost_system -lboost_timer -o $@

do_performance_test:: PerformanceTest
	./PerformanceTest

%.png: %.plot %.dat
	gnuplot $<
