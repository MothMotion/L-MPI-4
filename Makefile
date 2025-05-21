CC = mpicc
CCFLAGS = -Wall -Wextra -Iinclude

SRC_DIR = src
OBJ_DIR = obj
LSF_DIR = lsf

SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC))

LSF = $(wildcard $(LSF_DIR)/*.lsf)

TARGET = program

all: $(TARGET)

serial: CCFLAGS += -DSERIAL
serial: $(TARGET)
	mv $(TARGET) "$(TARGET)_serial"

parallel: $(TARGET)
	mv $(TARGET) "$(TARGET)_parallel"

$(TARGET): $(OBJ)
	$(CC) $(CCFLAGS) -o $@ $^

DEP := $(patsubst $(OBJ_DIR)/%.o, $(OBJ_DIR)/%.d, $(OBJ))
-include $(DEP)
DEPFLAGS = -MMD -MF $(@:.o=.d)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p obj
	$(CC) $(CCFLAGS) -c $< -o $@ $(DEPFLAGS)

clean:
	rm -f $(OBJ) $(DEP)

bsubload: $(TARGET)
	for lsf_script in $(LSF) ; do \
		bsub < $$lsf_script ; \
	done
