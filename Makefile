CXX ?= g++
CC ?= g++

CXXFLAGS = -fPIC

NAME = fourmis.so

SRC_DIR = ./src
BUILD_DIR = ./build

C_SOURCES = $(shell find $(SRC_DIR) -name '*.c')
CPP_SOURCES = $(shell find $(SRC_DIR) -name '*.cpp')

OBJECTS = $(addprefix $(BUILD_DIR)/, $(C_SOURCES:%.c=%.o))
OBJECTS += $(addprefix $(BUILD_DIR)/, $(CPP_SOURCES:%.cpp=%.o))

default: $(NAME)

$(NAME): $(OBJECTS)
	$(CXX) -o $(NAME) -shared -fPIC $(OBJECTS)

$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(BUILD_DIR)/$(dir $<)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c -o $@ $<

$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(BUILD_DIR)/$(dir $<)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c -o $@ $<

simulateur:
	cmake -B sinfourmis2025/ sinfourmis2025/
	$(MAKE) -C ./sinfourmis2025	
	cp ./sinfourmis2025/Sinfourmis ./

simuletest: simulateur $(NAME)
	./Sinfourmis -t dummy -t ./$(NAME) -d 50 ./sinfourmis2025/maps/test.json

simuletest_full: simulateur $(NAME)
	./Sinfourmis -t dummy -t ./$(NAME) -d 50 ./sinfourmis2025/maps/test.json

simule: simulateur $(NAME)
	./Sinfourmis -t dummy -t dummy -t dummy -t ./$(NAME) -d 50 ./sinfourmis2025/maps/spiral.json

simule_full: simulateur $(NAME)
	./Sinfourmis -t dummy -t dummy -t dummy -t ./$(NAME) ./sinfourmis2025/maps/spiral.json


animateur:
	npm --prefix SinfourmisAnimator/ i

map-creator:
	npm --prefix SinfourmisMapCreator/ i
	mkdir SinfourmisMapCreator/data


clean:
	$(RM) ./fourmis.so
	$(RM) ./src/fourmis.so
	$(RM) ./Sinfourmis
	$(RM) *.json
	$(RM) -r $(BUILD_DIR)


cleanall: clean
	$(MAKE) -C ./sinfourmis2025/ clean
