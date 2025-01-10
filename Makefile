CXX = g++


simulateur:
	cmake -B sinfourmis2025/ sinfourmis2025/
	$(MAKE) -C ./sinfourmis2025	
	cp ./sinfourmis2025/Sinfourmis ./
