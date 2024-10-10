CC=clang-13
LINK=llvm-link-13
CFLAGS += -emit-llvm -g \
	-I ../modules -I ../modules/common \
	-I ../modules/lin_alg -I ../modules/model \
	-I /snap/klee/10/usr/local/include/klee 

SRC_DIR = ../modules
BC_DIR = ./bc

all: linked_model.bc
# all: KT_model.bc

$(BC_DIR)/KT_model.bc: KT_model.c 
	@mkdir -p $(BC_DIR)
	$(CC) $(CFLAGS) -c KT_model.c -o $(BC_DIR)/KT_model.bc

$(BC_DIR)/model.bc: $(SRC_DIR)/model/model.c
	@mkdir -p $(BC_DIR)
	$(CC) $(CFLAGS) -c $(SRC_DIR)/model/model.c -o $(BC_DIR)/model.bc

$(BC_DIR)/common.bc: $(SRC_DIR)/common/common.c
	@mkdir -p $(BC_DIR)
	$(CC) $(CFLAGS) -c $(SRC_DIR)/common/common.c -o $(BC_DIR)/common.bc

linked_model.bc: $(BC_DIR)/KT_model.bc $(BC_DIR)/model.bc
	@mkdir -p $(BC_DIR)
	$(LINK) $(BC_DIR)/KT_model.bc $(BC_DIR)/model.bc -o linked_model.bc

clean:
	rm -f *.bc *.o
	rm -rf klee-*
	rm -rf bc
