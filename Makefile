all: shaderlabframework

run: shaderlabframework
	./ShaderLabFramework

shaderlabframework: clean
	mkdir -p build
	cd build && cmake ..
	cd build && make -j 8
	mv build/ShaderLabFramework ./

clean:
	$(RM) -r build ShaderLabFramework

.PHONY: all clean
