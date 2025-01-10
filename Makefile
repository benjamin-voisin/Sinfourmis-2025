CXX = g++


simulateur:
	cmake -B sinfourmis2025/ sinfourmis2025/
	$(MAKE) -C ./sinfourmis2025	
	cp ./sinfourmis2025/Sinfourmis ./

simule: simulateur fourmis
	./Sinfourmis --team ./fourmis.so --team ./fourmis.so ./sinfourmis2025/maps/test.json

animateur:
	npm --prefix SinfourmisAnimator/ i

fourmis:
	$(MAKE) -C ./src/ fourmis.so
	cp ./src/fourmis.so ./

clean:
	$(RM) ./fourmis.so
	$(RM) ./Sinfourmis


cleanall: clean
	$(MAKE) -C ./sinfourmis2025/ clean
