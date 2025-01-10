CXX = g++

default: fourmis

simulateur:
	cmake -B sinfourmis2025/ sinfourmis2025/
	$(MAKE) -C ./sinfourmis2025	
	cp ./sinfourmis2025/Sinfourmis ./

simule: simulateur fourmis
	./Sinfourmis -t dummy -t ./fourmis.so ./sinfourmis2025/maps/test.json

animateur:
	npm --prefix SinfourmisAnimator/ i

map-creator:
	npm --prefix SinfourmisMapCreator/ i
	mkdir SinfourmisMapCreator/data

fourmis:
	$(MAKE) -C ./src/ fourmis.so
	cp ./src/fourmis.so ./

clean:
	$(RM) ./fourmis.so
	$(RM) ./src/fourmis.so
	$(RM) ./Sinfourmis
	$(RM) *.json


cleanall: clean
	$(MAKE) -C ./sinfourmis2025/ clean
