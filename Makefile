main: main.cpp ams.c
        g++ main.cpp ams.c prng.c massdal.c -lm -o main -lstdc++ -lgzstream -lz -Wall -O3

heavy: heavy.cpp
        g++ heavy.cpp ams.c prng.c massdal.c -lm -o heavy -lstdc++ -lgzstream -lz -Wall -O3

sample: sample.cpp
        g++ sample.cpp -o sample -lstdc++ -lgzstream -lz

readfile: readfile.cpp
        g++ readfile.cpp -o readfile -lstdc++ -lgzstream -lz

intersect: intersect.c
        gcc -o intersect intersect.c ams.c prng.c massdal.c -lm -Wall

clean:
        rm -rf intersect